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
  irr::scene::ISceneNode *node;
  bool button_pressed;
  int old_x, old_y;
  std::vector<irr::video::ITexture *> textures;
  int current_texture;

  bool keyboard(const irr::SEvent &event);
  bool mouse(const irr::SEvent &event);

public:
  EventReceiver();
  bool OnEvent(const irr::SEvent &event);
  void setNode(is::ISceneNode *node);
  enum key_id
  {
    KEY_UP,
    KEY_DOWN,
    KEY_ROT_LEFT,
    KEY_ROT_RIGHT,
    KEY_STRAFE_LEFT,
    KEY_STRAFE_RIGHT,
    KEY_ATTACK,
    KEY_DEBUG_TRIGGER_SPAWN,
    STATE_MOVING,
    STATE_ATTACKING,
    END_ARRAY
  };
  int states[END_ARRAY] = {false};
};

#endif
