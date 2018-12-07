#include <irrlicht.h>

#include "events.hpp"

using namespace irr;

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;

EventReceiver::EventReceiver()
    : node(nullptr), button_pressed(false)
{
}

bool EventReceiver::keyboard(const SEvent &event)
{
  switch (event.KeyInput.Key)
  {
  case KEY_ESCAPE:
    exit(0);
  case KEY_KEY_Z:
    states[STATE_UP] = event.KeyInput.PressedDown;
    break;
  case KEY_KEY_S:
    states[STATE_DOWN] = event.KeyInput.PressedDown;
    break;
  case KEY_KEY_A:
    states[STATE_ROT_LEFT] = event.KeyInput.PressedDown;
    break;
  case KEY_KEY_E:
    states[STATE_ROT_RIGHT] = event.KeyInput.PressedDown;
    break;
  case KEY_KEY_Q:
    states[STATE_STRAFE_LEFT] = event.KeyInput.PressedDown;
    break;
  case KEY_KEY_D:
    states[STATE_STRAFE_RIGHT] = event.KeyInput.PressedDown;
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
  case EMIE_RMOUSE_PRESSED_DOWN:
    button_pressed = true;
    old_x = event.MouseInput.X;
    old_y = event.MouseInput.Y;
    break;
  case EMIE_RMOUSE_LEFT_UP:
    button_pressed = false;
    break;
  case EMIE_MOUSE_MOVED:
    if (button_pressed)
    {
      ic::vector3df rotation = node->getRotation();
      rotation.Y += (event.MouseInput.X - old_x);
      old_x = event.MouseInput.X;
      old_y = event.MouseInput.Y;
      node->setRotation(rotation);
    }
    break;
  default:;
  }

  return false;
}

bool EventReceiver::OnEvent(const SEvent &event)
{
  if (!node)
    return false;
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

void EventReceiver::setNode(irr::scene::ISceneNode *n)
{
  node = n;
}
