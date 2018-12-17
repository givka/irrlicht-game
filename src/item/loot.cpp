#include "loot.hpp"

Loot::Loot(irr::IrrlichtDevice *device, Player &player)
    : m_device(device)
{

    int damage = 10 + rand() % 10;
    int crit_chance = rand() % 100;
    Sword::enchant ench = static_cast<Sword::enchant>(rand() % Sword::ARRAY_END);

    m_sword = Sword(damage, crit_chance, ench);
    m_cost = 500 + rand() % 500;

    m_sword.initialise(m_device, 0);
    m_sword.getNode()->setPosition(ic::vector3df(105, -10, 100));
    m_sword.getNode()->setRotation(ic::vector3df(0, 0, 0));

    //TODO: add background tooltip
    // int size = 50;
    // m_tootlip_bg = m_device->getSceneManager()->addBillboardSceneNode(0, ic::dimension2d<irr::f32>(size * 16.0 / 9.0, size), ic::vector3df(105, 20, 100), -1, iv::SColor(150, 0, 0, 0), iv::SColor(150, 0, 0, 0));
    // m_tootlip_bg->setMaterialType(iv::EMT_TRANSPARENT_ALPHA_CHANNEL);
    // m_tootlip_bg->setMaterialFlag(iv::EMF_LIGHTING, false);
    addTooltip(player);
}

void Loot::addTooltip(Player &player)
{

    for (size_t index = 0; index < m_tooltip_texts.size(); index++)
    {
        if (m_tooltip_texts[index])
            m_tooltip_texts[index]->remove();
        m_tooltip_texts[index] = 0;
    }

    int y = 40;
    Sword player_sword = player.getSword();

    m_tooltip_texts.clear();
    m_tooltip_texts.push_back(addTooltipText("Press E to Buy", y));
    m_tooltip_texts.push_back(addTooltipText(player_sword.getDamageText(m_sword), y -= 10));
    m_tooltip_texts.push_back(addTooltipText(player_sword.getCritChanceText(m_sword), y -= 5));
    m_tooltip_texts.push_back(addTooltipText(player_sword.getEnchantText(m_sword), y -= 5));
    m_tooltip_texts.push_back(addTooltipText("Cost: " + std::to_string(m_cost), y -= 10));
}

irr::scene::IBillboardTextSceneNode *Loot::addTooltipText(std::string text, int y)
{
    std::wstring wtext = std::wstring(text.begin(), text.end());
    ic::dimension2du s = m_device->getGUIEnvironment()->getBuiltInFont()->getDimension(wtext.c_str());

    return m_device->getSceneManager()->addBillboardTextSceneNode(
        0, wtext.c_str(), 0, ic::dimension2d<irr::f32>(s.Width / 2, s.Height / 2), ic::vector3df(105, y, 100));
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

        if (position_player.getDistanceFrom(position_loot) < 50)
            switchSword(player);
    }
}

void Loot::switchSword(Player &player)
{
    if (m_device->getTimer()->getTime() - m_timer < 200)
        return;

    m_timer = m_device->getTimer()->getTime();
    Sword sword_buffer = player.getSword();
    player.setSword(m_sword);
    m_sword.switchStats(sword_buffer, 0);
    m_sword.getNode()->setPosition(ic::vector3df(105, -10, 100));
    m_sword.getNode()->setRotation(ic::vector3df(0, 0, 0));
    m_cost = 0;
    addTooltip(player);
}
