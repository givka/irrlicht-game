#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_H

#include <irrlicht.h>
#include "player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Camera
{
  float cam_offset = 150.0;

public:
  Camera();
  is::ICameraSceneNode *node;
  void setNode(irr::IrrlichtDevice *device, is::ISceneManager *smgr);
  void updatePosition(Player player);
};

#endif
