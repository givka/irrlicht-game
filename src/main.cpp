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

typedef enum GameState
{
    MENU_SCREEN,
    GAME,
    END_SCREEN
} GameState;

void initState(GameState state)
{
    switch (state)
    {
    case GAME:
        std::cout << "init game" << std::endl;
        break;
    case MENU_SCREEN:
        std::cout << "init menu" << std::endl;
        break;
        break;
    case END_SCREEN:
        std::cout << "init end" << std::endl;
        break;
    default:
        break;
    }
}
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

    Upgrade hp_upgrade(device, HEALTH, ic::vector3df(352, 125, -86), 10, 25, player);
    Upgrade damage_upgrade(device, SWORD, ic::vector3df(102, 125, -108), 10, 100, player);
    Upgrade stam_upgrade(device, STAMINA, ic::vector3df(-119, 125, -141), 10, 25, player);

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

    GameState game_state = MENU_SCREEN;
    initState(game_state);

    bool state_init_flag = false;
    auto start_text = gui->addStaticText(L"Press [E] to start",
                                         ic::rect<irr::s32>(10, HEIGHT - 100, 500, HEIGHT));
    auto help_text = gui->addStaticText(L"Press [R] to display help",
                                        ic::rect<irr::s32>(10, HEIGHT - 60, WIDTH, HEIGHT));
    auto title_text = gui->addStaticText(L"NOM DU JEU",
                                         ic::rect<irr::s32>(WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT));
    auto game_over_text = gui->addStaticText(L"YOU DIED",
                                             ic::rect<irr::s32>(WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT));
    auto restart_text = gui->addStaticText(L"Press [R] to restart",
                                           ic::rect<irr::s32>(WIDTH / 2, HEIGHT / 2, WIDTH, HEIGHT));
    auto help_screen_text = gui->addStaticText(L"Waves of monsters will appear !\nDefeat them all to progress\nYou can upgrade your character ([E] to buy)\n[Z, Q, S, D] to move\n[Left Click] to Attack\n[Right Click] to parry\n",
                                               ic::rect<irr::s32>(0, 0, WIDTH, HEIGHT));
    help_screen_text->setOverrideColor(iv::SColor(255, 255, 255, 255));
    help_screen_text->setVisible(false);

    restart_text->setOverrideColor(iv::SColor(255, 255, 255, 255));
    restart_text->setRelativePosition(ic::rect<s32>(WIDTH / 2 - restart_text->getTextWidth() / 2, HEIGHT / 2 + 70, WIDTH, HEIGHT));
    restart_text->setVisible(false);
    game_over_text->setRelativePosition(ic::rect<s32>(WIDTH / 2 - title_text->getTextWidth() / 2, HEIGHT / 2, WIDTH, HEIGHT));
    game_over_text->setOverrideColor(iv::SColor(255, 255, 0, 0));
    game_over_text->setVisible(false);

    auto fader = gui->addInOutFader();
    int fade_out_time = 0;

    while (device->run())
    {
        switch (game_state)
        {
        case MENU_SCREEN:
        {
            if (!state_init_flag)
            {
                enemy_count_text->setVisible(false);
                driver->draw2DImage(driver->getTexture("data/bg.png"), ic::rect<s32>(0, 0, WIDTH, HEIGHT),
                                    ic::rect<s32>(0, 0, 1920, 1007));
                start_text->setOverrideColor(iv::SColor(255, 255, 255, 255));
                help_text->setOverrideColor(iv::SColor(255, 255, 255, 255));
                title_text->setOverrideColor(iv::SColor(255, 255, 255, 255));
                title_text->setRelativePosition(ic::rect<s32>(WIDTH / 2 - title_text->getTextWidth() / 2, HEIGHT / 2, WIDTH, HEIGHT));
                state_init_flag = true;
            }
            if (receiver.getStates()[EventReceiver::KEY_SWITCH_WEAPON])
            {
                state_init_flag = false;
                start_text->setVisible(false);
                title_text->setVisible(false);
                help_text->setVisible(false);
                enemy_count_text->setVisible(true);
                help_screen_text->setVisible(false);
                initState(GAME);
                game_state = GAME;
                continue;
            }
            else if (receiver.getStates()[EventReceiver::KEY_DEBUG_TRIGGER_SPAWN])
            {
                help_screen_text->setVisible(true);
            }
            break;
        };
        case END_SCREEN:
        {
            if (!state_init_flag)
            {
                waves.setVisible(false);
                souls.setVisible(false);
                cursor.setVisible(false);
                fader->fadeOut(4000);
                fader->setVisible(true);
                fade_out_time = device->getTimer()->getTime();
                state_init_flag = true;
                enemy_count_text->setVisible(false);
            }
            game_over_text->setVisible(true);
            if (fade_out_time + 4000 < device->getTimer()->getTime())
            {
                fader->setVisible(false);
                driver->beginScene(true, true, iv::SColor(255, 0, 0, 0));
                fader->setEnabled(false);
                restart_text->setVisible(true);
            }
            if (fade_out_time + 2000 < device->getTimer()->getTime() &&
                receiver.getStates()[EventReceiver::KEY_DEBUG_TRIGGER_SPAWN])
            {
                state_init_flag = false;
                game_over_text->setVisible(false);
                restart_text->setVisible(false);
                enemy_count_text->setVisible(true);
                initState(GAME);
                game_state = GAME;
                flag = true;
                player.reset();
                hp_upgrade.reset();
                stam_upgrade.reset();
                computer.eraseAllEnemies();
                std::cout << "remaining: " << computer.getNumberOfEnemies() << std::endl;
                wave_end_time = -2;
                current_enemy_count = 0;
                waveMgr.reset();
                waves.setVisible(true);
                souls.setVisible(true);
                cursor.setVisible(true);
                continue;
            }
            break;
        }
        case GAME:
            if (current_enemy_count != computer.getNumberOfEnemies())
            {
                current_enemy_count = computer.getNumberOfEnemies();
                enemy_count_string = L"Enemies: " + std::to_wstring(current_enemy_count);
                enemy_count_text->setText(enemy_count_string.c_str());
            }
            else if (wave_end_time >= 0)
                enemy_count_text->setText(L"Next wave incoming");

            //check for end of wave, start next wave //TODO: add score, pause between waves, etc
            if (computer.isWaveFinished())
            {
                if (wave_end_time == -1)
                    wave_end_time = device->getTimer()->getTime();
                else if (wave_end_time == -2 || wave_end_time < device->getTimer()->getTime() - 3000)
                {
                    wave_end_time = -1;
                    waveMgr.incrementWaveId();
                    if (waveMgr.isCurrentWavePredetermined())
                        waveMgr.spawnWave(level, waveMgr.getCurrentWave(), computer, device, selector); //spawn next wave
                    else
                        waveGen.spawnWave(level, waveMgr.getCurrentWave(), computer, device, selector);
                }
            }

            driver->beginScene(true, true, iv::SColor(0, 0, 0, 0));

            player.update(receiver);
            computer.update(player);

            smgr->drawAll();
            health_bar.update(player.getHealth(), player.getMaxHealth());
            stamina_bar.update(player.getStamina(), player.getMaxStamina());
            damage_upgrade.update(player, receiver);
            hp_upgrade.update(player, receiver);
            stam_upgrade.update(player, receiver);
            souls.update(player.getSoulsToShow());
            waves.update(waveMgr.getCurrentWave());
            sun.update();
            cursor.update();
            if (player.getHealth() <= 0 || player.getPosition().Y < -50)
            {
                initState(END_SCREEN);
                game_state = END_SCREEN;
            }
            break;
        }
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
