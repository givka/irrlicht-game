#include <irrlicht.h>

#include "sword.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Sword::Sword()
{
}

void Sword::initialise(irr::IrrlichtDevice *device, is::ICameraSceneNode *nodePlayer)
{
    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();

    node = smgr->addMeshSceneNode(smgr->getMesh("data/sword.obj"), nodePlayer);
    node->setMaterialFlag(iv::EMF_LIGHTING, true);
    node->setMaterialTexture(0, driver->getTexture("data/sword_texture.jpg"));
    node->setPosition(core::vector3df(15, -10, 10));
    node->setRotation(core::vector3df(90, -10, 90));
}

void Sword::setAttack()
{
    if (isAttacking)
        return;
    isAttacking = true;
    swordGoingDown = true;
    float X = 0;
    float Y = -100;
    float Z = std::rand() % 2 ? -5 : 5;
    destination = ic::vector3df(X, Y, Z);
}

void Sword::updatePosition()
{
    float speed = 10.0;
    if (!isAttacking)
        return;

    ic::vector3df position = node->getPosition();
    ic::vector3df rotation = node->getRotation();

    position.Z += swordGoingDown ? 1 : -1;

    rotation.Y += swordGoingDown ? -speed : speed;
    rotation.Z += swordGoingDown ? -destination.Z : destination.Z;

    if (rotation.Y < destination.Y)
        swordGoingDown = false;

    if (!swordGoingDown && rotation.Y >= -10)
        isAttacking = false;

    node->setPosition(position);
    node->setRotation(rotation);
}