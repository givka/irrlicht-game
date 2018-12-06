#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Enemy
{
  void updateRotation(is::IAnimatedMeshSceneNode *player);

public:
  Enemy();
  is::IAnimatedMeshSceneNode *node;
  void setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh);
  void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);
  void updatePosition(is::IAnimatedMeshSceneNode *player);
};

#endif
