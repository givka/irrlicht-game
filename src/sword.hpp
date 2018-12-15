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
    void startBlock();
    void endBlock();
    bool getIsAttacking();
    int getSwingNumber();
    float getAttack();
    Sword::enchant getCurrentEnchant();
    iv::SColor getCurrentEnchantColor(int alpha = 255);
    void setEnchantment(enchant new_enchantment);

  private:
    int m_damage_min;
    int m_damage_max;
    bool m_is_attacking = false;
    bool m_is_blocking = false;
    bool m_sword_going_down = true;
    int m_swing_number = 0;
    ic::vector3df m_destination;
    irr::IrrlichtDevice *m_device;
    is::IMeshSceneNode *m_node;
    is::ILightSceneNode *m_node_light;

    iv::SColor m_enchant_colors[ARRAY_END] = {
        iv::SColor(255, 0, 0, 0),       //transparent
        iv::SColor(255, 255, 123, 44),  //orange
        iv::SColor(255, 123, 123, 255), //blue
        iv::SColor(255, 255, 85, 255),  //purple
        iv::SColor(255, 85, 255, 85),   //green
    };

    enchant m_current_enchant = NONE;

    std::vector<is::IParticleSystemSceneNode *> m_particles;
};

#endif
