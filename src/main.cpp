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
#include "item/sun.hpp"
#include "ia/waveGenerator.hpp"
#include "ui/cursor.hpp"

#define DEBUG_INFO

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

using namespace std;

int const HEIGHT = 720;
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

    Sun sun(device);

    WaveManager waveMgr;
    WaveGenerator waveGen;
    waveMgr.loadJSON("data/waves.json");

    Loot loot(device, player);
    Upgrade hp_upgrade(device, player, HEALTH, ic::vector3df(352, 125, -86), 10, 50);
    Upgrade stam_upgrade(device, player, STAMINA, ic::vector3df(-119, 125, -141), 10, 50);

    // add enemy
    Computer computer;

    // remove collision selector
    selector->drop();

    // infinite loop
    int lastFPS = -1;

    //spawn first wave
    PlayerBar health_bar(device, 0.10, 0.10, 0.25, 0.11, 200, iv::SColorf(0.8, 0.0, 0.0, 0.7));
    PlayerBar stamina_bar(device, 0.10, 0.115, 0.25, 0.125, 200, iv::SColorf(0.0, 0.5, 0.0, 0.7));
    Cursor cursor(device);

    gui->getSkin()->setFont(gui->getFont("data/myfont.xml"));

    std::wstring enemy_count_string = L"Enemies: " + std::to_wstring(0);

    auto enemy_count_text = gui->addStaticText(L"Enemies : ", ic::rect<irr::s32>(10, HEIGHT - 60, 500, HEIGHT));
    enemy_count_text->setOverrideColor(iv::SColor(255, 255, 255, 255));

    int current_enemy_count = 0;
    int wave_end_time = -1;

    bool flag = false;
    while (device->run())
    {
        if(current_enemy_count != computer.getNumberOfEnemies())
        {
            current_enemy_count = computer.getNumberOfEnemies();
            enemy_count_string = L"Enemies: " + std::to_wstring(current_enemy_count);
            enemy_count_text->setText(enemy_count_string.c_str());
        }
        else if(wave_end_time >= 0)
            enemy_count_text->setText(L"Next wave incoming");

        //check for end of wave, start next wave //TODO: add score, pause between waves, etc
        if (computer.isWaveFinished())
        {
            std::cout << "wave finished" << std::endl;
                if(wave_end_time == -1)
                    wave_end_time = device->getTimer()->getTime();
                else if(wave_end_time == -2 || wave_end_time < device->getTimer()->getTime() - 5000)
                {
                    std::cout << "respawning shit" << std::endl;
                    wave_end_time = -1;
                    waveMgr.incrementWaveId();
                    if(waveMgr.isCurrentWavePredetermined())
                        waveMgr.spawnWave(level, waveMgr.getCurrentWave(), computer, device, selector); //spawn next wave
                    else
                        waveGen.spawnWave(level, waveMgr.getCurrentWave(), computer, device, selector);
                }

        }

        driver->beginScene(true, true, iv::SColor(0, 0, 0, 0));

        player.update(receiver);
        computer.update(player);


        loot.update(player, receiver);
        hp_upgrade.update(player, receiver);
        stam_upgrade.update(player, receiver);
        souls.update(player.getSoulsToShow());
        waves.update(waveMgr.getCurrentWave());
        sun.update();
        cursor.update();

        smgr->drawAll();

        health_bar.update(player.getHealth());
        stamina_bar.update(player.getStamina());

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

        if (receiver.getStates()[EventReceiver::KEY_DEBUG_TRIGGER_SPAWN] && !flag)
        {
            flag = true;
            player.reset();
            hp_upgrade.reset();
            stam_upgrade.reset();
            computer.eraseAllEnemies();
            std::cout << "remaining: " << computer.getNumberOfEnemies() << std::endl;
            wave_end_time = -2;
            current_enemy_count = 0;
            waveMgr.reset();
            continue;
        }
    }
    device->drop();

    return 0;
}
