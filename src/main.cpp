#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "computer.hpp"
#include "level.hpp"
#include "WaveManager.h"

#define DEBUG_INFO

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const NBR_ENEMIES = 40;
int const WIDTH = 1280;
int const HEIGHT = 800;

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

    WaveManager waveMgr;
    waveMgr.loadJSON("data/waves.json");

    // add enemy
    Computer computer;
    for (size_t i = 0; i < NBR_ENEMIES; i++)
    {
        //computer.addEnemy(driver, smgr, meshSkeleton, selector);
    }

    // remove collision selector
    selector->drop();

    // infinite loop
    int lastFPS = -1;

    //spawn first wave
    waveMgr.spawnWave(level, 0, computer, device, meshSkeleton, selector);

    while (device->run())
    {
        //check for end of wave, start next wave //TODO: add score, pause between waves, etc
        if (computer.isWaveFinished())
        {
            if (waveMgr.getCurrentWave() == waveMgr.getLastWaveId()) //last wave finished, game over
            {
                std::cout << "Spawning wave " << waveMgr.getCurrentWave() << std::endl;
                waveMgr.spawnWave(level, waveMgr.getCurrentWave(), computer, device, meshSkeleton, selector); //but for now we debug
            }
            else
            {
                std::cout << "Spawning wave " << waveMgr.getCurrentWave() + 1 << std::endl;
                waveMgr.spawnWave(level, waveMgr.getCurrentWave() + 1, computer, device, meshSkeleton, selector); //spawn next wave
            }
        }

        driver->beginScene(true, true, iv::SColor(0, 0, 0, 0));

        player.updatePosition(receiver);
        computer.update(player);

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

        if (receiver.getStates()[EventReceiver::KEY_DEBUG_TRIGGER_SPAWN])
        {
            std::cout << "spawning wave 0" << std::endl;
            waveMgr.spawnWave(level, 0, computer, device, meshSkeleton, selector);
        }
    }
    device->drop();

    return 0;
}
