#ifndef SWORD_H
#define SWORD_H

#define SWORD_H

#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "../utils.hpp"

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
        FROST,    //blue
        VAMPIRIC, //purple
        POISON,   //green
        ARRAY_END
    };
    Sword();
    Sword(int damage, int crit_percent, enchant ench);
    void initialise(irr::IrrlichtDevice *device, is::ISceneNode *nodePlayer);
    void updatePosition();
    void setAttack();
    void startBlock();
    void endBlock();
    bool getIsAttacking();
    int getSwingNumber();
    float getAttack();
    Sword::enchant getCurrentEnchant();
    iv::SColor getCurrentEnchantColor();
    iv::SColor getEnchantColor(enchant ench);
    void setEnchantment(enchant new_enchantment);
    int getCritPercent();
    int getDamageMin();
    int getDamageMax();
    is::IMeshSceneNode *getNode();

    void switchStats(Sword sword, is::ISceneNode *node);

    std::string getDamageText(Sword sword);
    std::string getCritChanceText(Sword sword);
    std::string getEnchantText(Sword sword);

    void reset();

  private:
    int m_damage_min;
    int m_damage_max;
    int m_crit_percent;
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

    std::string m_enchant_names[ARRAY_END] = {
        "NONE",
        "FIRE",
        "FROST",
        "VAMPIRIC",
        "POISON",
    };

    enchant m_current_enchant;

    std::vector<is::IParticleSystemSceneNode *> m_particles;
};

#endif
