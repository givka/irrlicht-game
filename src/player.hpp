#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_H

#include <irrlicht.h>
#include <iostream>
#include "events.hpp"
#include "utils.hpp"
#include "sword.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Player
{
    float speedPosition = 5.0;
    float speedRotation = 5.0;
    Sword sword;

    void setNode(iv::IVideoDriver *driver);
    void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);

public:
    Player();
    is::ILightSceneNode *nodeLight;
    is::ICameraSceneNode *node;
    void initialise(irr::IrrlichtDevice *device, is::ITriangleSelector *selector);
    void updatePosition(EventReceiver *receiver);
};

#endif
