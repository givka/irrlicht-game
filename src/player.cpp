#include <irrlicht.h>

#include "player.hpp"
#include "events.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Player::Player()
{
}

void Player::setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh)
{
  node = smgr->addAnimatedMeshSceneNode(mesh);
  node->setMaterialFlag(iv::EMF_LIGHTING, false);
  node->setMD2Animation(is::EMAT_STAND);
  node->setMaterialTexture(0, driver->getTexture("data/blue_texture.pcx"));
}

void Player::addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector)
{
  ic::vector3df boxMaxEdge = node->getTransformedBoundingBox().MaxEdge;
  ic::vector3df boxCenter = node->getTransformedBoundingBox().getCenter();
  ic::vector3df ellipseRadius = boxMaxEdge - boxCenter;

  is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(selector, node, ellipseRadius);
  node->addAnimator(collision);
  collision->drop();
}

void Player::updatePosition(EventReceiver *receiver)
{
  updateAnimation(receiver);

  auto states = receiver->states;
  ic::vector3df position = node->getPosition();
  ic::vector3df rotation = node->getRotation();

  if (states[receiver->KEY_UP])
  {
    position.X += speedPosition * cos(rotation.Y * M_PI / 180.0);
    position.Z += -speedPosition * sin(rotation.Y * M_PI / 180.0);
  }
  if (states[receiver->KEY_DOWN])
  {
    position.X += -0.5 * speedPosition * cos(rotation.Y * M_PI / 180.0);
    position.Z += 0.5 * speedPosition * sin(rotation.Y * M_PI / 180.0);
  }

  if (states[receiver->KEY_STRAFE_LEFT])
  {
    position.X += -speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
    position.Z += speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
  }
  if (states[receiver->KEY_STRAFE_RIGHT])
  {
    position.X += speedPosition * cos((90 + rotation.Y) * M_PI / 180.0);
    position.Z += -speedPosition * sin((90 + rotation.Y) * M_PI / 180.0);
  }
  if (states[receiver->KEY_ROT_RIGHT])
  {
    rotation.Y += speedRotation;
  }
  if (states[receiver->KEY_ROT_LEFT])
  {
    rotation.Y -= speedRotation;
  }
  node->setPosition(position);
  node->setRotation(rotation);
}

void Player::updateAnimation(EventReceiver *receiver)
{
  auto states = receiver->states;
  bool attackJustFinished = false;

  if (states[receiver->KEY_ATTACK])
  {
    if (states[receiver->STATE_ATTACKING] == false)
    {
      states[receiver->STATE_ATTACKING] = true;
      node->setMD2Animation(is::EMAT_ATTACK);
    }
  }

  // When attack animation is done
  if (states[receiver->STATE_ATTACKING] && (int)(node->getEndFrame() - node->getFrameNr()) == 0)
  {
    states[receiver->STATE_ATTACKING] = false;
    attackJustFinished = true;
  }

  if (states[receiver->STATE_ATTACKING] == false)
  {
    if (states[receiver->KEY_UP] || states[receiver->KEY_DOWN] || states[receiver->KEY_STRAFE_LEFT] || states[receiver->KEY_STRAFE_RIGHT])
    {
      if (states[receiver->STATE_MOVING] == false || attackJustFinished)
      {
        states[receiver->STATE_MOVING] = true;
        node->setMD2Animation(is::EMAT_RUN);
      }
    }
    else if (states[receiver->STATE_MOVING] || attackJustFinished)
    {
      node->setMD2Animation(is::EMAT_STAND);
      states[receiver->STATE_MOVING] = false;
    }
  }
}

is::IAnimatedMeshSceneNode *Player::getNode()
{
  return node;
}