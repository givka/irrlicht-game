#ifndef UPGRADE_H
#define UPGRADE_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "../entity/player.hpp"
#include "../events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

typedef enum UpgradeType
{
    HEALTH,
    STAMINA,
    DAMAGE
} UpgradeType;

class Upgrade
{
  public:
    Upgrade(irr::IrrlichtDevice *device, Player &player, UpgradeType, ic::vector3df position, int increment, int cost);
    void update(Player &player, EventReceiver &receiver);

  private:
    int m_cost;
    int m_timer;
    irr::IrrlichtDevice *m_device = m_device;
    irr::scene::IBillboardSceneNode *m_tootlip_bg;
    std::vector<is::IBillboardTextSceneNode *> m_tooltip_texts;

    irr::scene::IBillboardTextSceneNode *addTooltipText(std::string text, int y);
    void addTooltip(Player &player);

    ic::vector3df m_position;
    std::string m_name;

    int m_current_val;
    int m_increment;
    ig::IGUIFont *m_font;
    UpgradeType m_type;
};

#endif