//
// Created by adrien on 12/11/18.
//

#include <fstream>
#include <sstream>
#include "WaveManager.h"
#include "level.hpp"
#include "document.h"

WaveManager::WaveManager() : m_current_wave(0)
{
}

void WaveManager::loadJSON(std::string path_to_json)
{
    //Careful, no type check
    //Careful, assumes waves are already ordered

    std::ifstream json_file_stream(path_to_json);
    std::stringstream buffer;
    buffer << json_file_stream.rdbuf();

    rapidjson::Document d;
    d.Parse(buffer.str().c_str());

    int id = 0;
    auto waves = d["waves"].GetArray();
    for (auto wave_it = waves.Begin(); wave_it != waves.End(); wave_it++)
    {
        Wave wave;
        wave.id = id;
        auto spawns = (*wave_it)["spawns"].GetArray();
        for (auto spawn_it = spawns.Begin(); spawn_it != spawns.End(); spawn_it++)
        {
            Spawn spawn;
            spawn.spawn_point_id = (*spawn_it)["spawn_point_id"].GetInt();
            spawn.health = (*spawn_it)["health"].GetInt();
            spawn.damage = (*spawn_it)["damage"].GetInt();
            spawn.scale = (*spawn_it)["scale"].GetFloat();
            spawn.swing_timer = (*spawn_it)["swing_timer"].GetInt();
            wave.spawns.push_back(spawn);
        }
        m_waves.push_back(wave);
        id++;
    }
}

int WaveManager::getCurrentWave()
{
    return m_current_wave;
}

void WaveManager::spawnWave(Level &level, int wave_id, Computer &computer, irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector)
{
    for (auto spawn : m_waves[wave_id].spawns) //assumes ids start at 0 and waves are sorted
    {
        Enemy enemy(spawn.health, spawn.damage, spawn.scale, spawn.swing_timer);
        auto pos = level.getSpawnPoint(spawn.spawn_point_id).getPosition();
        auto rot = level.getSpawnPoint(spawn.spawn_point_id).getOrientation();

        computer.addEnemy(&enemy, pos, rot, device, mesh, selector);
    }

    m_current_wave = wave_id;
}

unsigned long WaveManager::getLastWaveId()
{
    return m_waves.size() - 1;
}