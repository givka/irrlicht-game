#include "loot.hpp"

Loot::Loot(irr::IrrlichtDevice *device)
    : m_device(device)
{

    int damage = 10 + rand() % 10;
    int crit_chance = rand() % 100;
    Sword::enchant ench = static_cast<Sword::enchant>(rand() % Sword::ARRAY_END);

    m_sword = Sword(damage, crit_chance, ench);
    m_cost = 500 + rand() % 500;

    std::cout << damage << ", " << crit_chance << ", " << ench << std::endl;

    m_sword.initialise(m_device, 0);
    m_sword.getNode()->setPosition(ic::vector3df(105, -10, 100));
    m_sword.getNode()->setRotation(ic::vector3df(0, 0, 0));
}

void Loot::update(Player &player, EventReceiver &receiver)
{
    ic::vector3df rotation_loot = m_sword.getNode()->getRotation();
    rotation_loot.Y++;
    m_sword.getNode()->setRotation(rotation_loot);

    if (receiver.getStates()[EventReceiver::KEY_SWITCH_WEAPON])
    {
        ic::vector3df position_loot = m_sword.getNode()->getPosition();
        ic::vector3df position_player = player.getNode()->getPosition();

        std::cout << position_player.getDistanceFrom(position_loot) << std::endl;
        if (position_player.getDistanceFrom(position_loot) < 50)
            switchSword(player);
    }
}

void Loot::switchSword(Player &player)
{
    Sword sword_buffer = player.getSword();
    player.setSword(m_sword);
    m_sword.switchStats(sword_buffer, 0);
    m_sword.getNode()->setPosition(ic::vector3df(105, -10, 100));
    m_sword.getNode()->setRotation(ic::vector3df(0, 0, 0));
}
