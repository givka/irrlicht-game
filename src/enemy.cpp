#include <irrlicht.h>

#include "enemy.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Enemy::Enemy()
{
}

void Enemy::setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh)
{
  float const scale = ((float)((rand() % 200) + 100)) / 100.0f;
  float const X = rand() % 200;
  float const Z = rand() % 200;
  node = smgr->addAnimatedMeshSceneNode(mesh);
  node->setMaterialFlag(iv::EMF_LIGHTING, false);
  node->setMD2Animation(is::EMAT_RUN);
  node->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
  node->setPosition(core::vector3df(X, 0, Z));
  node->setScale(core::vector3df(scale, scale, scale));
}

void Enemy::addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector)
{
  ic::vector3df boxMaxEdge = node->getTransformedBoundingBox().MaxEdge;
  ic::vector3df boxCenter = node->getTransformedBoundingBox().getCenter();
  ic::vector3df ellipseRadius = boxMaxEdge - boxCenter;

  is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(selector, node, ellipseRadius);
  node->addAnimator(collision);
  collision->drop();
}

void Enemy::updatePosition(is::IAnimatedMeshSceneNode *player)
{
  core::vector3df position_player = player->getPosition();
  core::vector3df position_enemy = node->getPosition();
  float speed_enemy = 0.01f;

  if (fabs(position_enemy.X - position_player.X) > 20)
    position_enemy.X += speed_enemy * (position_player.X - position_enemy.X);

  if (fabs(position_enemy.Z - position_player.Z) > 20)
    position_enemy.Z += speed_enemy * (position_player.Z - position_enemy.Z);

  node->setPosition(position_enemy);

  updateRotation(player);
}

// Enemy will always face player
void Enemy::updateRotation(is::IAnimatedMeshSceneNode *player)
{
  core::vector3df position_player = player->getPosition();
  core::vector3df position_enemy = node->getPosition();
  core::vector3df rotation_enemy = node->getRotation();

  core::vector3df position_diff = position_player - position_enemy;
  rotation_enemy.Y = atan(position_diff.Z / position_diff.X) * (180.0f / irr::core::PI);
  if ((position_player.X - position_enemy.X) > 0)
  {
    rotation_enemy.Y = 90 - rotation_enemy.Y;
  }
  else if ((position_player.X - position_enemy.X) < 0)
  {
    rotation_enemy.Y = -90 - rotation_enemy.Y;
  }
  rotation_enemy.Y -= 90;
  node->setRotation(rotation_enemy);
}
