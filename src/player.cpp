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
    node = smgr->getActiveCamera();
    device->getCursorControl()->setVisible(false);
    node->setFOV(2.0);

    node->setPosition(core::vector3df(100, 30, 100));

    nodeLight = smgr->addLightSceneNode(node, core::vector3df(0, 0, 0),
                                        video::SColorf(1.0f, 0.5f, 0.5f, 0.0f), 300.0f);
    nodeLight->setPosition(ic::vector3df(0, 20, -20));

    sword.initialise(device, node);

    is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(
            selector, node, core::vector3df(30, 30, 30),
            core::vector3df(0, -10, 0), core::vector3df(0, 15, 0));
    node->addAnimator(collision);
    collision->drop();
}

void Player::updatePosition(EventReceiver *receiver)
{
    auto states = receiver->states;
    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    if (states[receiver->KEY_UP])
    {
        position.X += -speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }
    if (states[receiver->KEY_DOWN])
    {
        position.X += speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
        position.Z += -speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
    }

    if (states[receiver->KEY_STRAFE_LEFT])
    {
        position.X += -speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += speedPosition * sin(rotation.Y * M_PI / 180.0);
    }
    if (states[receiver->KEY_STRAFE_RIGHT])
    {
        position.X += speedPosition * cos(rotation.Y * M_PI / 180.0);
        position.Z += -speedPosition * sin(rotation.Y * M_PI / 180.0);
    }

    if (states[receiver->KEY_ATTACK])
    {
        sword.setAttack();
    }

    node->setPosition(position);
    sword.updatePosition();
}