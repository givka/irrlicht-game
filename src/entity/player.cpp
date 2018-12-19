#include <irrlicht.h>

#include "player.hpp"

Player::Player()
{
    m_max_health = 200;
    m_max_stamina = 200;
    m_health = m_max_health;
    m_stamina = m_max_stamina;
}

void Player::initialise(irr::IrrlichtDevice *device, is::ITriangleSelector *selector)
{
    is::ISceneManager *smgr = device->getSceneManager();

    m_device = device;

    smgr->addCameraSceneNodeFPS();
    m_node = smgr->getActiveCamera();
    device->getCursorControl()->setVisible(false);
    m_node->setFOV(MIN_FOV);

    m_node->setPosition(ic::vector3df(0, 150, 0));

    m_sword.initialise(device, m_node);

    m_collision = smgr->createCollisionResponseAnimator(
        selector, m_node, ic::vector3df(30, 30, 30),
        ic::vector3df(0, -10, 0), ic::vector3df(0, 15, 0));
    m_node->addAnimator(m_collision);
    // collision->drop();
}

void Player::update(EventReceiver &receiver)
{
    updateBloodScreen();
    updateSoulsEffects();
    updatePosition(receiver);
}

void Player::updateBloodScreen()
{
    if (m_hit_alpha != 0)
    {
        if (m_hit_image)
            m_hit_image->remove();

        m_hit_alpha -= 1;
        const int width = m_device->getVideoDriver()->getScreenSize().Width;
        const int height = m_device->getVideoDriver()->getScreenSize().Height;
        m_hit_image = m_device->getGUIEnvironment()->addImage(ic::recti(0, 0, width, height));
        m_hit_image->setImage(m_device->getVideoDriver()->getTexture("data/screen/hit.png"));
        m_hit_image->setColor(iv::SColor(m_hit_alpha, 255, 255, 255));
        m_hit_image->setScaleImage(true);
    }

    if (m_teleport_alpha != 0)
    {
        if (m_teleport_image)
            m_teleport_image->remove();

        m_teleport_alpha -= 1;
        const int width = m_device->getVideoDriver()->getScreenSize().Width;
        const int height = m_device->getVideoDriver()->getScreenSize().Height;
        m_teleport_image = m_device->getGUIEnvironment()->addImage(ic::recti(0, 0, width, height));
        m_teleport_image->setImage(m_device->getVideoDriver()->getTexture("data/screen/teleport.png"));
        m_teleport_image->setColor(iv::SColor(m_teleport_alpha, 255, 255, 255));
        m_teleport_image->setScaleImage(true);
    }

    float screen_speed = 5;
    for (size_t i = 0; i < m_blood_screens.size();)
    {
        if (m_blood_screens[i].alpha > screen_speed)
        {
            m_blood_screens[i].alpha -= screen_speed;
            m_blood_screens[i].image->setColor(iv::SColor(m_blood_screens[i].alpha, 255, 255, 255));
            i++;
        }
        else
        {
            m_blood_screens[i].image->remove();
            m_blood_screens[i].image = 0;
            m_blood_screens.erase(m_blood_screens.begin() + i);
        }
    }
}

void Player::addMaxHealth(int increment)
{
    m_max_health += increment;
    m_health += increment;
}

void Player::updatePosition(EventReceiver &receiver)
{
    checkTeleportation();
    bool *states = receiver.getStates();
    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if (states[EventReceiver::KEY_BLOCK])
    {
        if (!m_blocking && !m_sword.getIsAttacking() && m_stamina >= 10 && !m_waiting_for_unblock)
        {
            m_blocking = true;
            m_sword.startBlock();
        }
    }
    else if (m_blocking)
    {
        m_waiting_for_unblock = false;
        m_blocking = false;
        m_sword.endBlock();
    }
    else
        m_waiting_for_unblock = false;

    if (states[EventReceiver::KEY_UP])
    {
        if (m_node->getFOV() < MAX_FOV)
            m_node->setFOV(m_node->getFOV() + STEP_FOV);
        position.X += -m_speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += m_speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }
    else
    {
        if (m_node->getFOV() > MIN_FOV)
            m_node->setFOV(m_node->getFOV() - STEP_FOV);
    }
    if (states[EventReceiver::KEY_DOWN])
    {
        position.X += m_speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += -m_speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }

    if (states[EventReceiver::KEY_STRAFE_LEFT])
    {
        position.X += -m_speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += m_speedPosition * sin(rotation.Y * M_PI / 180.0);
    }
    if (states[EventReceiver::KEY_STRAFE_RIGHT])
    {
        position.X += m_speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += -m_speedPosition * sin(rotation.Y * M_PI / 180.0);
    }
    if (states[EventReceiver::KEY_ATTACK])
    {
        m_sword.setAttack();
    }
    if (states[EventReceiver::KEY_DEBUG_SWORD_EFFECT])
    {
        Sword::enchant m_current_enchant = static_cast<Sword::enchant>(rand() % Sword::ARRAY_END);
        m_sword.setEnchantment(m_current_enchant);
    }

    m_node->setPosition(position);
    m_sword.updatePosition();

    if (m_blocking)
        m_stamina -= 2;
    else
        m_stamina += 1;

    if (m_stamina <= 0)
    {
        m_waiting_for_unblock = true;
        m_stamina = 0;
        m_blocking = false;
        m_sword.endBlock();
    }
    if (m_stamina > m_max_stamina)
        m_stamina = m_max_stamina;
}

void Player::checkTeleportation()
{
    ic::vector3df position = m_node->getPosition();
    ic::vector3df teleporters[2] = {ic::vector3df(-900, 140, -260), ic::vector3df(1080, 140, -260)};
    Utils::showPosition("", m_node->getPosition());
    for (size_t i = 0; i < 2; i++)
    {
        float distance = position.getDistanceFrom(teleporters[i]);
        if (distance < 30.0)
        {
            position = teleporters[i == 0 ? 1 : 0];
            position.X += i == 0 ? -100 : 100;
            m_node->setPosition(position);
            m_collision->setTargetNode(m_node);
            m_teleport_alpha = 123;
            break;
        }
    }
}

ic::vector3df Player::getPosition()
{
    return m_node->getPosition();
}

ic::vector3df Player::getRotation()
{
    return m_node->getRotation();
}

Sword Player::getSword()
{
    return m_sword;
}

bool Player::isBlocking()
{
    return m_blocking;
}

void Player::takeDamage(int damage)
{
    m_health -= damage;
    m_hit_alpha = 123;
    if (m_health < 0)
        m_health = 0;
}

int Player::getHealth()
{
    return m_health;
}

int Player::getStamina()
{
    return m_stamina;
}

is::ICameraSceneNode *Player::getNode()
{
    return m_node;
}

int Player::getSouls()
{
    return m_souls;
}

void Player::addSoulsEffect(SoulsEffect souls_effect)
{
    irr::scene::IParticleSystemSceneNode *souls_effect_node = Utils::setParticuleSystem(
        m_device, 0, souls_effect.position, souls_effect.color);
    is::IParticleEmitter *emitter = souls_effect_node->getEmitter();
    emitter->setDirection(ic::vector3df(0, 0, 0));
    emitter->setMaxLifeTime(800);
    emitter->setMaxAngleDegrees(90);
    emitter->setMinStartSize(ic::dimension2df(5, 5));
    emitter->setMaxStartSize(ic::dimension2df(5, 5));
    souls_effect.node = souls_effect_node;
    m_souls_effects.push_back(souls_effect);
}

void Player::addMaxStam(int increment)
{
    m_max_stamina += increment;
}

void Player::updateSoulsEffects()
{

    float speed = 0.1f * m_souls_to_add;
    m_souls_to_add = fabs(speed) > 0.1 ? m_souls_to_add - speed : 0;
    m_souls_to_show = fabs(speed) > 0.1 ? m_souls_to_show + speed : m_souls;

    for (size_t index = 0; index < m_souls_effects.size();)
    {
        float speed_rotation = 5.0f;
        ic::vector3df position_effect = m_souls_effects[index].node->getPosition();
        ic::vector3df rotation_effect = m_souls_effects[index].node->getRotation();
        ic::vector3df position_player = m_node->getPosition();

        ic::vector3df position_diff = position_player - position_effect;
        rotation_effect.Y = atan(position_diff.Z / position_diff.X) * (180.0f / irr::core::PI);
        if ((position_player.X - position_effect.X) > 0)
        {
            rotation_effect.Y = 90 - rotation_effect.Y;
        }
        else if ((position_player.X - position_effect.X) < 0)
        {
            rotation_effect.Y = -90 - rotation_effect.Y;
        }
        rotation_effect.Y -= 90;

        position_effect.X += speed_rotation * cos((rotation_effect.Y) * M_PI / 180.0);
        position_effect.Y += 0.1 * position_diff.Y;
        position_effect.Z -= speed_rotation * sin((rotation_effect.Y) * M_PI / 180.0);

        m_souls_effects[index].node->setRotation(rotation_effect);
        m_souls_effects[index].node->setPosition(position_effect);

        if (position_player.getDistanceFrom(position_effect) < 10)
        {
            m_souls_effects[index].node->remove();
            m_souls_effects[index].node = 0;

            if (m_souls_effects[index].type == ST_HEALTH)
                heal(m_souls_effects[index].value);

            if (m_souls_effects[index].type == ST_MONEY)
            {
                m_souls += m_souls_effects[index].value;
                m_souls_to_add += m_souls_effects[index].value;
            }

            m_souls_effects.erase(m_souls_effects.begin() + index);
        }
        else
            index++;
    }
}

void Player::setSword(Sword sword)
{
    m_sword.switchStats(sword, m_node);
}

void Player::heal(int health)
{
    m_health += health;
    if (m_health > m_max_health)
        m_health = m_max_health;
}

void Player::removeSouls(int cost)
{
    m_souls -= cost;
    m_souls_to_add -= cost;
}

int Player::getSoulsToShow()
{
    return m_souls_to_show;
}

void Player::addBloodScreen()
{
    std::string blood_path = "data/screen/" + std::to_string(rand() % 7) + ".png";

    const int width = m_device->getVideoDriver()->getScreenSize().Width;
    const int height = m_device->getVideoDriver()->getScreenSize().Height;
    irr::gui::IGUIImage *image = m_device->getGUIEnvironment()->addImage(ic::recti(width / 4.0, height / 4.0, width * 3.0 / 4.0, height * 3.0 / 4.0));
    image->setImage(
        m_device->getVideoDriver()->getTexture(std::wstring(blood_path.begin(), blood_path.end()).c_str()));
    image->setColor(iv::SColor(100, 255, 255, 255));
    image->setScaleImage(true);

    m_blood_screens.push_back({100, image});
}

void Player::enemyHitCallback()
{
    addBloodScreen();
}