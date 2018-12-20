//
// Created by adrien on 12/19/18.
//

#include "waveGenerator.hpp"
#include "WaveManager.h"

WaveGenerator::WaveGenerator(int health_increment, int health_variance, int damage_increment, int damage_variance, int wave_per_new_enemy, int nb_enemy_variance) :
m_health_increment(health_increment), m_health_variance(health_variance),
m_damage_increment(damage_increment), m_damage_variance(damage_variance),
m_wave_per_new_enemy(wave_per_new_enemy), m_nb_enemy_variance(nb_enemy_variance)
{

}

void WaveGenerator::spawnWave(Level &level, int wave_id, Computer &computer, irr::IrrlichtDevice *device, is::ITriangleSelector *selector) {
    //number of enemies to spawn
    int nb_enemies = 2 + wave_id / m_wave_per_new_enemy;

    //spawn points sets
    std::vector<int> not_picked;
    for(int i = 0; i < level.getNbSpawnPoints(); i++)
    {
        not_picked.push_back(i);
    }

    std::vector<int> generated;
    for(int i = 0; i < nb_enemies; i++)
    {
        int id = (int) (rand() % not_picked.size());
        generated.push_back(not_picked[id]);
        not_picked.erase(not_picked.begin() + id);
    }
    //enemies
    Spawn spawn;
    for(int sp : generated)
    {
        int model_id = rand() % 19;

        //choose model
        std::string path_text = "data/models/" + std::to_string(model_id) + ".pcx";
        std::string path_model = "data/models/" + std::to_string(model_id) + ".md2";
        is::IAnimatedMesh *mesh = device->getSceneManager()->getMesh(std::wstring(path_model.begin(), path_model.end()).c_str());
        iv::ITexture *texture = device->getVideoDriver()->getTexture(std::wstring(path_text.begin(), path_text.end()).c_str());

        //generate stats
        spawn.health = 40 + wave_id * m_health_increment + (rand() % (m_health_variance)) - m_health_variance/2;
        spawn.damage = 10 + wave_id * m_damage_increment + (rand() % (m_damage_variance)) - m_damage_variance/2;
        spawn.scale = 1.0; (float) (1.0 + rand() / 4.0);
        spawn.speed = 1.0; // (float) (1.0 + rand() / 4.0);
        spawn.swing_timer = 1500 + (rand() % 50) - 25;
        spawn.spawn_point_id = sp;
        Enemy enemy(spawn.health, spawn.damage, spawn.scale, spawn.speed, model_id, spawn.swing_timer);

        auto pos = level.getSpawnPoint(spawn.spawn_point_id).getPosition();
        auto rot = level.getSpawnPoint(spawn.spawn_point_id).getOrientation();

        computer.addEnemy(&enemy, pos, rot, device, mesh, texture, selector);
    }
}

