#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>
#include <vector>
#include "player.hpp"
#include "events.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Enemy
{
  public:
    Enemy();
    Enemy(int health, int damage, float scale);
    bool isDead();
    bool isAlive();
    void update(Player &player, std::vector<Enemy> enemies);
    void initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector, int enemy_id);
    void setPosition(ic::vector3df pos);
    void setOrientation(ic::vector3df ori);
    is::IAnimatedMeshSceneNode *getNode();

  private:
    int m_id;
    int m_health;
    int m_damage;
    float m_scale;
    int m_death_time;
    bool m_already_hit_player;
    int m_last_swing_number = 0;
    //TODO: other way to access it globaly ??
    irr::IrrlichtDevice *m_device;

    float m_health_bar_size;
    is::IBillboardSceneNode *m_health_bar;
    is::IBillboardSceneNode *m_health_bar_bg;

    ic::vector3df m_death_dir;
    ic::vector3df m_last_position;
    is::IAnimatedMeshSceneNode *m_node;

    is::IParticleSystemSceneNode *m_effect = 0;

    void
    updateRotation(Player &player);
    void updatePosition(std::vector<Enemy> enemies);
    void updateDeath();
    bool isBeingAttacked(Player &player);
    bool isAttacking(Player &player);
    void attackPlayer(Player &player);
    void checkEnchantment(Player &player);
    void setEffect(Player &player, ic::vector3df direction);

    enum enemy_state
    {
        IS_RUNNING,
        IS_DYING,
        IS_DEAD,
        IS_ATTACKING,
    };
    enemy_state m_state = IS_RUNNING;
};

#endif
