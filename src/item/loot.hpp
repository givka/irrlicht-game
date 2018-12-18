#ifndef LOOT_H
#define LOOT_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "sword.hpp"
#include "../entity/player.hpp"
#include "../events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Loot
{
  public:
    Loot(irr::IrrlichtDevice *device, Player &player);
    void update(Player &player, EventReceiver &receiver);

  private:
    Sword m_sword;
    int m_cost;
    int m_timer;
    irr::IrrlichtDevice *m_device = m_device;
    irr::scene::IBillboardSceneNode *m_tootlip_bg;
    std::vector<is::IBillboardTextSceneNode *> m_tooltip_texts;

    void switchSword(Player &player);
    irr::scene::IBillboardTextSceneNode *addTooltipText(std::string text, int y);
    ig::IGUIFont *m_font;
    void addTooltip(Player &player);
};

#endif