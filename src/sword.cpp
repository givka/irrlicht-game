#include <irrlicht.h>

#include "sword.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Sword::Sword()
    : m_damage_min(20), m_damage_max(m_damage_min * 1.2), m_crit_percent(20), m_current_enchant(NONE)
{
}

Sword::Sword(int damage, int crit_percent, enchant ench)
    : m_damage_min(damage), m_damage_max(damage * 1.2), m_crit_percent(crit_percent), m_current_enchant(ench)
{
}
void Sword::initialise(irr::IrrlichtDevice *device, is::ISceneNode *nodePlayer)
{
    m_device = device;
    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();

    m_node = smgr->addMeshSceneNode(smgr->getMesh("data/sword.obj"), nodePlayer);
    m_node->setMaterialFlag(iv::EMF_LIGHTING, true);
    m_node->setMaterialTexture(0, driver->getTexture("data/sword_texture.jpg"));
    m_node->setPosition(core::vector3df(15, -10, 10));
    m_node->setRotation(core::vector3df(90, -10, 90));

    m_node_light = smgr->addLightSceneNode(m_node,
                                           ic::vector3df(12.5, -10, -15),
                                           iv::SColorf(getCurrentEnchantColor()),
                                           250.0f);

    m_particles.clear();

    m_particles.push_back(Utils::setParticuleSystem(
        device, m_node, ic::vector3df(5, 0, 0), m_enchant_colors[m_current_enchant]));
    m_particles.push_back(Utils::setParticuleSystem(
        device, m_node, ic::vector3df(10, 0, 0), m_enchant_colors[m_current_enchant]));
    m_particles.push_back(Utils::setParticuleSystem(
        device, m_node, ic::vector3df(15, 0, 0), m_enchant_colors[m_current_enchant]));
    m_particles.push_back(Utils::setParticuleSystem(
        device, m_node, ic::vector3df(20, 0, 0), m_enchant_colors[m_current_enchant]));
    m_particles.push_back(Utils::setParticuleSystem(
        device, m_node, ic::vector3df(25, 0, 0), m_enchant_colors[m_current_enchant]));
}

void Sword::setEnchantment(enchant new_enchantment)
{
    m_current_enchant = new_enchantment;
    iv::SColor color = getCurrentEnchantColor();
    m_node_light->getLightData().DiffuseColor = iv::SColorf(color);
    for (size_t index = 0; index < m_particles.size(); index++)
        m_particles[index]->getEmitter()->setMaxStartColor(m_enchant_colors[new_enchantment]);
}

void Sword::setAttack()
{
    if (m_is_attacking)
        return;
    m_is_blocking = false;
    m_is_attacking = true;
    m_sword_going_down = true;
    m_swing_number++;
    float X = 0;
    float Y = -100;
    float Z = std::rand() % 2 ? -5 : 5;
    m_destination = ic::vector3df(X, Y, Z);
}

void Sword::updatePosition()
{
    float speed = 10.0;
    if (!m_is_blocking && !m_is_attacking)
        return;

    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if (m_is_blocking)
    {
        rotation.Z = 160;
    }

    if (m_is_attacking)
    {
        position.Z += m_sword_going_down ? 1 : -1;
        rotation.Y += m_sword_going_down ? -speed : speed;
        rotation.Z += m_sword_going_down ? -m_destination.Z : m_destination.Z;

        if (rotation.Y < m_destination.Y)
            m_sword_going_down = false;

        if (!m_sword_going_down && rotation.Y >= -10)
            m_is_attacking = false;
    }

    m_node->setPosition(position);
    m_node->setRotation(rotation);
}

bool Sword::getIsAttacking()
{
    return m_is_attacking;
}

Sword::enchant Sword::getCurrentEnchant()
{
    return m_current_enchant;
}

int Sword::getSwingNumber()
{
    return m_swing_number;
}

float Sword::getAttack()
{
    const int damage_diff = m_damage_max - m_damage_min;
    return rand() % (damage_diff + 1) + m_damage_min;
}

iv::SColor Sword::getCurrentEnchantColor()
{
    return getEnchantColor(m_current_enchant);
}

iv::SColor Sword::getEnchantColor(enchant ench)
{
    return ench == Sword::NONE
               ? iv::SColor(255, 255, 255, 255)
               : m_enchant_colors[ench];
}

void Sword::startBlock()
{
    m_is_attacking = false;
    m_is_blocking = true;
    m_sword_going_down = false;
}

void Sword::endBlock()
{
    m_is_blocking = false;
    m_is_attacking = false;
    m_sword_going_down = false;
    m_node->setPosition(core::vector3df(15, -10, 10));
    m_node->setRotation(core::vector3df(90, -10, 90));
}

int Sword::getCritPercent()
{
    return m_crit_percent;
}

is::IMeshSceneNode *Sword::getNode()
{
    return m_node;
}

void Sword::switchStats(Sword sword, is::ISceneNode *node)
{
    m_node->remove();
    m_node = 0;

    m_current_enchant = sword.m_current_enchant;
    m_damage_min = sword.m_damage_min;
    m_damage_max = sword.m_damage_max;
    m_crit_percent = sword.m_crit_percent;

    initialise(m_device, node);
}

int Sword::getDamageMin()
{
    return m_damage_min;
}
int Sword::getDamageMax()
{
    return m_damage_max;
}

std::string Sword::getDamageText(Sword sword)
{
    return "Damage: " + std::to_string(m_damage_min) + " - " + std::to_string(m_damage_max) + " -> " +
           std::to_string(sword.m_damage_min) + " - " + std::to_string(sword.m_damage_max);
}

std::string Sword::getCritChanceText(Sword sword)
{
    return "Crit Chance: " + std::to_string(m_crit_percent) + "% -> " +
           std::to_string(sword.m_crit_percent) + "%";
}

std::string Sword::getEnchantText(Sword sword)
{
    return "Enchantment: " + m_enchant_names[m_current_enchant] + " -> " +
           m_enchant_names[sword.m_current_enchant];
}
