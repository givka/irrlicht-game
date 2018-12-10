#include <irrlicht.h>

#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Computer::Computer()
{
}

void Computer::addEnemy(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector)
{
  Enemy enemy;
  enemy.setNode(driver, smgr, meshSkeleton, nbrEnemiesAdded++);
  enemy.addCollisionMap(smgr, selector);
  enemies.push_back(enemy);
}

void Computer::update(Player player, EventReceiver *receiver)
{
  checkAttack(player, receiver);
  updatePosition(player);
}

void Computer::checkAttack(Player player, EventReceiver *receiver)
{
  if (receiver->states[receiver->KEY_ATTACK] == false)
    return;

  for (size_t index = 0; index < enemies.size(); index++)
  {

    ic::vector3df positionPlayer = player.node->getPosition();
    ic::vector3df positionEnemy = enemies[index].node->getPosition();
    float const distance = positionEnemy.getDistanceFrom(positionPlayer);

    if (distance < 50.0)
    {
      enemies[index].whereToDie = positionEnemy - positionPlayer;
      enemies[index].state = Enemy::IS_DYING;
    }
  }
}

void Computer::updatePosition(Player player)
{
  for (size_t index = 0; index < enemies.size();)
  {
    enemies[index].update(player, enemies);
    if (enemies[index].isDead)
    {
      enemies.erase(enemies.begin() + index);
      std::cout << "enemies remaining: " << enemies.size() << std::endl;
    }
    else
      index++;
  }
}