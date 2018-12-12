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

} // namespace Utils
