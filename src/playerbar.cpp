#include "playerbar.hpp"
#include <iostream>


PlayerBar::PlayerBar(ic::recti bg_rect, int max) : m_bg_rect(bg_rect), m_max_val(max) { }

void PlayerBar::draw(iv::IVideoDriver *driver)
{
    driver->draw2DRectangle(m_bg_color.toSColor(), m_bg_rect);
    ic::recti fg_rect (m_bg_rect.UpperLeftCorner
                       + ic::vector2di(m_border_size, m_border_size),
                       m_bg_rect.LowerRightCorner
                       - ic::vector2di((irr::s32) (m_border_size + (m_bg_rect.getSize().Width - 2 * m_border_size) * (1-m_percentage)), m_border_size) );
    driver->draw2DRectangle(m_fg_color.toSColor(), fg_rect);
}

void PlayerBar::setMaxVal(int max_val)
{
    m_max_val = max_val;
}

void PlayerBar::updateVal(int val)
{
    m_percentage = (((float) val) / ((float) m_max_val));
}

void PlayerBar::setBGRect(ic::recti bg_rect)
{
    m_bg_rect = bg_rect;
}

void PlayerBar::setFGColor(iv::SColorf color)
{
    m_fg_color = color;
}
