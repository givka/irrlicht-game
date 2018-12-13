#include <irrlicht.h>

#include "sword.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

Sword::Sword()
{
}

void Sword::initialise(irr::IrrlichtDevice *device, is::ICameraSceneNode *nodePlayer)
{
    m_device = device;
    is::ISceneManager *smgr = device->getSceneManager();
    iv::IVideoDriver *driver = device->getVideoDriver();

    m_node = smgr->addMeshSceneNode(smgr->getMesh("data/sword.obj"), nodePlayer);
    m_node->setMaterialFlag(iv::EMF_LIGHTING, true);
    m_node->setMaterialTexture(0, driver->getTexture("data/sword_texture.jpg"));
    m_node->setPosition(core::vector3df(15, -10, 10));
    m_node->setRotation(core::vector3df(90, -10, 90));
    setEnchantment(m_current_enchant);
}

void Sword::setEnchantment(enchant new_enchantment)
{

    is::IParticleSystemSceneNode *ps1 = setParticuleSystem(new_enchantment, ic::vector3df(5, 0, 0));
    is::IParticleSystemSceneNode *ps2 = setParticuleSystem(new_enchantment, ic::vector3df(10, 0, 0));
    is::IParticleSystemSceneNode *ps3 = setParticuleSystem(new_enchantment, ic::vector3df(15, 0, 0));
    is::IParticleSystemSceneNode *ps4 = setParticuleSystem(new_enchantment, ic::vector3df(20, 0, 0));
    is::IParticleSystemSceneNode *ps5 = setParticuleSystem(new_enchantment, ic::vector3df(25, 0, 0));
}

is::IParticleSystemSceneNode *Sword::setParticuleSystem(enchant new_enchantment, ic::vector3df position)
{
    is::ISceneManager *smgr = m_device->getSceneManager();
    iv::IVideoDriver *driver = m_device->getVideoDriver();

    scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(false, m_node);

    scene::IParticleEmitter *em = ps->createBoxEmitter(
        core::aabbox3d<f32>(-1, -1, -1, 1, 1, 1),
        core::vector3df(0.0f, 0.1f, 0.0f),
        500,
        500,
        irr::video::SColor(255, 0, 0, 0),
        irr::video::SColor(255, 255, 50, 10),
        0,
        0,
        40,
        core::dimension2d<f32>(10.0f, 10.0f),
        core::dimension2d<f32>(10.0f, 10.0f));

    ps->setEmitter(em);
    em->drop();

    ps->setMaterialFlag(video::EMF_LIGHTING, false);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialTexture(0, driver->getTexture("data/smoke.jpg"));
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    ps->setPosition(position);

    return ps;
}

void Sword::setAttack()
{
    if (isAttacking)
        return;
    isAttacking = true;
    m_sword_going_down = true;
    float X = 0;
    float Y = -100;
    float Z = std::rand() % 2 ? -5 : 5;
    m_destination = ic::vector3df(X, Y, Z);
}

void Sword::updatePosition()
{
    float speed = 10.0;
    if (!isAttacking)
        return;

    ic::vector3df position = m_node->getPosition();
    ic::vector3df rotation = m_node->getRotation();

    position.Z += m_sword_going_down ? 1 : -1;

    rotation.Y += m_sword_going_down ? -speed : speed;
    rotation.Z += m_sword_going_down ? -m_destination.Z : m_destination.Z;

    if (rotation.Y < m_destination.Y)
        m_sword_going_down = false;

    if (!m_sword_going_down && rotation.Y >= -10)
        isAttacking = false;

    m_node->setPosition(position);
    m_node->setRotation(rotation);
}