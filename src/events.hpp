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
  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);

public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  enum key_id
  {
    KEY_UP,
    KEY_DOWN,
    KEY_ROT_LEFT,
    KEY_ROT_RIGHT,
    KEY_STRAFE_LEFT,
    KEY_STRAFE_RIGHT,
    KEY_ATTACK,
    STATE_MOVING,
    STATE_ATTACKING,
    END_ARRAY
  };
  int states[END_ARRAY] = {false};
};

#endif
