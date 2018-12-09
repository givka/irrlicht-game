#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>
#include <vector>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Enemy
{
  int id;
  void updateRotation(is::IAnimatedMeshSceneNode *player);
  bool isAllowedToMove(std::vector<Enemy> enemies);
  void updateDeath();

public:
  Enemy();
  is::IAnimatedMeshSceneNode *node;
  void setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh, int const id);
  void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);
  void updatePosition(is::IAnimatedMeshSceneNode *player, std::vector<Enemy> enemies);
  bool isDead = false;
  ic::vector3df whereToDie;
  ic::vector3df lastPosition;
};

#endif
