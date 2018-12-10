#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>
#include <vector>
#include "player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Enemy
{
  int id;
  bool isAllowedToMove(std::vector<Enemy> enemies);
  void updateRotation(Player player);
  void updatePosition(Player player, std::vector<Enemy> enemies);
  void updateDeath();

public:
  Enemy();
  is::IAnimatedMeshSceneNode *node;
  void setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh, int const id);
  void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);
  void update(Player player, std::vector<Enemy> enemies);

  bool isDead = false;
  ic::vector3df whereToDie;
  ic::vector3df lastPosition;

  enum key_id
  {
    IS_ALIVE,
    IS_DYING,
    IS_DEAD,
  };
  bool state = IS_ALIVE;
};

#endif
