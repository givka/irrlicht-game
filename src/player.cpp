#include <irrlicht.h>

#include "player.hpp"
#include "events.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Player::Player()
{
}

void Player::initialise(irr::IrrlichtDevice *device, is::ITriangleSelector *selector)
{
    is::ISceneManager *smgr = device->getSceneManager();

    smgr->addCameraSceneNodeFPS();
    m_node = smgr->getActiveCamera();
    device->getCursorControl()->setVisible(false);
    m_node->setFOV(2.0);

    m_node->setPosition(core::vector3df(100, 30, 100));

    m_nodeLight = smgr->addLightSceneNode(m_node, core::vector3df(0, 0, 0),
                                          video::SColorf(1.0f, 0.5f, 0.5f, 0.0f), 300.0f);
    m_nodeLight->setPosition(ic::vector3df(0, 20, -20));

    m_sword.initialise(device, m_node);

    is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(
        selector, m_node, core::vector3df(30, 30, 30),
        core::vector3df(0, -10, 0), core::vector3df(0, 15, 0));
    m_node->addAnimator(collision);
    collision->drop();
}

void Player::updatePosition(EventReceiver *receiver)
{
    auto states = receiver->states;
    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if (states[receiver->KEY_UP])
    {
        position.X += -m_speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += m_speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }
    if (states[receiver->KEY_DOWN])
    {
        position.X += m_speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += -m_speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }

    if (states[receiver->KEY_STRAFE_LEFT])
    {
        position.X += -m_speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += m_speedPosition * sin(rotation.Y * M_PI / 180.0);
    }
    if (states[receiver->KEY_STRAFE_RIGHT])
    {
        position.X += m_speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += -m_speedPosition * sin(rotation.Y * M_PI / 180.0);
    }

    if (states[receiver->KEY_ATTACK])
    {
        m_sword.setAttack();
    }

    m_node->setPosition(position);
    m_sword.updatePosition();
}

ic::vector3df Player::getPosition() {
    return m_node->getPosition();
}

ic::vector3df Player::getRotation() {
    return m_node->getRotation();
}
