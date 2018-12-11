#include <irrlicht.h>
#include "camera.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Camera::Camera()
{
}

void Camera::setNode(irr::IrrlichtDevice *device, is::ISceneManager *smgr)
{
  smgr->addCameraSceneNode(nullptr, ic::vector3df(0, 30, -70), ic::vector3df(0, 0, 0));
  node = device->getSceneManager()->getActiveCamera();
  node->setFOV(1.5);
}

void Camera::updatePosition(Player player)
{

  ic::vector3df cam_position = node->getPosition();
  ic::vector3df node_position = player.node->getPosition();
  ic::vector3df node_rotation = player.node->getRotation();

  float cam_rot_y = 180 - node_rotation.Y;

  ic::vector3df target_position = node_position;

  cam_position = target_position;

  cam_position.Y += 100.0;
  cam_position.X += cam_offset * cos(cam_rot_y * M_PI / 180.0);
  cam_position.Z += cam_offset * sin(cam_rot_y * M_PI / 180.0);

  node->setPosition(cam_position);
  node->setTarget(target_position);
}