//
// Created by adrien on 12/19/18.
//

#ifndef IRRLICHT_GAME_WAVEGENERATOR_HPP
#define IRRLICHT_GAME_WAVEGENERATOR_HPP

#include <vector>
#include <string>
#include "irrlicht.h"
#include "level.hpp"
#include "computer.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class WaveGenerator
{
public:
    WaveGenerator(int health_increment = 10, int health_variance = 10, int damage_increment = 5, int damage_variance = 5, int wave_per_new_enemy = 2, int nb_enemy_variance = 1);
    void spawnWave(Level &level, int wave_id, Computer &computer, irr::IrrlichtDevice *device, is::ITriangleSelector *selector);


private:
    //simple linear difficulty
    int m_health_increment;
    int m_damage_increment;
    int m_health_variance;
    int m_damage_variance;
    int m_wave_per_new_enemy;
    int m_nb_enemy_variance;

};

#endif //IRRLICHT_GAME_WAVEGENERATOR_HPP
