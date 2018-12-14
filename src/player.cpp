#include <irrlicht.h>

#include "player.hpp"
#include "events.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Player::Player()
{
    m_health = 200;
    std::cout << "constr " << std::endl;
}

void Player::initialise(irr::IrrlichtDevice *device, is::ITriangleSelector *selector)
{
    is::ISceneManager *smgr = device->getSceneManager();

    smgr->addCameraSceneNodeFPS();
    m_node = smgr->getActiveCamera();
    device->getCursorControl()->setVisible(false);
    m_node->setFOV(2.0);

    m_node->setPosition(core::vector3df(100, 30, 100));

    m_sword.initialise(device, m_node);

    is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(
        selector, m_node, core::vector3df(30, 30, 30),
        core::vector3df(0, -10, 0), core::vector3df(0, 15, 0));
    m_node->addAnimator(collision);
    collision->drop();
}

void Player::updatePosition(EventReceiver &receiver)
{
    bool *states = receiver.getStates();
    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if(states[EventReceiver::KEY_BLOCK])
    {
        if(!m_blocking && !m_sword.getIsAttacking())
        {
            m_blocking = true;
            m_sword.startBlock();
        }

    }
    else if(m_blocking)
    {
        m_blocking = false;
        m_sword.endBlock();
    }

    if (states[EventReceiver::KEY_UP])
    {
        position.X += -m_speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += m_speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
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

bool Player::isBlocking() {
    return m_blocking;
}

void Player::takeDamage(int damage) {
    m_health -= damage;
    if(m_health < 0)
        m_health = 0;
    std::cout << damage << " Player health " << m_health << std::endl;
}
