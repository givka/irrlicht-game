#include <irrlicht.h>

#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Computer::Computer()
{
}

void Computer::addEnemy(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector)
{
    Enemy enemy;
    enemy.initialise(device, mesh, texture, selector, nbrEnemiesAdded++);
    m_enemies.push_back(enemy);
}

void Computer::addEnemy(Enemy *enemy, ic::vector3df &pos, ic::vector3df &ori, irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector)
{
    enemy->initialise(device, mesh, texture, selector, nbrEnemiesAdded++);
    Enemy e = *enemy;
    e.setPosition(pos);
    e.setOrientation(ori);
    m_enemies.push_back(e);
}

void Computer::update(Player &player)
{
    updatePosition(player);
}

void Computer::updatePosition(Player &player)
{
    for (size_t index = 0; index < m_enemies.size();)
    {
        m_enemies[index].update(player, m_enemies);
        if (m_enemies[index].isDead())
            m_enemies.erase(m_enemies.begin() + index);
        else
            index++;
    }
}

bool Computer::isWaveFinished()
{
    return m_enemies.size() == 0;
}

int Computer::getNumberOfEnemies() {
    return (int) m_enemies.size();
}

void Computer::eraseAllEnemies() {
    for(auto e : m_enemies)
    {
        e.removeNodes();
    }
    m_enemies.clear();
}
