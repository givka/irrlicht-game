//
// Created by adrien on 12/11/18.
//

#ifndef IRRLICHT_GAME_WAVEMANAGER_H
#define IRRLICHT_GAME_WAVEMANAGER_H

#include <vector>
#include <string>
#include "irrlicht.h"
#include "computer.hpp"
#include "level.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

typedef struct Spawn
{
    int spawn_point_id;
    int health;
    int damage;
    float scale;
    //float speed;
    //todo: model id/pointer, same for tex
}Spawn;

typedef struct Wave
{
    int id;
    std::vector<Spawn> spawns;
}Wave;

class WaveManager {
public:
    WaveManager();
    void loadJSON(std::string path_to_json);
    int getCurrentWave();
    void spawnWave(Level &level, int wave_id, Computer &computer, iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *meshSkeleton, is::ITriangleSelector *selector); //todo:: model mgt system

private:
    int m_current_wave;
    std::vector<Wave> m_waves;
};


#endif //IRRLICHT_GAME_WAVEMANAGER_H
