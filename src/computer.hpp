#ifndef COMPUTER_H
#define COMPUTER_H

#define COMPUTER_H

#include <iostream>
#include <vector>
#include <irrlicht.h>
#include "computer.hpp"
#include "enemy.hpp"
#include "player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Computer
{
  std::vector<Enemy> enemies;

  void updatePosition(Player player);
  void updateDeath();
  void checkAttack(Player player, EventReceiver *receiver);

public:
  Computer();
  void addEnemy(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector, int const id);

  void update(Player player, EventReceiver *receiver);
};

#endif
