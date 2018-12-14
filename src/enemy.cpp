#include <irrlicht.h>
#include <iostream>

#include "enemy.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Enemy::Enemy() : m_health(100), m_damage(15), m_scale(1.0f), m_already_hit_player(false), m_swing_timer(1000)
{
    m_last_swing_time = 0;
}

Enemy::Enemy(float health, int damage, float scale, int swing_timer) : m_health(health), m_damage(damage), m_scale(scale), m_already_hit_player(false), m_swing_timer(swing_timer)
{
    m_last_swing_time = 0;
}

void Enemy::initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector, int enemy_id)
{

    auto smgr = device->getSceneManager();
    auto driver = device->getVideoDriver();
    float const X = rand() % 200;
    float const Z = rand() % 200;
    m_device = device;
    m_id = enemy_id;
    m_node = smgr->addAnimatedMeshSceneNode(mesh);
    m_node->setMaterialFlag(iv::EMF_LIGHTING, true);
    m_node->setMD2Animation(is::EMAT_RUN);
    m_node->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
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
}

void Enemy::update(Player &player, std::vector<Enemy> enemies)
{
    updateDamageText();
    checkBloodTimer();

    if (m_state == IS_DYING)
        return updateDeath();

    checkDoT(player);
    if (isBeingAttacked(player))
        return;
    updateRotation(player);
    if (!isAttacking(player))
        updatePosition(enemies);
    if (m_state == IS_ATTACKING)
        attackPlayer(player);
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

bool Enemy::isAttacking(Player &player)
{
    if (!isAlive())
        return false;
    if (m_state == IS_ATTACKING)
    {
        if (m_node->getEndFrame() - m_node->getFrameNr() <= 1)
        {
            m_state = IS_RUNNING;
            m_node->setMD2Animation(is::EMAT_RUN);
            m_already_hit_player = false;
            return false;
        }
        return true;
    }

    ic::vector3df position_player = player.getPosition();
    ic::vector3df position_enemy = m_node->getPosition();
    const float distance = position_player.getDistanceFrom(position_enemy);

    if (distance < 40)
    {
        if( m_last_swing_time + m_swing_timer < m_device->getTimer()->getTime()){
            m_state = IS_ATTACKING;
            m_node->setMD2Animation(is::EMAT_ATTACK);
            m_last_swing_time = m_device->getTimer()->getTime();
        }
        else if(m_state != IS_STOPPED)
        {
            m_state = IS_STOPPED;
            m_node->setMD2Animation(is::EMAT_STAND);
        }

        return true;
    }
    else if(m_state == IS_STOPPED)
    {
        m_state = IS_RUNNING;
        m_node->setMD2Animation(is::EMAT_RUN);
    }
    return false;
}

void Enemy::updatePosition(std::vector<Enemy> enemies)
{
    float speed_rotation = 2.5f;

    ic::vector3df position_enemy = m_node->getPosition();
    ic::vector3df rotation_enemy = m_node->getRotation();

    position_enemy.X += speed_rotation * cos((rotation_enemy.Y) * M_PI / 180.0);
    position_enemy.Z -= speed_rotation * sin((rotation_enemy.Y) * M_PI / 180.0);

    for (size_t i = 0; i < enemies.size(); i++)
    {
        Enemy neighbour = enemies[i];
        if (m_id != neighbour.m_id)
        {
            ic::vector3df position_neighbour = neighbour.getNode()->getPosition();
            float distX = position_neighbour.X - position_enemy.X;
            float distZ = position_neighbour.Z - position_enemy.Z;
            if (abs(distX) < 10 && abs(distZ) < 10)
                return;
        }
    }

    m_node->setPosition(position_enemy);
    m_last_position = position_enemy;
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

void Enemy::updateDeath()
{

    int time_dying = m_device->getTimer()->getTime() - m_death_time;
    float speedDying = 0.1f;

    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if (time_dying >= 1500 || m_node->getFrameNr() == m_node->getEndFrame())
    {
        m_state = IS_DEAD;
        m_node->remove();
        m_node = 0;
        return;
    }

    m_last_position = position;
    position.X += speedDying * m_death_dir.X;
    position.Y += speedDying * m_death_dir.Y + 5.0;
    position.Z += speedDying * m_death_dir.Z;

    rotation.X += speedDying * m_death_dir.X;
    rotation.Y += speedDying * m_death_dir.Y;
    rotation.Z += speedDying * m_death_dir.Z;

    // m_node->setPosition(position);
    // m_node->setRotation(rotation);
}

bool Enemy::isDead()
{
    return m_state == IS_DEAD;
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

bool Enemy::isBeingAttacked(Player &player)
{
    if (!isAlive())
        return false;

    ic::vector3df position_player = player.getPosition();
    ic::vector3df position_enemy = m_node->getPosition();
    ic::vector3df rotation_player = player.getRotation();
    ic::vector3df rotation_enemy = m_node->getRotation();
    Sword sword = player.getSword();

    if (sword.getIsAttacking() && m_last_swing_number != sword.getSwingNumber())
    {
        float const distance = position_player.getDistanceFrom(position_enemy);
        float const angle = sin((rotation_player.Y - rotation_enemy.Y) * M_PI / 180.0);
        bool is_attacked = distance < 50.0 && angle <= -0.80f;

        // enemy in front of us => angle = -1 or fall under map
        // cone from -0.8 -> -1 <- -0.8
        if (is_attacked || position_enemy.Y < -20)
        {
            removeHealth(player, sword.getAttack());
            addBloodEffect();
            if (isAlive())
            {
                m_last_swing_number = sword.getSwingNumber();
                checkEnchantment(player);
            }
        }
        return true;
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
    case Sword::ICE:
        setEffect(player, ic::vector3df(0, 0.2f, 0));
        break;
    case Sword::VAMPIRIC:
        setEffect(player, ic::vector3df(0, 0.2f, 0));
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
    if (m_current_effect == Sword::NONE)
        return;

    if (m_dot_tick_number > 3)
    {
        m_current_effect = Sword::NONE;
        m_dot_tick_number = 0;
        m_last_dot_time = 0;
        m_effect_node->remove();
        m_effect_node = 0;
        return;
    }

    const int time = m_device->getTimer()->getTime();

    // DoT every second
    if (time - m_last_dot_time < 1000)
        return;

    m_last_dot_time = time;
    m_dot_tick_number++;

    switch (m_current_effect)
    {
    case Sword::NONE:
        break;
    case Sword::FIRE:
        removeHealth(player, resistance_fire * DOT_DAMAGE);
        break;
    case Sword::ICE:
        break;
    case Sword::VAMPIRIC:
        break;
    case Sword::POISON:
        removeHealth(player, resistance_poison * DOT_DAMAGE);
        break;
    }
}

void Enemy::removeHealth(Player &player, const float damage)
{
    m_health -= damage;

    float health_bar_size = m_health_bar_size / 100.0 * m_health;
    m_health_bar->setSize(ic::dimension2df(health_bar_size, 1));
    // can't manage to align health bar left...
    // m_health_bar->setPosition(ic::vector3df(10, 30, -health_bar_size / 2.0));

    addDamageText(player, damage);

    if (m_health <= 0)
    {
        m_death_dir = m_node->getPosition() - player.getPosition();
        m_state = IS_DYING;
        m_death_time = m_device->getTimer()->getTime();
        m_node->setMD2Animation(is::EMAT_DEATH_FALLBACK);
        m_node->setLoopMode(false);
        m_health_bar->remove();
        m_health_bar_bg->remove();
    }
    std::cout << std::to_string(m_id) << ": health remaining: " << m_health << std::endl;
}

void Enemy::addDamageText(Player &player, const float damage)
{
    iv::SColor text_color = player.getSword().getCurrentEnchantColor();

    std::string damage_text = std::to_string((int)damage);
    std::wstring damage_wtext = std::wstring(damage_text.begin(), damage_text.end());

    irr::scene::IBillboardTextSceneNode *node = m_device->getSceneManager()->addBillboardTextSceneNode(0, damage_wtext.c_str(), m_node, irr::core::dimension2d<irr::f32>((10.0F), (10.0F)), irr::core::vector3df(10, 10, 0), -1, text_color, text_color);

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

bool Enemy::isAlive()
{
    return m_state != IS_DYING && m_state != IS_DEAD;
}

void Enemy::attackPlayer(Player &player)
{
    if (m_already_hit_player || m_node->getFrameNr() - m_node->getStartFrame() <= (m_node->getEndFrame() - m_node->getStartFrame()) / 4.0) //only attack by end of swing to let player avoid/parry
        return;
    ic::vector3df position_player = player.getPosition();
    ic::vector3df position_enemy = m_node->getPosition();
    const float distance = position_player.getDistanceFrom(position_enemy);

    if (distance < 40)
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

void Enemy::addBloodEffect()
{

    if (!m_blood_node)
    {
        m_blood_node = Utils::setParticuleSystem(m_device, m_node, ic::vector3df(10, 0, 0), iv::SColor(255, 255, 0, 0));
        is::IParticleEmitter *emitter = m_blood_node->getEmitter();
        emitter->setMaxLifeTime(300);
        emitter->setMaxAngleDegrees(40);

        std::string blood_texture = "data/blood" + std::to_string(rand() % 5 + 1) + ".png";
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
