#include "playerbar.hpp"
#include <iostream>

PlayerBar::PlayerBar(irr::IrrlichtDevice *device, int x0, int y0, int x1, int y1, int max, iv::SColorf fg_color)
    : m_device(device), m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1), m_max_val(max), m_fg_color(fg_color)
{
}

void PlayerBar::update(int val)
{
    iv::IVideoDriver *driver = m_device->getVideoDriver();
    int x0 = m_x0 >= 0 ? m_x0 : m_x0 + driver->getScreenSize().Width;
    int y0 = m_y0 >= 0 ? m_y0 : m_y0 + driver->getScreenSize().Height;
    int x1 = m_x1 >= 0 ? m_x1 : m_x1 + driver->getScreenSize().Width;
    int y1 = m_y1 >= 0 ? m_y1 : m_y1 + driver->getScreenSize().Height;

    m_bg_rect = ic::rect<irr::s32>(x0, y0, x1, y1);

    m_percentage = (((float)val) / ((float)m_max_val));

    driver->draw2DRectangle(m_bg_color.toSColor(), m_bg_rect);

    ic::recti fg_rect(m_bg_rect.UpperLeftCorner + ic::vector2di(m_border_size, m_border_size),
                      m_bg_rect.LowerRightCorner - ic::vector2di((irr::s32)(m_border_size + (m_bg_rect.getSize().Width - 2 * m_border_size) * (1 - m_percentage)), m_border_size));

    driver->draw2DRectangle(m_fg_color.toSColor(), fg_rect);
}

void PlayerBar::setMaxVal(int max_val)
{
    m_max_val = max_val;
}

void PlayerBar::setBGRect(ic::recti bg_rect)
{
    m_bg_rect = bg_rect;
}

void PlayerBar::setFGColor(iv::SColorf color)
{
    m_fg_color = color;
}
