#ifndef SWORD_H
#define SWORD_H

#define SWORD_H

#include <irrlicht.h>
#include <iostream>
#include "utils.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Sword
{
    bool swordGoingDown = true;
    ic::vector3df destination;

public:
    Sword();
    void initialise(irr::IrrlichtDevice *device, is::ICameraSceneNode *nodePlayer);
    is::IMeshSceneNode *node;
    void updatePosition();
    bool isAttacking = false;
    void setAttack();
};

#endif
