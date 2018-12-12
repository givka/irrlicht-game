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
    checkAttack(player, receiver);
    updatePosition(player);
}

void Computer::checkAttack(Player player, EventReceiver *receiver)
{
    if (receiver->states[receiver->KEY_ATTACK] == false)
        return;

    for (size_t index = 0; index < m_enemies.size(); index++)
    {

        ic::vector3df positionPlayer = player.node->getPosition();
        ic::vector3df positionEnemy = m_enemies[index].getNode()->getPosition();
        float const distance = positionEnemy.getDistanceFrom(positionPlayer);

        if (distance < 50.0)
        {
            m_enemies[index].kill(positionEnemy - positionPlayer);
        }
    }
}

void Computer::updatePosition(Player player)
{
    for (size_t index = 0; index < m_enemies.size();)
    {
        m_enemies[index].update(player, m_enemies);
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
