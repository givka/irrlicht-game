#include <irrlicht.h>
#include <iostream>

#include "events.hpp"
#include "entity/player.hpp"
#include "entity/enemy.hpp"
#include "ia/computer.hpp"
#include "ia/level.hpp"
#include "ia/WaveManager.h"
#include "ui/playerbar.hpp"
#include "ui/score.hpp"
#include "item/loot.hpp"
#include "item/upgrade.hpp"

#define DEBUG_INFO

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const HEIGHT = 800;
int const WIDTH = HEIGHT * 16.0 / 9.0;

int main()
{
    EventReceiver receiver;

    IrrlichtDevice *device = createDevice(iv::EDT_OPENGL,
                                          ic::dimension2d<u32>(WIDTH, HEIGHT),
                                          16, false, false, true, &receiver);

    iv::IVideoDriver *driver = device->getVideoDriver();
    is::ISceneManager *smgr = device->getSceneManager();
    ig::IGUIEnvironment *gui = device->getGUIEnvironment();

    srand(time(NULL));

    Level level;
    level.loadFromJSON("data/level.json", device, driver, smgr);

    // add collision map selector
    is::ITriangleSelector *selector;
    selector = smgr->createOctreeTriangleSelector(level.getMapNode()->getMesh(), level.getMapNode());
    level.getMapNode()->setTriangleSelector(selector);
    level.getMapNode()->setMaterialType(iv::EMT_SOLID);
    level.getMapNode()->setMaterialFlag(iv::EMF_LIGHTING, true);

    Player player;
    player.initialise(device, selector);

    Score souls(device, 0.90, 0.90, 1.0);
    Score waves(device, 0.090, 0.11, 1.5);

    WaveManager waveMgr;
    waveMgr.loadJSON("data/waves.json");

    Loot loot(device, player);
    Upgrade hp_upgrade(device, player, HEALTH, ic::vector3df(0, 10, 100), 10, 50);
    Upgrade stam_upgrade(device, player, STAMINA, ic::vector3df(50, 10, 100), 10, 50);

    // add enemy
    Computer computer;

    // remove collision selector
    selector->drop();

    // infinite loop
    int lastFPS = -1;

    //spawn first wave
    waveMgr.spawnWave(level, 0, computer, device, selector);

    PlayerBar health_bar(device, 0.10, 0.10, 0.25, 0.11, 200, iv::SColorf(0.8, 0.0, 0.0, 0.7));
    PlayerBar stamina_bar(device, 0.10, 0.115, 0.25, 0.125, 200, iv::SColorf(0.0, 0.5, 0.0, 0.7));

    while (device->run())
    {
        //check for end of wave, start next wave //TODO: add score, pause between waves, etc
        if (computer.isWaveFinished())
        {
            if (waveMgr.getCurrentWave() == waveMgr.getLastWaveId()) //last wave finished, game over
            {
                std::cout << "Spawning wave " << waveMgr.getCurrentWave() << std::endl;
                waveMgr.spawnWave(level, waveMgr.getCurrentWave(), computer, device, selector); //but for now we debug
            }
            else
            {
                std::cout << "Spawning wave " << waveMgr.getCurrentWave() + 1 << std::endl;
                waveMgr.spawnWave(level, waveMgr.getCurrentWave() + 1, computer, device, selector); //spawn next wave
            }
        }

        driver->beginScene(true, true, iv::SColor(0, 0, 0, 0));

        player.update(receiver);
        computer.update(player);
        smgr->drawAll();
        health_bar.update(player.getHealth());
        stamina_bar.update(player.getStamina());
        loot.update(player, receiver);
        hp_upgrade.update(player, receiver);
        stam_upgrade.update(player, receiver);
        souls.update(player.getSouls());
        waves.update(waveMgr.getCurrentWave());
        gui->drawAll();

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
            waveMgr.spawnWave(level, 0, computer, device, selector);
        }
    }
    device->drop();

    return 0;
}
