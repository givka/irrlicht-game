#ifndef SWORD_H
#define SWORD_H

#define SWORD_H

#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "utils.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Sword
{

  public:
    enum enchant
    {
        NONE,     //transparent
        FIRE,     //orange
        ICE,      //blue
        VAMPIRIC, //purple
        POISON,   //green
        ARRAY_END
    };
    Sword();
    void initialise(irr::IrrlichtDevice *device, is::ICameraSceneNode *nodePlayer);
    void updatePosition();
    void setAttack();
    bool getIsAttacking();
    Sword::enchant getCurrentEnchant();

  private:
    bool m_is_attacking = false;
    bool m_sword_going_down = true;
    ic::vector3df m_destination;
    irr::IrrlichtDevice *m_device;
    is::IMeshSceneNode *m_node;
    is::ILightSceneNode *m_node_light;

    iv::SColor m_enchant_colors[ARRAY_END] = {
        iv::SColor(255, 0, 0, 0),      //transparent
        iv::SColor(255, 255, 100, 50), //orange
        iv::SColor(255, 50, 50, 255),  //blue
        iv::SColor(255, 255, 50, 255), //purple
        iv::SColor(255, 50, 255, 50),  //green
    };

    enchant m_current_enchant = NONE;

    std::vector<is::IParticleSystemSceneNode *> m_particles;

    void setEnchantment(enchant new_enchantment);
    is::IParticleSystemSceneNode *setParticuleSystem(ic::vector3df position);
};

#endif
