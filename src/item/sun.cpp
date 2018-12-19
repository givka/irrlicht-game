#include "sun.hpp"

Sun::Sun(irr::IrrlichtDevice *device)
    : m_device(device)
{

    m_sun_color = iv::SColor(255, 255, 255, 255);

    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();

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
    m_sky = smgr->addSkyDomeSceneNode(driver->getTexture("data/skydome.jpg"), 16, 8, 0.95f, 2.0f);
}

void Sun::update()
{
    m_sun_light->setRadius(m_sun_light->getRadius() + 1);
    ic::vector3df rotation_sky = m_sky->getRotation();
    rotation_sky.Y += 0.05;
    m_sky->setRotation(rotation_sky);
}