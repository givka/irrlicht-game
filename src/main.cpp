#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const NBR_ENEMIES = 100;
int const WIDTH = 1400;
int const HEIGHT = 900;

int main()
{
  EventReceiver receiver;

  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(WIDTH, HEIGHT),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  // add map
  device->getFileSystem()->addFileArchive("data/map-20kdm2.pk3");

  is::IAnimatedMesh *meshMap = smgr->getMesh("20kdm2.bsp");
  is::IMeshSceneNode *nodeMap = smgr->addOctreeSceneNode(meshMap->getMesh(0));
  nodeMap->setPosition(core::vector3df(-1300, -104, -1249));
  nodeMap->setMaterialType(iv::EMT_SOLID);
  nodeMap->setMaterialFlag(iv::EMF_LIGHTING, true);

  // add collision map selector
  is::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(nodeMap->getMesh(), nodeMap);
  nodeMap->setTriangleSelector(selector);

  is::IAnimatedMesh *meshSkeleton = smgr->getMesh("data/tris.md2");

  Player player;
  player.initialise(device, selector);

  // add enemy
  Computer computer;

  for (size_t i = 0; i < NBR_ENEMIES; i++)
  {
    computer.addEnemy(driver, smgr, meshSkeleton, selector);
  }

  // remove collision selector
  selector->drop();

  // infinite loop
  int lastFPS = -1;
  while (device->run())
  {
    driver->beginScene(true, true, iv::SColor(0, 0, 0, 0));

    player.updatePosition(&receiver);
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
