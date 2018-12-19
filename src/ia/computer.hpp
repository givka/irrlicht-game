#ifndef COMPUTER_H
#define COMPUTER_H

#define COMPUTER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <irrlicht.h>
#include "computer.hpp"
#include "../entity/enemy.hpp"
#include "../entity/player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Computer
{
  public:
    Computer();
    void addEnemy(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector);
    void addEnemy(Enemy *enemy, ic::vector3df &pos, ic::vector3df &ori, irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector); //todo: obviously needs to be changed
    void update(Player &player);
    bool isWaveFinished();
    int getNumberOfEnemies();

  private:
    std::vector<Enemy> m_enemies;
    int nbrEnemiesAdded;
    void updatePosition(Player &player);
};

#endif
