#ifndef CURSOR_H
#define CURSOR_H

#include "irrlicht.h"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Cursor
{
  public:
    Cursor(irr::IrrlichtDevice *device);
    void update();
    void setVisible(bool);
  private:
    irr::IrrlichtDevice *m_device;
    ig::IGUIImage *m_image = 0;
};

#endif
