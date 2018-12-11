#ifndef COMPUTER_H
#define COMPUTER_H

#define COMPUTER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <irrlicht.h>
#include "computer.hpp"
#include "enemy.hpp"
#include "player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Computer
{
public:
    Computer();
    void addEnemy(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector);
    void addEnemy(Enemy *enemy, ic::vector3df &pos, ic::vector3df &ori, iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector); //todo: obviously needs to be changed
    void update(Player player, EventReceiver *receiver);

private:
    std::vector<Enemy> m_enemies;

    void updatePosition(Player player);
    void checkAttack(Player player, EventReceiver *receiver);
    int nbrEnemiesAdded = 0;
};

#endif
