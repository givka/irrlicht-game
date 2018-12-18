#include <irrlicht.h>

#include "utils.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

namespace Utils
{

void showPosition(const std::string &nodeName, ic::vector3df position)
{
    std::cout << nodeName
              << ": (" << position.X << ", " << position.Y << ", " << position.Z << ")"
              << std::endl;
}
void operator<<(std::ostream & stream, ic::vector3df v)
{
    stream << v.X << " " << v.Y << " " << v.Z << std::endl;
}
is::IParticleSystemSceneNode *setParticuleSystem(irr::IrrlichtDevice *device,
                                                 is::ISceneNode *node,
                                                 ic::vector3df position,
                                                 iv::SColor color)
{
    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();
    scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(false, node);
    scene::IParticleEmitter *em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-1, -1, -1, 1, 1, 1), core::vector3df(0.0f, 0.1f, 0.0f), 200, 200, iv::SColor(255, 0, 0, 0), color, 0, 0, 40, ic::dimension2d<f32>(10.0f, 10.0f), ic::dimension2d<f32>(10.0f, 10.0f));
    ps->setEmitter(em);
    em->drop();
    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("data/vfx/smoke.jpg"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    ps->setPosition(position);
    return ps;
}

float maxComponent(ic::vector3df v)
{
    if(v.X > v.Y && v.X > v.Z)
        return v.X;
    if(v.Y > v.Z)
        return v.Y;
    return v.Z;
}
} // namespace Utils
