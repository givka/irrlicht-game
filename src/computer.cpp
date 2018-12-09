#include <irrlicht.h>

#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Computer::Computer()
{
}

void Computer::addEnemy(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector, int const id)
{
  Enemy enemy;
  enemy.setNode(driver, smgr, meshSkeleton, id);
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

  for (int index = 0; index < enemies.size(); index++)
  {

    Enemy *enemy = &enemies[index];

    ic::vector3df positionPlayer = player.node->getPosition();
    ic::vector3df positionEnemy = enemy->node->getPosition();
    float const distance = positionEnemy.getDistanceFrom(positionPlayer);

    if (distance < 50.0)
    {
      enemy->whereToDie = positionEnemy - positionPlayer;
      enemy->isDead = true;
    }
  }
}

void Computer::updatePosition(Player player)
{
  for (int i = 0; i < enemies.size(); i++)
  {
    enemies[i].updatePosition(player.node, enemies);

    //draw line

    // is::ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();

    // ic::line3d<f32> ray;

    // ray.start = enemies[0].node->getPosition();
    // ray.end = ray.start + (player.node->getPosition() - ray.start).normalize() * 1000.0f;

    // // driver->draw3DLine(ray.start, ray.end);

    // // Tracks the current intersection point with the level or a mesh
    // ic::vector3df intersection;
    // // Used to show with triangle has been hit
    // ic::triangle3df hitTriangle;

    // is::ISceneNode *selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay(
    //     ray,
    //     intersection,
    //     hitTriangle);

    // // cout << fabs(intersection.getDistanceFrom(enemies[i].node->getPosition())) << endl;

    // iv::SMaterial m;
    // m.Lighting = false;
    // driver->setMaterial(m);
    // driver->setTransform(video::ETS_WORLD, core::matrix4());

    // driver->draw3DLine(enemies[i].node->getPosition(), intersection);
  }
}