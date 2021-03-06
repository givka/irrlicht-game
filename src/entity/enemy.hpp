#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_H

#include <irrlicht.h>
#include <vector>
#include "player.hpp"
#include "../utils.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Enemy
{
  public:
    Enemy();
    Enemy(float health, int damage, float scale, float speed, int model_id, int swing_timer);
    bool isDead();
    bool isAlive();
    void update(Player &player, std::vector<Enemy> enemies);
    void initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, iv::ITexture *texture, is::ITriangleSelector *selector, int enemy_id);
    void setPosition(ic::vector3df pos);
    void setOrientation(ic::vector3df ori);
    void removeNodes();
    is::IAnimatedMeshSceneNode *getNode();

  private:
    enum damage_type
    {
        DT_NORMAL,
        DT_DOT,
        DT_CRIT
    };

    int m_model_id;

    int m_id;
    float m_health;
    float m_max_health;
    int m_damage;
    float m_scale;
    int m_death_time;
    bool m_already_hit_player;
    int m_last_swing_number = 0;
    int m_swing_timer;
    int m_last_swing_time;
    float m_speed = 1.0;
    //TODO: other way to access it globaly ??
    irr::IrrlichtDevice *m_device;
    damage_type m_last_hit_type;

    // DoT
    int m_last_dot_time = 0;
    int m_dot_tick_number = 0;
    Sword::enchant m_current_effect = Sword::NONE;
    float FROST_SPEED = 0.25;

    // health bar
    float m_health_bar_size;
    is::IBillboardSceneNode *m_health_bar;
    is::IBillboardSceneNode *m_health_bar_bg;

    ic::vector3df m_death_dir;
    ic::vector3df m_knockback_dir;
    ic::vector3df m_last_position;
    is::IAnimatedMeshSceneNode *m_node;

    is::IParticleSystemSceneNode *m_effect_node = 0;
    is::IParticleSystemSceneNode *m_blood_node = 0;
    int m_blood_timer;

    int m_souls;

    enum enemy_state
    {
        IDLE,
        WALKING,
        ATTACKING,
        STAGGERED,
        DYING,
        DEAD
    };

    void updateRotation(Player &player);
    void updatePosition(std::vector<Enemy> enemies);
    void updateDeath(Player &player);
    bool isBeingAttacked(Player &player);
    void attackPlayer(Player &player);
    void checkEnchantment(Player &player);
    void setEffect(Player &player, ic::vector3df direction);
    void checkDoT(Player &player);
    void updateDamageText();
    void addBloodEffect(damage_type dt);
    void checkBloodTimer();
    void removeHealth(Player &player, const float damage, damage_type dt);
    void addDamageText(Player &player, const float damage, damage_type dt);
    void switchToState(enemy_state state, Player &player);
    bool isAtRange(Player &player);
    void updateKnockback(Player &player);

    struct DamageText
    {
        is::IBillboardTextSceneNode *node;
        int creation_time;
        ic::vector3df direction;
    };

    ig::IGUIFont *m_font;
    std::vector<DamageText> m_damage_texts;

    enemy_state m_state = WALKING;
};

#endif
