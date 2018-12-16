#ifndef LOOT_H
#define LOOT_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "sword.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Loot
{
  public:
    Loot(irr::IrrlichtDevice *device);
    void update();

  private:
    Sword m_sword;
    int m_cost;
    irr::IrrlichtDevice *m_device = m_device;
};

#endif