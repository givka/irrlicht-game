#ifndef UTILS_H
#define UTILS_H

#define UTILS_H

#include <irrlicht.h>
#include <iostream>
#include <string>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

namespace Utils
{

void showPosition(const std::string &nodeName, ic::vector3df position);
is::IParticleSystemSceneNode *setParticuleSystem(irr::IrrlichtDevice *device, is::ISceneNode *node, ic::vector3df position, iv::SColor color = iv::SColor(255, 0, 0, 0));

} // namespace Utils

#endif
