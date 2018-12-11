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

} // namespace Utils

#endif
