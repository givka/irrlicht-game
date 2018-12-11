#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "camera.hpp"
#include "computer.hpp"
#include "level.hpp"

#define DEBUG_INFO

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const NBR_ENEMIES = 20;

int main()
{
  EventReceiver receiver;

  IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                        ic::dimension2d<u32>(1366, 768),
                                        16, false, false, false, &receiver);

  iv::IVideoDriver *driver = device->getVideoDriver();
  is::ISceneManager *smgr = device->getSceneManager();

  Level level;
  level.loadFromJSON("data/level.json", device, driver, smgr);

  // add collision map selector
  is::ITriangleSelector *selector;
  selector = smgr->createOctreeTriangleSelector(level.getMapNode()->getMesh(), level.getMapNode());
  level.getMapNode()->setTriangleSelector(selector);

  is::IAnimatedMesh *meshSkeleton = smgr->getMesh("data/tris.md2");

  Player player;
  player.setNode(driver, smgr, meshSkeleton);
  player.addCollisionMap(smgr, selector);

  // add enemy
  Computer computer;

  for (size_t i = 0; i < NBR_ENEMIES; i++)
  {
    computer.addEnemy(driver, smgr, meshSkeleton, selector);
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

    #ifdef DEBUG_INFO
      auto node = player.getNode();
      auto pos = node->getPosition();
      std::cout << pos.X << " " << pos.Y << " " << pos.Z << std::endl;
    #endif

    driver->endScene();
  }
  device->drop();

  return 0;
}
