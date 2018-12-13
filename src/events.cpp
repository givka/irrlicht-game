#include <irrlicht.h>

#include "events.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

EventReceiver::EventReceiver()
{
}

bool EventReceiver::keyboard(const SEvent &event)
{
    switch (event.KeyInput.Key)
    {
    case KEY_ESCAPE:
        exit(0);
    case KEY_KEY_Z:
        m_states[KEY_UP] = event.KeyInput.PressedDown;
        break;
    case KEY_KEY_S:
        m_states[KEY_DOWN] = event.KeyInput.PressedDown;
        break;
    case KEY_KEY_Q:
        m_states[KEY_STRAFE_LEFT] = event.KeyInput.PressedDown;
        break;
    case KEY_KEY_D:
        m_states[KEY_STRAFE_RIGHT] = event.KeyInput.PressedDown;
        break;
    case KEY_SPACE:
        break;
    case KEY_KEY_R:
        m_states[KEY_DEBUG_TRIGGER_SPAWN] = event.KeyInput.PressedDown;
        break;
    default:
        break;
    }

    return false;
}
bool EventReceiver::mouse(const SEvent &event)
{
    switch (event.MouseInput.Event)
    {
    case EMIE_LMOUSE_PRESSED_DOWN:
        m_states[KEY_ATTACK] = true;
        break;
    case EMIE_LMOUSE_LEFT_UP:
        m_states[KEY_ATTACK] = false;
        break;
    default:;
    }

    return false;
}
bool EventReceiver::OnEvent(const SEvent &event)
{

    switch (event.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        return keyboard(event);
    case EET_MOUSE_INPUT_EVENT:
        return mouse(event);
    default:;
    }
    return false;
}

bool *EventReceiver::getStates()
{
    return m_states;
}
