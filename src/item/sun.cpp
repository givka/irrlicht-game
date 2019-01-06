#include "sun.hpp"

Sun::Sun(irr::IrrlichtDevice *device)
    : m_device(device)
{

    m_sun_color = iv::SColor(255, 255, 255, 0);

    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();

    ic::vector3df sun_position = ic::vector3df(105, 400, -200);

    m_sun_light = smgr->addLightSceneNode(0, sun_position, m_sun_color, 2000);
    m_sun_particle = Utils::setParticuleSystem(device, m_sun_light, ic::vector3df(0, 1000, 0), m_sun_color);
    auto em = m_sun_particle->getEmitter();
    em->setMinStartColor(iv::SColor(255, 255, 255, 255));
    em->setMinStartSize(ic::dimension2df(2, 2));
    em->setMaxStartSize(ic::dimension2df(2, 2));
    em->setMinLifeTime(0);
    em->setMaxParticlesPerSecond(1000);
    em->setMaxLifeTime(100000);
    em->setMaxAngleDegrees(180);
    em->setDirection(ic::vector3df(0, 0, 0.05));

    m_sky = smgr->addSkyDomeSceneNode(driver->getTexture("data/skydome.jpg"), 16, 8, 0.95f, 2.0f);

    // is::ISceneNodeAnimator *anim_cercle = smgr->createFlyCircleAnimator(m_sun_particle->getPosition(), 250, 0.001, ic::vector3df(.0, 0.0f, 1.f), 0);
    // m_sun_particle->addAnimator(anim_cercle);
    // anim_cercle->drop();

    addFires();
}

void Sun::addFires()
{
    ic::vector3df fires[4] = {ic::vector3df(-620, -100, 304),
                              ic::vector3df(-600, -100, -816),
                              ic::vector3df(820, -100, -788),
                              ic::vector3df(800, -100, 300)};

    for (size_t i = 0; i < 4; i++)
    {
        is::IParticleSystemSceneNode *fire = Utils::setParticuleSystem(m_device, 0, fires[i], iv::SColor(255, 255, 50, 0));
        auto em = fire->getEmitter();
        em->setMinStartColor(iv::SColor(255, 0, 0, 255));
        em->setMinStartSize(ic::dimension2df(20, 20));
        em->setMaxStartSize(ic::dimension2df(200, 200));
        em->setMinLifeTime(0);
        em->setMaxLifeTime(1000);
        em->setMaxAngleDegrees(0);
        em->setDirection(ic::vector3df(0, 0.5, 0));
    }
}

void Sun::update()
{
    // strange behaviour, the interpolation works only between 1.0 and 0.99
    if (m_interpolate < 0.99 || m_sun_color.color == m_color_to_go.color)
    {
        m_color_to_go = m_colors[rand() % 8];
        m_interpolate = 1.0;
    }

    ic::vector3df rotation_sky = m_sky->getRotation();
    rotation_sky.Y += 0.05;
    m_sky->setRotation(rotation_sky);

    m_sun_color = m_sun_color.getInterpolated(m_color_to_go, m_interpolate -= 0.00001);
    m_sun_particle->getEmitter()->setMaxStartColor(m_sun_color);
    m_sun_light->getLightData().DiffuseColor = m_sun_color;
}