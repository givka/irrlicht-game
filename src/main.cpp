#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "computer.hpp"
#include "level.hpp"

#define DEBUG_INFO

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

  Level level;
  level.loadFromJSON("data/level.json", device, driver, smgr);

  // add collision map selector
  is::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(level.getMapNode()->getMesh(), level.getMapNode());
  level.getMapNode()->setTriangleSelector(selector);
  level.getMapNode()->setMaterialType(iv::EMT_SOLID);
  level.getMapNode()->setMaterialFlag(iv::EMF_LIGHTING, true);
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

    driver->endScene();
  }
  device->drop();

  return 0;
}
