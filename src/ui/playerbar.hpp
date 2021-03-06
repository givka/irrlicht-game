#ifndef PLAYERBAR_H
#define PLAYERBAR_H

#include "irrlicht.h"
#include "../entity/player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class PlayerBar
{
  public:
    PlayerBar(irr::IrrlichtDevice *device, float x0, float y0, float x1, float y1, int max, iv::SColorf fg_color);
    void update(int val, int max_val);

  private:
    float m_x0;
    float m_y0;
    float m_x1;
    float m_y1;
    int m_max_val;
    int m_origin_max_val;
    irr::IrrlichtDevice *m_device;
    float m_percentage;
    ic::recti m_bg_rect;

    const iv::SColorf m_bg_color = iv::SColorf(0.0, 0.0, 0.0, 0.7);
    iv::SColorf m_fg_color = iv::SColorf(0.8, 0.0, 0.0, 0.7);
    const int m_border_size = 2;
};

#endif
