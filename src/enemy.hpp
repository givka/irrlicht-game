#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>
#include <vector>
#include "player.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class Enemy
{
public:

    Enemy();
    bool isDead();
    void setNode(iv::IVideoDriver *driver, is::ISceneManager *smgr, is::IAnimatedMesh *mesh, int const id);
    is::IAnimatedMeshSceneNode *getNode();
    void addCollisionMap(is::ISceneManager *smgr, is::ITriangleSelector *selector);
    void update(Player player, std::vector<Enemy> enemies);
    void kill(ic::vector3df);

private:

    int m_id;
    int m_health;
    int m_damage;
    bool m_dead = false;
    ic::vector3df m_death_dir;
    ic::vector3df m_last_position;
    is::IAnimatedMeshSceneNode *m_node;

    bool isAllowedToMove(std::vector<Enemy> enemies);
    void updateRotation(Player player);
    void updatePosition(Player player, std::vector<Enemy> enemies);
    void updateDeath();


    enum enemy_state //wtf
    {
        IS_ALIVE,
        IS_DYING,
        IS_DEAD,
    };
    enemy_state state = IS_ALIVE;
};

#endif
