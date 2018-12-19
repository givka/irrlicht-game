#include "sun.hpp"

Sun::Sun(irr::IrrlichtDevice *device)
    : m_device(device)
{

    m_sun_color = iv::SColor(255, 255, 255, 0);

    is::ISceneManager *smgr = device->getSceneManager();

    ic::vector3df sun_position = ic::vector3df(145.906, 688.894, -271.34);

    m_sun_particle = Utils::setParticuleSystem(device, 0, sun_position, m_sun_color);
    auto em = m_sun_particle->getEmitter();
    em->setMinStartColor(iv::SColor(255, 255, 255, 255));
    em->setMinStartSize(ic::dimension2df(10, 10));
    em->setMaxStartSize(ic::dimension2df(100, 100));
    em->setMinLifeTime(0);

    em->setMaxLifeTime(500);
    em->setMaxAngleDegrees(360);

    m_sun_light = smgr->addLightSceneNode(m_sun_particle, ic::vector3df(0, 0, 0), m_sun_color, 2000);
}

void Sun::update()
{
    m_sun_light->setRadius(m_sun_light->getRadius() + 1);
}