#ifndef LOOT_H
#define LOOT_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "sword.hpp"
#include "player.hpp"
#include "events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Loot
{
  public:
    Loot(irr::IrrlichtDevice *device);
    void update(Player &player, EventReceiver &receiver);

  private:
    Sword m_sword;
    int m_cost;
    irr::IrrlichtDevice *m_device = m_device;
    void switchSword(Player &player);
    irr::scene::IBillboardSceneNode *m_tootlip_bg;
    irr::scene::IBillboardTextSceneNode *addTooltipText(std::string text, int y);
    void addTooltip();
    std::vector<is::IBillboardTextSceneNode *> m_tooltip_texts;
};

#endif