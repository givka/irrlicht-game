#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include <irrlicht.h>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class SpawnPoint{
public:
    SpawnPoint(int id, ic::vector3df position, ic::vector3df orientation, bool is_visible=false, is::ISceneNode * node=nullptr);

    is::ISceneNode *getNode();
    void setNode(is::ISceneNode* node);

    int getId();

    bool isVisible();
    void setVisible(bool visible);

    ic::vector3df getPosition();
    ic::vector3df getOrientation();

private:
    is::ISceneNode *m_node;

    bool m_visible;
    int m_id;

    ic::vector3df m_position;
    ic::vector3df m_orientation;
};

#endif