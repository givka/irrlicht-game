#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "../entity/player.hpp"
#include "../events.hpp"
#include "../item/sword.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

typedef enum UpgradeType
{
    HEALTH,
    STAMINA,
    SWORD
} UpgradeType;

class Upgrade
{
  public:
    Upgrade(irr::IrrlichtDevice *device, UpgradeType type, ic::vector3df position, int increment, int cost, Player &player);
    void update(Player &player, EventReceiver &receiver);
    void reset();

  private:
    irr::IrrlichtDevice *m_device = m_device;
    UpgradeType m_type;
    ic::vector3df m_position;
    int m_increment;
    int m_cost;
    int m_original_cost;
    int m_timer;
    irr::scene::IBillboardSceneNode *m_tootlip_bg;
    std::vector<is::IBillboardTextSceneNode *> m_tooltip_texts;

    irr::scene::IBillboardTextSceneNode *addTooltipText(std::string text, int y);
    void addTooltip(Player &player);
    void changeSword(Player &player);
    is::ISceneNode *addSphere(iv::SColor color);

    Sword m_sword;
    std::string m_name;

    is::ISceneNode *m_node;

    int m_current_val;
    ig::IGUIFont *m_font;
};

#endif