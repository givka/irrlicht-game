#include "upgrade.hpp"

Upgrade::Upgrade(irr::IrrlichtDevice *device, UpgradeType type, ic::vector3df position, int increment, int cost, Player &player)
    : m_device(device), m_type(type), m_position(position), m_increment(increment), m_cost(cost), m_original_cost(cost)
{
    switch (type)
    {
    case HEALTH:
        m_name = "Health";
        m_current_val = player.getHealth();
        m_node = addSphere(iv::SColor(255, 255, 0, 0));
        break;
    case STAMINA:
        m_name = "Stamina";
        m_current_val = player.getStamina();
        m_node = addSphere(iv::SColor(255, 0, 255, 0));
        break;
    case SWORD:
        m_name = "Sword";
        int damage = 30;
        int crit_chance = 5;
        Sword::enchant ench = static_cast<Sword::enchant>(rand() % Sword::ARRAY_END);
        m_sword = Sword(damage, crit_chance, ench);

        m_sword.initialise(m_device, 0);
        m_node = m_sword.getNode();
        m_node->setPosition(m_position);
        m_node->setRotation(ic::vector3df(0, 0, 0));
        break;
    }
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

    int y = m_node->getPosition().Y;
    m_tooltip_texts.clear();

    m_tooltip_texts.push_back(addTooltipText("Cost: " + std::to_string(m_cost), y += 20));
    if (m_type == SWORD)
    {
        Sword player_sword = player.getSword();
        m_tooltip_texts.push_back(addTooltipText(player_sword.getEnchantText(m_sword), y += 10));
        m_tooltip_texts.push_back(addTooltipText(player_sword.getCritChanceText(m_sword), y += 5));
        m_tooltip_texts.push_back(addTooltipText(player_sword.getDamageText(m_sword), y += 5));
    }
    else
    {
        m_tooltip_texts.push_back(addTooltipText(std::to_string(m_current_val) + std::string(" -> ") + std::to_string(m_current_val + m_increment), y += 10));
        m_tooltip_texts.push_back(addTooltipText(std::string("Upgrade: ") + m_name + std::string(" (+") + std::to_string(m_increment) + std::string(")"), y += 5));
    }
    m_tooltip_texts.push_back(addTooltipText("Press E to Buy", y += 10));
}

is::ISceneNode *Upgrade::addSphere(iv::SColor color)
{
    is::ISceneNode *node = m_device->getSceneManager()->addCubeSceneNode(10, 0, -1, m_position, ic::vector3df(50, 0, 0));
    is::IParticleSystemSceneNode *particle_node = Utils::setParticuleSystem(m_device, node, ic::vector3df(0, 0, 0), color);
    particle_node->getEmitter()->setMaxStartSize(ic::dimension2df(10, 10));
    particle_node->getEmitter()->setMinStartSize(ic::dimension2df(25, 25));
    particle_node->getEmitter()->setMaxAngleDegrees(360);
    particle_node->getEmitter()->setMaxLifeTime(200);
    return node;
}

irr::scene::IBillboardTextSceneNode *Upgrade::addTooltipText(std::string text, int y)
{
    std::wstring wtext = std::wstring(text.begin(), text.end());
    ic::vector3df pos = m_position;
    pos.Y = y;

    int size = 5;

    is::IBillboardTextSceneNode *node = m_device->getSceneManager()->addBillboardTextSceneNode(
        m_font, wtext.c_str(), 0, irr::core::dimension2d<irr::f32>((8 * size), (size)), pos, -1);
    node->setColor(iv::SColor(255, 255, 255, 255));

    return node;
}

void Upgrade::update(Player &player, EventReceiver &receiver)
{
    ic::vector3df rotation = m_node->getRotation();
    rotation.Y++;
    m_node->setRotation(rotation);

    if (receiver.getStates()[EventReceiver::KEY_SWITCH_WEAPON])
    {

        if (player.getNode()->getPosition().getDistanceFrom(m_position) < 50 && player.getSouls() > m_cost)
        {
            player.removeSouls(m_cost);
            switch (m_type)
            {
            case HEALTH:
                player.addMaxHealth(m_increment);
                m_current_val += m_increment;
                break;
            case STAMINA:
                player.addMaxStam(m_increment);
                m_current_val += m_increment;
                break;
            case SWORD:
                changeSword(player);
                break;
            default:
                break;
            }
            m_cost *= 1.2;
            addTooltip(player);
        }
    }
}

void Upgrade::reset()
{
    m_cost = m_original_cost;
}

void Upgrade::changeSword(Player &player)
{
    if (m_device->getTimer()->getTime() - m_timer < 200)
        return;
    m_timer = m_device->getTimer()->getTime();
    Sword sword_buffer = player.getSword();
    player.setSword(m_sword);

    m_node->remove();
    m_node = 0;
    m_sword = Sword(1.5 * m_sword.getDamageMin(), m_sword.getCritPercent() + rand() % 5, static_cast<Sword::enchant>(rand() % Sword::ARRAY_END));
    m_sword.initialise(m_device, 0);
    m_node = m_sword.getNode();
    m_node->setPosition(m_position);
    m_node->setRotation(ic::vector3df(0, 0, 0));
}