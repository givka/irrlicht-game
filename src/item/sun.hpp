#ifndef SUN_H
#define SUN_H

#include <string>
#include <vector>
#include "irrlicht.h"
#include "sword.hpp"
#include "../entity/player.hpp"
#include "../events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Sun
{
  public:
    Sun(irr::IrrlichtDevice *device);
    void update();

  private:
    irr::IrrlichtDevice *m_device = m_device;
    irr::scene::IParticleSystemSceneNode *m_sun_particle;
    irr::scene::ILightSceneNode *m_sun_light;
    iv::SColor m_sun_color;
    is::ISceneNode *m_sky;

    void addFires();

    float m_interpolate = 1.0;
    iv::SColor m_color_to_go;

    // maybe just randomize m_color_to_go ?
    iv::SColor m_colors[8] = {
        iv::SColor(255, 255, 255, 255),
        iv::SColor(255, 255, 255, 0),
        iv::SColor(255, 255, 0, 255),
        iv::SColor(255, 255, 0, 0),
        iv::SColor(255, 0, 255, 255),
        iv::SColor(255, 0, 255, 0),
        iv::SColor(255, 0, 0, 255),
        iv::SColor(255, 0, 0, 0),
    };
};

#endif