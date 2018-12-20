#include "cursor.hpp"
#include <iostream>

Cursor::Cursor(irr::IrrlichtDevice *device)
    : m_device(device)
{
}

void Cursor::update()
{
    ig::IGUIEnvironment *gui = m_device->getGUIEnvironment();
    iv::IVideoDriver *driver = m_device->getVideoDriver();

    const int width = driver->getScreenSize().Width;
    const int height = driver->getScreenSize().Height;

    const int size = 0.005 * width;

    if (m_image)
        m_image->remove();

    m_image = gui->addImage(ic::recti((width - size) / 2, (height - size) / 2, (width + size) / 2, (height + size) / 2));
    m_image->setImage(driver->getTexture("data/cursor.png"));
    m_image->setScaleImage(true);
    m_image->setColor(iv::SColor(123, 255, 0, 0));
}

void Cursor::setVisible(bool b) {
    m_image->setVisible(b);
}
