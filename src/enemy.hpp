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
    Enemy(float health, int damage, float scale);
    bool isDead();
    bool isAlive();
    void update(Player &player, std::vector<Enemy> enemies);
    void initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector, int enemy_id);
    void setPosition(ic::vector3df pos);
    void setOrientation(ic::vector3df ori);
    is::IAnimatedMeshSceneNode *getNode();

  private:
    int m_id;
    float m_health;
    int m_damage;
    float m_scale;
    int m_death_time;
    bool m_already_hit_player;
    int m_last_swing_number = 0;
    //TODO: other way to access it globaly ??
    irr::IrrlichtDevice *m_device;

    // DoT
    int m_last_dot_time = 0;
    int m_dot_tick_number = 0;
    Sword::enchant m_current_effect = Sword::NONE;
    float resistance_fire = 0.5;
    float resistance_poison = 0.3;
    float DOT_DAMAGE = 20;

    // health bar
    float m_health_bar_size;
    is::IBillboardSceneNode *m_health_bar;
    is::IBillboardSceneNode *m_health_bar_bg;

    ic::vector3df m_death_dir;
    ic::vector3df m_last_position;
    is::IAnimatedMeshSceneNode *m_node;

    is::IParticleSystemSceneNode *m_effect_node = 0;
    is::IParticleSystemSceneNode *m_blood_node = 0;
    int m_blood_timer;

    void
    updateRotation(Player &player);
    void updatePosition(std::vector<Enemy> enemies);
    void updateDeath();
    bool isBeingAttacked(Player &player);
    bool isAttacking(Player &player);
    void attackPlayer(Player &player);
    void checkEnchantment(Player &player);
    void setEffect(Player &player, ic::vector3df direction);
    void checkDoT(Player &player);
    void removeHealth(Player &player, const float damage);
    void addDamageText(Player &player, const float damage);
    void updateDamageText();
    void addBloodEffect();
    void checkBloodTimer();

    struct DamageText
    {
        is::IBillboardTextSceneNode *node;
        int creation_time;
        ic::vector3df direction;
    };

    std::vector<DamageText> m_damage_texts;

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
