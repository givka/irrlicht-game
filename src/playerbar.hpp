#ifndef PLAYERBAR_H
#define PLAYERBAR_H

#include "irrlicht.h"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class PlayerBar
{
  public:
    PlayerBar(irr::IrrlichtDevice *device, int x0, int y0, int x1, int y1, int max, iv::SColorf fg_color);
    void draw();
    void setMaxVal(int max_val);
    void update(int val);
    void setBGRect(ic::recti bg_rect);
    void setFGColor(iv::SColorf color);

  private:
    int m_x0;
    int m_y0;
    int m_x1;
    int m_y1;
    int m_max_val;
    irr::IrrlichtDevice *m_device;
    float m_percentage;
    ic::recti m_bg_rect;

    const iv::SColorf m_bg_color = iv::SColorf(0.0, 0.0, 0.0, 0.7);
    iv::SColorf m_fg_color = iv::SColorf(0.8, 0.0, 0.0, 0.7);
    const int m_border_size = 2;
};

#endif
