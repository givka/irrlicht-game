#ifndef PLAYERBAR_H
#define PLAYERBAR_H

#include "irrlicht.h"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class PlayerBar
{
public:
    PlayerBar(ic::recti bg_rect, int max = 200);
    void draw(iv::IVideoDriver *driver);
    void setMaxVal(int max_val);
    void updateVal(int val);
    void setBGRect(ic::recti bg_rect);
    void setFGColor(iv::SColorf color);
private:
    int m_max_val;
    float m_percentage;
    ic::recti m_bg_rect;

    const iv::SColorf m_bg_color = iv::SColorf(0.0, 0.0, 0.0, 0.7);
    iv::SColorf m_fg_color = iv::SColorf(0.8, 0.0, 0.0, 0.7);
    const int m_border_size = 2;

};


#endif
