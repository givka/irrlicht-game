#include <irrlicht.h>
#include <iostream>
#include "enemy.hpp"

using namespace irr;
namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Enemy::Enemy()
    : m_health(100), m_damage(15), m_scale(1.0f), m_speed(1.0f), m_already_hit_player(false), m_swing_timer(1000)
{
    m_last_swing_time = 0;
    m_max_health = m_health;
    m_souls = (int)(m_scale * (20 + rand() % 20));
}
Enemy::Enemy(float health, int damage, float scale, float speed, int model_id, int swing_timer)
    : m_health(health), m_damage(damage), m_scale(scale), m_speed(speed), m_already_hit_player(false), m_swing_timer(swing_timer), m_model_id(model_id)
{
    m_last_swing_time = 0;
    m_max_health = m_health;
    m_souls = (int)(m_scale * (20 + rand() % 20));
}

void Enemy::initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector, int enemy_id)
{

    auto smgr = device->getSceneManager();
    float const X = rand() % 200;
    float const Z = rand() % 200;
    m_device = device;
    m_id = enemy_id;
    m_node = smgr->addAnimatedMeshSceneNode(mesh);
    m_node->setMaterialFlag(iv::EMF_LIGHTING, true);
    m_node->setMD2Animation(is::EMAT_RUN);
    m_node->setMaterialTexture(0, texture);
    m_node->setPosition(core::vector3df(X, 0, Z));
    m_node->setScale(core::vector3df(m_scale, m_scale, m_scale));

    ic::vector3df boxMaxEdge = m_node->getTransformedBoundingBox().MaxEdge;
    ic::vector3df boxCenter = m_node->getTransformedBoundingBox().getCenter();
    ic::vector3df ellipseRadius = boxMaxEdge - boxCenter;

    is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(selector, m_node, ellipseRadius);
    m_node->addAnimator(collision);
    collision->drop();

    m_health_bar_size = ellipseRadius.Y / 2.0;
    m_health_bar = smgr->addBillboardSceneNode(m_node,
                                               ic::dimension2d<irr::f32>(m_health_bar_size, 1),
                                               ic::vector3df(10, 30, 0), -1, iv::SColor(100, 255, 0, 0), iv::SColor(123, 255, 0, 0));
    m_health_bar_bg = smgr->addBillboardSceneNode(m_node,
                                                  ic::dimension2d<irr::f32>(m_health_bar_size, 1),
                                                  ic::vector3df(9.99, 30, 0), -1, iv::SColor(123, 0, 0, 0), iv::SColor(123, 0, 0, 0));

    m_health_bar->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL);
    m_health_bar->setMaterialFlag(iv::EMF_LIGHTING, false);
    m_health_bar_bg->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL);
    m_health_bar_bg->setMaterialFlag(iv::EMF_LIGHTING, false);
    m_font = m_device->getGUIEnvironment()->getFont("data/myfont.xml");
}

//update functions
void Enemy::switchToState(Enemy::enemy_state state, Player &player)
{
    switch (state)
    {
    case IDLE:
        m_node->setMD2Animation(is::EMAT_STAND);
        m_node->setLoopMode(true);
        break;
    case WALKING:
        m_node->setMD2Animation(is::EMAT_RUN);
        m_node->setLoopMode(true);
        break;
    case ATTACKING:
        m_node->setMD2Animation(is::EMAT_ATTACK);
        m_last_swing_time = m_device->getTimer()->getTime();
        m_node->setLoopMode(false);
        m_already_hit_player = false;
        break;
    case STAGGERED:
        m_node->setMD2Animation(is::EMAT_PAIN_A);
        m_node->setLoopMode(false);
        m_node->setCurrentFrame(m_node->getStartFrame());
        m_knockback_dir = m_node->getPosition() - player.getPosition();
        break;
    case DYING:
        m_node->setMD2Animation(is::EMAT_DEATH_FALLBACK);
        m_death_dir = m_node->getPosition() - player.getPosition();
        m_knockback_dir = m_node->getPosition() - player.getPosition();
        m_death_time = m_device->getTimer()->getTime();
        m_node->setLoopMode(false);
        m_health_bar->remove();
        m_health_bar_bg->remove();
        break;
    case DEAD:
        player.addSoulsEffect({m_souls, Player::ST_MONEY, iv::SColor(255, 255, 255, 255), m_node->getPosition(), 0});
        m_node->remove();
        m_node = 0;
        break;
    }
    m_state = state;
}
void Enemy::update(Player &player, std::vector<Enemy> enemies)
{
    switch (m_state)
    {
    case IDLE:
        if (!isAtRange(player)) //check if is still at range of player
            switchToState(WALKING, player);
        else if (m_last_swing_time + m_swing_timer < m_device->getTimer()->getTime()) //check if attack possible
            switchToState(ATTACKING, player);
        break;
    case WALKING:
        updateRotation(player); //might need to happen out for idle aswell
        updatePosition(enemies);
        if (isAtRange(player)) //check if is at range of player
            switchToState(IDLE, player);
        break;
    case ATTACKING:
        if (m_node->getEndFrame() - m_node->getFrameNr() <= 1) // if animation over
            switchToState(IDLE, player);
        attackPlayer(player);
        break;
    case STAGGERED:
        if (m_node->getEndFrame() - m_node->getFrameNr() <= 1) // if animation over
            switchToState(IDLE, player);
        updateKnockback(player);
        //update position //todo: knockback
        break;
    case DYING:
        updateDeath(player);
    default:
        return;
    }

    if (isBeingAttacked(player))
    {
        if (m_state != STAGGERED && m_state != DYING)
        {
            switchToState(STAGGERED, player);
        }
    }
    checkDoT(player);
    updateDamageText();
    checkBloodTimer();
}
void Enemy::updatePosition(std::vector<Enemy> enemies)
{
    float speed_rotation = 2.5f * m_speed;

    ic::vector3df position_enemy = m_node->getPosition();
    ic::vector3df rotation_enemy = m_node->getRotation();

    position_enemy.X += speed_rotation * cos((rotation_enemy.Y) * M_PI / 180.0);
    position_enemy.Z -= speed_rotation * sin((rotation_enemy.Y) * M_PI / 180.0);

    ic::vector3df extent = m_node->getTransformedBoundingBox().getExtent();
    extent.Y = 0;
    float enemy_radius = (float)((Utils::maxComponent(extent)) * 0.5);
    float neighbor_radius = 0.0f;

    for (size_t i = 0; i < enemies.size(); i++)
    {
        Enemy neighbour = enemies[i];
        if (m_id != neighbour.m_id)
        {
            extent = m_node->getTransformedBoundingBox().getExtent();
            extent.Y = 0;
            neighbor_radius = (float)(Utils::maxComponent(extent) * 0.5);

            if (m_node->getTransformedBoundingBox().getCenter().getDistanceFrom(neighbour.getNode()->getTransformedBoundingBox().getCenter()) < enemy_radius + neighbor_radius)
                return;
        }
    }
    m_node->setPosition(position_enemy);
}
// Enemy will always face player
void Enemy::updateRotation(Player &player)
{
    core::vector3df position_player = player.getPosition();
    core::vector3df position_enemy = m_node->getPosition();
    core::vector3df rotation_enemy = m_node->getRotation();

    core::vector3df position_diff = position_player - position_enemy;
    rotation_enemy.Y = atan(position_diff.Z / position_diff.X) * (180.0f / irr::core::PI);
    if ((position_player.X - position_enemy.X) > 0)
    {
        rotation_enemy.Y = 90 - rotation_enemy.Y;
    }
    else if ((position_player.X - position_enemy.X) < 0)
    {
        rotation_enemy.Y = -90 - rotation_enemy.Y;
    }
    rotation_enemy.Y -= 90;
    m_node->setRotation(rotation_enemy);
}
void Enemy::updateKnockback(Player &player)
{
    float speed = 0.05f / m_scale;
    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    position.X += speed * m_knockback_dir.X;
    position.Y += speed * m_knockback_dir.Y + 5.0;
    position.Z += speed * m_knockback_dir.Z;

    if (m_state == DYING)
    {
        rotation.X += speed * m_knockback_dir.X;
        rotation.Y += speed * m_knockback_dir.Y;
        rotation.Z += speed * m_knockback_dir.Z;
    }

    m_node->setPosition(position);
    m_node->setRotation(rotation);
}
void Enemy::updateDeath(Player &player)
{
    int time_dying = m_device->getTimer()->getTime() - m_death_time;

    if (time_dying >= 1500 || m_node->getFrameNr() == m_node->getEndFrame())
        return switchToState(DEAD, player);

    if (m_last_hit_type != DT_DOT)
        updateKnockback(player);
}

void Enemy::attackPlayer(Player &player)
{
    if (m_already_hit_player || m_node->getFrameNr() - m_node->getStartFrame() <= (m_node->getEndFrame() - m_node->getStartFrame()) / 4.0) //only attack by end of swing to let player avoid/parry
        return;

    if (isAtRange(player))
    {
        //player was hit
        m_already_hit_player = true;
        if (player.isBlocking())
        {
            std::cout << "blocked" << std::endl; //todo: particle effect
        }
        else
        {
            player.takeDamage(m_damage); //todo: screen flash / particle effect
        }
    }
}

bool Enemy::isBeingAttacked(Player &player)
{
    if (!isAlive())
        return false;

    Sword sword = player.getSword();

    if (sword.getIsAttacking() && m_last_swing_number != sword.getSwingNumber())
    {
        bool is_attacked = sword.getNode()->getTransformedBoundingBox().intersectsWithBox(m_node->getTransformedBoundingBox());

        if (is_attacked)
        {
            const bool is_crit = rand() % 101 <= sword.getCritPercent();
            const float damage = is_crit ? 2 * sword.getAttack() : sword.getAttack();
            removeHealth(player, damage, is_crit ? DT_CRIT : DT_NORMAL);
            addBloodEffect(is_crit ? DT_CRIT : DT_NORMAL);
            player.enemyHitCallback();
            if (isAlive())
            {
                m_last_swing_number = sword.getSwingNumber();
                checkEnchantment(player);
            }
        }
        return is_attacked;
    }
    return false;
}
void Enemy::checkEnchantment(Player &player)
{
    Sword::enchant current_enchant = player.getSword().getCurrentEnchant();

    if (current_enchant == Sword::NONE)
        return;

    m_current_effect = current_enchant;

    switch (current_enchant)
    {
    case Sword::FIRE:
        setEffect(player, ic::vector3df(0, 0.2f, 0));
        break;
    case Sword::FROST:
        setEffect(player, ic::vector3df(0, 0.2f, 0));
        break;
    case Sword::VAMPIRIC:
        player.addSoulsEffect({VAMPIRIC_HEAL, Player::ST_HEALTH, player.getSword().getEnchantColor(Sword::VAMPIRIC), m_node->getPosition(), 0});
        break;
    case Sword::POISON:
        setEffect(player, ic::vector3df(0, 0.2f, 0));
        break;

    default:
        break;
    }
}
void Enemy::checkDoT(Player &player)
{
    if (!isAlive())
        return;

    if (m_current_effect == Sword::NONE || m_current_effect == Sword::VAMPIRIC)
        return;

    if (m_dot_tick_number > 3)
    {
        m_current_effect = Sword::NONE;
        m_dot_tick_number = 0;
        m_last_dot_time = 0;
        m_effect_node->remove();
        m_effect_node = 0;
        m_speed = 1.0;
        return;
    }

    const int time = m_device->getTimer()->getTime();

    // DoT every second
    if (time - m_last_dot_time < 1000)
        return;

    m_last_dot_time = time;
    m_dot_tick_number++;

    if (m_current_effect == Sword::FIRE)
        removeHealth(player, resistance_fire * DOT_DAMAGE, DT_DOT);
    else if (m_current_effect == Sword::POISON)
        removeHealth(player, resistance_poison * DOT_DAMAGE, DT_DOT);
    else if (m_current_effect == Sword::FROST)
        m_speed = FROST_SPEED;
}
void Enemy::checkBloodTimer()
{
    if (!m_blood_node)
        return;

    if (m_device->getTimer()->getTime() - m_blood_timer > 300)
    {
        m_blood_node->remove();
        m_blood_node = 0;
        m_blood_timer = 0;
    }
}

void Enemy::removeHealth(Player &player, const float damage, damage_type dt)
{
    m_health -= damage;
    m_last_hit_type = dt;

    std::cout << "current model hit: " << m_model_id << std::endl;

    float health_bar_size = m_health_bar_size * m_health / m_max_health;
    m_health_bar->setSize(ic::dimension2df(health_bar_size, 1));
    // can't manage to align health bar left...
    // m_health_bar->setPosition(ic::vector3df(10, 30, -health_bar_size / 2.0));

    addDamageText(player, damage, dt);

    if (m_health <= 0)
    {
        switchToState(DYING, player);
    }
}
void Enemy::addDamageText(Player &player, const float damage, damage_type dt)
{

    float size = (dt == DT_CRIT ? 0.005 : 0.0025) * m_device->getVideoDriver()->getScreenSize().Width;

    iv::SColor text_color = dt == DT_DOT
                                ? player.getSword().getEnchantColor(m_current_effect)
                                : iv::SColor(200, 200, 200, 200);
    if (dt == DT_CRIT)
        text_color = iv::SColor(200, 200, 200, 0);

    std::string damage_text = std::to_string((int)damage);
    std::wstring damage_wtext = std::wstring(damage_text.begin(), damage_text.end());

    irr::scene::IBillboardTextSceneNode *node = m_device->getSceneManager()->addBillboardTextSceneNode(m_font, damage_wtext.c_str(), m_node, irr::core::dimension2d<irr::f32>((size), (size)), irr::core::vector3df(10, 10, 0), -1, text_color, text_color);
    const ic::vector3df direction = ic::vector3df(10, rand() % 10, -10 + rand() % 20);
    m_damage_texts.push_back({node, (int)m_device->getTimer()->getTime(), direction});
}
void Enemy::updateDamageText()
{

    for (size_t index = 0; index < m_damage_texts.size();)
    {
        ic::vector3df position = m_damage_texts[index].node->getPosition();
        position.Y += m_damage_texts[index].direction.Y / 10.0;
        position.Z += m_damage_texts[index].direction.Z / 10.0;
        m_damage_texts[index].node->setPosition(position);

        if (m_device->getTimer()->getTime() - m_damage_texts[index].creation_time > 1000)
        {
            m_damage_texts[index].node->remove();
            m_damage_texts[index].node = 0;
            m_damage_texts.erase(m_damage_texts.begin() + index);
        }
        else
            index++;
    }
}
void Enemy::setEffect(Player &player, ic::vector3df direction)
{
    Sword sword = player.getSword();
    iv::SColor color = sword.getCurrentEnchantColor();

    if (!m_effect_node)
    {
        m_effect_node = Utils::setParticuleSystem(m_device, m_node, ic::vector3df(0, -20, 0), color);
        is::IParticleEmitter *emitter = m_effect_node->getEmitter();
        emitter->setMaxLifeTime(200);
        emitter->setMaxAngleDegrees(10);
        emitter->setMaxStartSize(ic::dimension2df(40, 40));
        emitter->setDirection(direction);
    }
    else
    {
        is::IParticleEmitter *emitter = m_effect_node->getEmitter();
        emitter->setMaxStartColor(color);
    }
}
void Enemy::addBloodEffect(damage_type dt)
{

    const int size = dt == DT_CRIT ? 20 : 10;

    if (!m_blood_node)
    {
        m_blood_node = Utils::setParticuleSystem(m_device, m_node, ic::vector3df(10, 0, 0), iv::SColor(255, 255, 0, 0));
        is::IParticleEmitter *emitter = m_blood_node->getEmitter();
        emitter->setMaxLifeTime(300);
        emitter->setMaxAngleDegrees(360);
        emitter->setMaxStartSize(ic::dimension2df(size, size));

        std::string blood_texture = "data/blood/particules/" + std::to_string(rand() % 5) + ".png";
        std::wstring path = std::wstring(blood_texture.begin(), blood_texture.end());
        m_blood_node->setMaterialTexture(0, m_device->getVideoDriver()->getTexture(path.c_str()));
        m_blood_node->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL);
    }
    else
    {
        is::IParticleEmitter *emitter = m_blood_node->getEmitter();
        emitter->setDirection(ic::vector3df(((float)(rand() % 2)) / 10.0, ((float)(rand() % 2)) / 10.0, ((float)(rand() % 2)) / 10.0));
    }

    m_blood_timer = m_device->getTimer()->getTime();
}

//accesseurs
bool Enemy::isDead()
{
    return m_state == DEAD;
}
is::IAnimatedMeshSceneNode *Enemy::getNode()
{
    return m_node;
}
void Enemy::setPosition(ic::vector3df pos)
{
    m_node->setPosition(pos);
}
void Enemy::setOrientation(ic::vector3df ori)
{
    m_node->setRotation(ori);
}
bool Enemy::isAlive()
{
    return m_state != DYING && m_state != DEAD;
}

bool Enemy::isAtRange(Player &player)
{
    float dist = player.getPosition().getDistanceFrom(m_node->getPosition());
    if (dist < 55) //todo: bounding box stuff
    {
        return true;
    }
    return false;
}

void Enemy::removeNodes()
{
    m_node->remove();
    m_node = 0;
    m_state = DEAD;
    m_health_bar->remove();
    m_health_bar_bg->remove();
}
