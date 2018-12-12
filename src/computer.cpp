#include <irrlicht.h>

#include "computer.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Computer::Computer()
{
}

void Computer::addEnemy(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector)
{
    Enemy enemy;
    enemy.initialise(device, mesh, selector, nbrEnemiesAdded++);
    m_enemies.push_back(enemy);
}

void Computer::addEnemy(Enemy *enemy, ic::vector3df &pos, ic::vector3df &ori, irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector)
{
    enemy->initialise(device, mesh, selector, nbrEnemiesAdded++);
    Enemy e = *enemy;
    e.setPosition(pos);
    e.setOrientation(ori);
    m_enemies.push_back(e);
}

void Computer::update(Player player, EventReceiver *receiver)
{
    updatePosition(player, receiver);
}

void Computer::updatePosition(Player player, EventReceiver *receiver)
{
    for (size_t index = 0; index < m_enemies.size();)
    {
        m_enemies[index].update(player, m_enemies, receiver);
        if (m_enemies[index].isDead())
        {
            m_enemies.erase(m_enemies.begin() + index);
            std::cout << "enemies remaining: " << m_enemies.size() << std::endl;
        }
        else
            index++;
    }
}

bool Computer::isWaveFinished()
{
    return m_enemies.size() == 0;
}
