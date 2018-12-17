#include "spawnPoint.hpp"

SpawnPoint::SpawnPoint(int id, ic::vector3df position, ic::vector3df orientation, bool is_visible, is::ISceneNode * node)
: m_id(id), m_position(position), m_orientation(orientation), m_visible(is_visible), m_node(node)
{
    if(m_node != nullptr)
        m_node->setVisible(m_visible);
}

void SpawnPoint::setNode(is::ISceneNode* node)
{
    m_node = node;
    if(m_node != nullptr)
        m_node->setVisible(m_visible);
}

void SpawnPoint::setVisible(bool visible)
{
    m_visible = visible;
    if(m_node != nullptr)
        m_node->setVisible(m_visible);
}

ic::vector3df SpawnPoint::getPosition()
{
    return m_position;
}

ic::vector3df SpawnPoint::getOrientation()
{
    return m_orientation;
}

is::ISceneNode *SpawnPoint::getNode()
{
    return m_node;
}

int SpawnPoint::getId()
{
    return m_id;
}

bool SpawnPoint::isVisible()
{
    return m_visible;
};