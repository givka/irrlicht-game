#include "loot.hpp"

Loot::Loot(irr::IrrlichtDevice *device)
    : m_device(device)
{

    int damage = 100 + rand() % 100;
    int crit_chance = rand() % 100;
    Sword::enchant ench = static_cast<Sword::enchant>(rand() % Sword::ARRAY_END);

    m_sword = Sword(damage, crit_chance, ench);
    m_cost = 500 + rand() % 500;

    std::cout << damage << ", " << crit_chance << ", " << ench << std::endl;

    m_sword.initialise(m_device, 0);
    m_sword.getNode()->setPosition(ic::vector3df(105, -10, 100));
    m_sword.getNode()->setRotation(ic::vector3df(0, 0, 0));
}

void Loot::update()
{
    ic::vector3df rotation = m_sword.getNode()->getRotation();

    rotation.Y++;

    m_sword.getNode()->setRotation(rotation);
}
