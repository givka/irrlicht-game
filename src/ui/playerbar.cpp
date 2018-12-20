#include "playerbar.hpp"
#include <iostream>

PlayerBar::PlayerBar(irr::IrrlichtDevice *device,
                     float x0, float y0, float x1, float y1,
                     int max,
                     iv::SColorf fg_color)
    : m_device(device), m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1), m_max_val(max), m_fg_color(fg_color), m_origin_max_val(max)
{
}

void PlayerBar::update(int val, int max_val)
{
    m_max_val = max_val;
    float size = ((float)m_max_val) / ((float)m_origin_max_val);

    iv::IVideoDriver *driver = m_device->getVideoDriver();
    int x0 = m_x0 * driver->getScreenSize().Width;
    int y0 = m_y0 * driver->getScreenSize().Height;
    int x1 = m_x1 * size * driver->getScreenSize().Width;
    int y1 = m_y1 * driver->getScreenSize().Height;

    if (x0 + x1 > driver->getScreenSize().Width)
        x1 = driver->getScreenSize().Width - x0;

    m_percentage = (((float)val) / ((float)m_max_val));

    m_bg_rect = ic::rect<irr::s32>(x0, y0, x1, y1);

    driver->draw2DRectangle(m_bg_color.toSColor(), m_bg_rect);

    ic::recti fg_rect(m_bg_rect.UpperLeftCorner + ic::vector2di(m_border_size, m_border_size),
                      m_bg_rect.LowerRightCorner - ic::vector2di((irr::s32)(m_border_size + (m_bg_rect.getSize().Width - 2 * m_border_size) * (1 - m_percentage)), m_border_size));

    driver->draw2DRectangle(m_fg_color.toSColor(), fg_rect);
}
