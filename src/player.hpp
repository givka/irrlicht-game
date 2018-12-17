#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_H

#include <irrlicht.h>
#include <iostream>
#include <vector>
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
    ic::vector3df getPosition();
    ic::vector3df getRotation();
    void update(EventReceiver &receiver);
    Sword getSword();
    bool isBlocking();
    void takeDamage(int damage);
    void heal(int health);
    int getHealth();
    int getStamina();
    is::ICameraSceneNode *getNode();
    int getSouls();
    void addSouls(int souls);
    void addSoulsEffect(ic::vector3df enemy_position, iv::SColor color);
    void setSword(Sword sword);

  private:
    const float MAX_FOV = 2.0;
    const float MIN_FOV = 1.9;
    const float STEP_FOV = 0.01;
    float m_speedPosition = 5.0;
    float m_speedRotation = 5.0;
    Sword m_sword;
    is::ICameraSceneNode *m_node;
    bool m_blocking;
    float m_souls = 0;
    float m_souls_to_add = 0;

    int m_health;
    int m_stamina;
    int m_max_stamina;
    int m_max_health;

    bool m_waiting_for_unblock = false;

    irr::IrrlichtDevice *m_device;
    std::vector<is::IParticleSystemSceneNode *> m_souls_effects;
    void updatePosition(EventReceiver &receiver);
    void updateSoulsEffects();
};
#endif
