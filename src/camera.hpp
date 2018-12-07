#ifndef CAMERA_H
#define CAMERA_H

#define CAMERA_H

#include <irrlicht.h>

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
  void updatePosition(is::IAnimatedMeshSceneNode *player);
};

#endif
