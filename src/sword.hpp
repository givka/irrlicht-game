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

  public:
    Sword();
    void initialise(irr::IrrlichtDevice *device, is::ICameraSceneNode *nodePlayer);
    void updatePosition();
    bool isAttacking = false;
    void setAttack();

  private:
    bool m_sword_going_down = true;
    ic::vector3df m_destination;
    irr::IrrlichtDevice *m_device;
    is::IMeshSceneNode *m_node;

    enum enchant
    {
        NONE,     //white
        FIRE,     //red
        ICE,      //blue
        VAMPIRIC, //purple
        POISON,   //green
    };

    enchant m_current_enchant = NONE;

    void setEnchantment(enchant new_enchantment);
    is::IParticleSystemSceneNode *setParticuleSystem(enchant new_enchantment, ic::vector3df position);
};

#endif
