// events.h

#ifndef EVENTS_H
#define EVENTS_H

#define EVENTS_H
#include <vector>

#include <irrlicht.h>

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

class EventReceiver : public irr::IEventReceiver
{

  public:
    EventReceiver();
    enum enum_state
    {
        KEY_UP,
        KEY_DOWN,
        KEY_ROT_LEFT,
        KEY_ROT_RIGHT,
        KEY_STRAFE_LEFT,
        KEY_STRAFE_RIGHT,
        KEY_ATTACK,
        KEY_BLOCK,
        KEY_DEBUG_TRIGGER_SPAWN,
        KEY_DEBUG_SWORD_EFFECT,
        STATE_MOVING,
        STATE_ATTACKING,
        END_ARRAY
    };

    bool *getStates();

  private:
    bool OnEvent(const irr::SEvent &event);
    bool keyboard(const irr::SEvent &event);
    bool mouse(const irr::SEvent &event);
    bool m_states[END_ARRAY] = {false};
};

#endif
