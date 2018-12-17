#ifndef SCORE_HPP
#define SCORE_HPP

#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "events.hpp"
#include "utils.hpp"
#include "sword.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Score
{
  public:
    Score(irr::IrrlichtDevice *device);
    void update(int score);

  private:
    irr::IrrlichtDevice *m_device;
    int m_score;
    int MAX_NUMBER_OF_NUMBERS = 10;
    iv::ITexture *m_digits[10];

    std::vector<ig::IGUIImage *> m_images;
};

#endif //SCORE_HPP
