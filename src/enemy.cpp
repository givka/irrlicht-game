#include <irrlicht.h>
#include <iostream>

#include "enemy.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Enemy::Enemy() : m_health(100), m_damage(15), m_scale(1.0f), m_already_hit_player(false)
{
}

Enemy::Enemy(int health, int damage, float scale) : m_health(health), m_damage(damage), m_scale(scale), m_already_hit_player(false)
{
}

void Enemy::initialise(irr::IrrlichtDevice *device, is::IAnimatedMesh *mesh, is::ITriangleSelector *selector, int enemy_id)
{

    auto smgr = device->getSceneManager();
    auto driver = device->getVideoDriver();
    float const X = rand() % 200;
    float const Z = rand() % 200;
    m_device = device;
    m_id = enemy_id;
    m_node = smgr->addAnimatedMeshSceneNode(mesh);
    m_node->setMaterialFlag(iv::EMF_LIGHTING, true);
    m_node->setMD2Animation(is::EMAT_RUN);
    m_node->setMaterialTexture(0, driver->getTexture("data/red_texture.pcx"));
    m_node->setPosition(core::vector3df(X, 0, Z));
    m_node->setScale(core::vector3df(m_scale, m_scale, m_scale));

    ic::vector3df boxMaxEdge = m_node->getTransformedBoundingBox().MaxEdge;
    ic::vector3df boxCenter = m_node->getTransformedBoundingBox().getCenter();
    ic::vector3df ellipseRadius = boxMaxEdge - boxCenter;

    is::ISceneNodeAnimatorCollisionResponse *collision = smgr->createCollisionResponseAnimator(selector, m_node, ellipseRadius);
    m_node->addAnimator(collision);
    collision->drop();
}

void Enemy::update(Player &player, std::vector<Enemy> enemies, EventReceiver *receiver)
{
    if (m_state == IS_DYING)
        return updateDeath();

    if (isBeingAttacked(player, receiver))
        return;
    updateRotation(player);
    if (!isAttacking(player))
        updatePosition(enemies);
    if(m_state == IS_ATTACKING)
        attackPlayer(player);


}

bool Enemy::isAttacking(Player &player)
{
    if(!isAlive()) return false;
    if (m_state == IS_ATTACKING)
    {
        if (m_node->getEndFrame() - m_node->getFrameNr() <= 1)
        {
            m_state = IS_RUNNING;
            m_node->setMD2Animation(is::EMAT_RUN);
            m_already_hit_player = false;
            return false;
        }
        return true;
    }

    ic::vector3df position_player = player.getPosition();
    ic::vector3df position_enemy = m_node->getPosition();
    const float distance = position_player.getDistanceFrom(position_enemy);

    if (distance < 40)
    {
        m_state = IS_ATTACKING;
        m_node->setMD2Animation(is::EMAT_ATTACK);
        return true;
    }
    return false;
}

void Enemy::updatePosition(std::vector<Enemy> enemies)
{
    if (!isAllowedToMove(enemies))
        return;

    float speed_rotation = 2.5f;

    ic::vector3df position_enemy = m_node->getPosition();
    ic::vector3df rotation_enemy = m_node->getRotation();

    position_enemy.X += speed_rotation * cos((rotation_enemy.Y) * M_PI / 180.0);
    position_enemy.Z -= speed_rotation * sin((rotation_enemy.Y) * M_PI / 180.0);

    m_node->setPosition(position_enemy);
    m_last_position = position_enemy;
}

// Enemy will always face player
void Enemy::updateRotation(Player &player)
{
    core::vector3df position_player = player.getPosition();
    core::vector3df position_enemy = m_node->getPosition();
    core::vector3df rotation_enemy = m_node->getRotation();

    core::vector3df position_diff = position_player - position_enemy;
    rotation_enemy.Y = atan(position_diff.Z / position_diff.X) * (180.0f / irr::core::PI);
    if ((position_player.X - position_enemy.X) > 0)
    {
        rotation_enemy.Y = 90 - rotation_enemy.Y;
    }
    else if ((position_player.X - position_enemy.X) < 0)
    {
        rotation_enemy.Y = -90 - rotation_enemy.Y;
    }
    rotation_enemy.Y -= 90;
    m_node->setRotation(rotation_enemy);
}

bool Enemy::isAllowedToMove(std::vector<Enemy> enemies)
{
    ic::vector3df position_enemy = m_node->getPosition();

    for (size_t i = 0; i < enemies.size(); i++)
    {
        Enemy neighbour = enemies[i];
        if (m_id != neighbour.m_id)
        {
            ic::vector3df position_neighbour = neighbour.m_node->getPosition();
            bool isAllowed = true;

            float const distanceX = position_enemy.X - position_neighbour.X;
            float const distanceZ = position_enemy.Z - position_neighbour.Z;

            if (fabs(distanceX) < 5.0)
            {
                position_enemy.X += distanceX > 0 ? 1.0 : -1.0;
                m_node->setPosition(position_enemy);
                isAllowed = false;
            }

            if (fabs(distanceZ) < 5.0)
            {
                position_enemy.Z += distanceZ > 0 ? 1.0 : -1.0;
                m_node->setPosition(position_enemy);
                isAllowed = false;
            }

            if (!isAllowed)
            {
                return false;
            }
        }
    }
    return true;
}

void Enemy::updateDeath()
{

    int time_dying = m_device->getTimer()->getTime() - m_death_time;
    float speedDying = 0.1f;

    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    if (time_dying >= 1500 || m_node->getFrameNr() == m_node->getEndFrame())
    {
        m_state = IS_DEAD;
        m_node->remove();
        m_node = 0;
        return;
    }

    m_last_position = position;
    position.X += speedDying * m_death_dir.X;
    position.Y += speedDying * m_death_dir.Y + 5.0;
    position.Z += speedDying * m_death_dir.Z;

    rotation.X += speedDying * m_death_dir.X;
    rotation.Y += speedDying * m_death_dir.Y;
    rotation.Z += speedDying * m_death_dir.Z;

//    m_node->setPosition(position);
//    m_node->setRotation(rotation);
}

bool Enemy::isDead()
{
    return m_state == IS_DEAD;
}

is::IAnimatedMeshSceneNode *Enemy::getNode()
{
    return m_node;
}

void Enemy::setPosition(ic::vector3df pos)
{
    m_node->setPosition(pos);
}

void Enemy::setOrientation(ic::vector3df ori)
{
    m_node->setRotation(ori);
}

bool Enemy::isBeingAttacked(Player &player, EventReceiver *receiver)
{
    if(!isAlive()) return false;
    if (receiver->states[receiver->KEY_ATTACK])
    {
        ic::vector3df position_player = player.getPosition();
        ic::vector3df position_enemy = m_node->getPosition();
        ic::vector3df rotation_player = player.getRotation();
        ic::vector3df rotation_enemy = m_node->getRotation();
        float const distance = position_player.getDistanceFrom(position_enemy);
        float const angle = sin((rotation_player.Y - rotation_enemy.Y) * M_PI / 180.0);

        // enemy in front of us => angle = -1
        // cone from -0.8 -> -1 <- -0.8
        if (distance < 50.0 && angle <= -0.80f)
        {
            m_death_dir = position_enemy - position_player;
            m_state = IS_DYING;
            m_death_time = m_device->getTimer()->getTime();
            m_node->setMD2Animation(is::EMAT_DEATH_FALLBACK);
            m_node->setLoopMode(false);
            return true;
        }
    }
    return false;
}

bool Enemy::isAlive() {
    return m_state != IS_DYING && m_state != IS_DEAD;
}

void Enemy::attackPlayer(Player &player) {
    if (m_already_hit_player || m_node->getFrameNr() <= 3 * m_node->getEndFrame() / 4.0) //only attack by end of swing to let player avoid/parry
        return;

    ic::vector3df position_player = player.getPosition();
    ic::vector3df position_enemy = m_node->getPosition();
    const float distance = position_player.getDistanceFrom(position_enemy);

    if (distance < 40)
    {
        //player was hit
        m_already_hit_player = true;
        std::cout << "Player got hit" << std::endl;
    }
}
