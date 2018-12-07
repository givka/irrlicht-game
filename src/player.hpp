#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_H

#include <irrlicht.h>
#include "events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Player
{
  float speedPosition = 5.0;
  float speedRotation = 5.0;

public:
  Player();
  is::IAnimatedMeshSceneNode *node;
  void setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh);
  void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);
  void updatePosition(EventReceiver *receiver);
};

#endif
