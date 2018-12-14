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

class Player {
public:
    Player();

    void initialise(irr::IrrlichtDevice *device, is::ITriangleSelector *selector);

    void updatePosition(EventReceiver &receiver);

    ic::vector3df getPosition();

    ic::vector3df getRotation();

    Sword getSword();

    bool isBlocking();

    void takeDamage(int damage);

    int getHealth();

private:
    float m_speedPosition = 5.0;
    float m_speedRotation = 5.0;
    Sword m_sword;
    is::ICameraSceneNode *m_node;
    bool m_blocking;

    int m_health;
};
#endif
