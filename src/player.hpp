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
    int getStamina();
    is::ICameraSceneNode *getNode();
    int getSouls();
    void addSouls(int souls);

  private:
    float m_speedPosition = 5.0;
    float m_speedRotation = 5.0;
    Sword m_sword;
    is::ICameraSceneNode *m_node;
    bool m_blocking;
    int m_souls = 0;

    int m_health;
    int m_stamina;
    int m_max_stamina;
    int m_max_health;

    bool m_waiting_for_unblock = false;

    float m_health_bar_size;
    is::IBillboardSceneNode *m_health_bar;
    is::IBillboardSceneNode *m_health_bar_bg;
};
#endif
