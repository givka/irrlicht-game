#include "upgrade.hpp"

Upgrade::Upgrade(irr::IrrlichtDevice *device, Player &player, UpgradeType type, ic::vector3df position, int increment, int cost)
    : m_device(device), m_position(position), m_increment(increment), m_cost(cost), m_type(type)
{
    switch(type)
    {
        case HEALTH:
            m_name = "Health";
            m_current_val = player.getHealth();
        break;
        case STAMINA:
            m_name = "Stamina";
            m_current_val = player.getStamina();
        break;
        case DAMAGE:
            m_name = "Damage";
            m_current_val = 0; //player.getDamage();
        break;
    }
    std::cout << m_name << std::endl;
    m_font = m_device->getGUIEnvironment()->getFont("data/myfont.xml");
    addTooltip(player);
}

void Upgrade::addTooltip(Player &player)
{

    for (size_t index = 0; index < m_tooltip_texts.size(); index++)
    {
        if (m_tooltip_texts[index])
            m_tooltip_texts[index]->remove();
        m_tooltip_texts[index] = 0;
    }

    int y = m_position.Y;
    //Sword player_sword = player.getSword();

    //int diff_damage = player_sword.getDamageMax() - m_sword.getDamageMax();
    //int diff_crit = player_sword.getCritPercent() - m_sword.getCritPercent();

    m_tooltip_texts.clear();
    m_tooltip_texts.push_back(addTooltipText("Press E to Buy", y));
    m_tooltip_texts.push_back(addTooltipText(std::string("Upgrade: ") + m_name + std::string(" (+")  + std::to_string(m_increment) + std::string(")"), y -= 5));
    m_tooltip_texts.push_back(addTooltipText(std::to_string(m_current_val) + std::string(" -> ") + std::to_string(m_current_val + m_increment), y -= 5));
    std::cout << m_name << "fdkjhgfdkjgfd" << std::endl;
    m_tooltip_texts.push_back(addTooltipText("Cost: " + std::to_string(m_cost), y -= 5));
}

irr::scene::IBillboardTextSceneNode *Upgrade::addTooltipText(std::string text, int y)
{
    std::wstring wtext = std::wstring(text.begin(), text.end());
    //ic::dimension2du s = m_font->getDimension(wtext.c_str());
    std::cout << "wtext " << text.c_str() << std::endl ;
    ic::vector3df pos = m_position;
    pos.Y = y;

    int size = 5;

//    is::IBillboardTextSceneNode *node = m_device->getSceneManager()->addBillboardTextSceneNode(
//        m_font, wtext.c_str(), 0, ic::dimension2d<irr::f32>(s.Width / 2, s.Height / 2), pos);

    is::IBillboardTextSceneNode *node = m_device->getSceneManager()->addBillboardTextSceneNode(
            m_font, wtext.c_str(), 0, irr::core::dimension2d<irr::f32>((8*size), (size)), pos, -1);
    node->setColor(iv::SColor(255, 255, 255, 255));

    return node;
}

void Upgrade::update(Player &player, EventReceiver &receiver)
{

    if (receiver.getStates()[EventReceiver::KEY_SWITCH_WEAPON])
    {

        if (player.getNode()->getPosition().getDistanceFrom(m_position) < 50 && player.getSouls() > m_cost)
        {
            player.removeSouls(m_cost);
            switch(m_type)
            {
                case HEALTH:
                player.addMaxHealth(m_increment);
                m_current_val += m_increment;
                break;
                case STAMINA:
                player.addMaxStam(m_increment);
                m_current_val += m_increment;
                break;

            }
            addTooltip(player);
        }
    }
}

void Upgrade::switchSword(Player &player)
{
    if (m_device->getTimer()->getTime() - m_timer < 200)
        return;

    player.removeSouls(m_cost);
    m_timer = m_device->getTimer()->getTime();
    m_cost = 0;
    addTooltip(player);
}
