#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "camera.hpp"
#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const NBR_ENEMIES = 200;

int main()
{
  EventReceiver receiver;

  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1366, 768),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // add map
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");
  is::IAnimatedMesh *meshMap = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *nodeMap = smgr->addOctreeSceneNode(meshMap->getMesh(0));
  nodeMap->setPosition(core::vector3df(-1300, -104, -1249));

  // add collision map selector
  is::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
  nodeMap->setTriangleSelector(selector);

  is::IAnimatedMesh *meshSkeleton = smgr->getMesh("data/tris.md2");

  Player player;
  player.setNode(driver, smgr, meshSkeleton);
  player.addCollisionMap(smgr, selector);

  // add enemy
  Computer computer;

  for (int i = 0; i < NBR_ENEMIES; i++)
  {
    computer.addEnemy(driver, smgr, meshSkeleton, selector, i);
  }

  // remove collision selector
  selector->drop();

  // add camera
  Camera camera;
  camera.setNode(device, smgr);

  // add events
  receiver.setNode(player.node);

  // infinite loop
  int lastFPS = -1;
  while (device->run())
  {
    driver->beginScene(true, true, iv::SColor(100, 150, 200, 255));

    player.updatePosition(&receiver);
    camera.updatePosition(player);
    computer.update(player, &receiver);

    smgr->drawAll();
    driver->endScene();

    int fps = driver->getFPS();
    if (lastFPS != fps)
    {
      core::stringw str = L"BARRIERE - BOUCAUD - Irrlicht Engine [";
      str += driver->getName();
      str += "] FPS:";
      str += fps;

      device->setWindowCaption(str.c_str());
      lastFPS = fps;
    }
  }
  device->drop();

  return 0;
}
