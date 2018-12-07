// events.h

#ifndef EVENTS_H
#define EVENTS_H

#define EVENTS_H
#include <vector>

#include <irrlicht.h>

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
  void setNode(irr::scene::ISceneNode *node);
  enum key_id
  {
    STATE_UP,
    STATE_DOWN,
    STATE_ROT_LEFT,
    STATE_ROT_RIGHT,
    STATE_STRAFE_LEFT,
    STATE_STRAFE_RIGHT,
    STATE_MOVING,
    STATE_JUMP,
    STATE_END_ARRAY
  };
  int states[STATE_END_ARRAY] = {0};
};

#endif
