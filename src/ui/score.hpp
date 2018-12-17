#ifndef SCORE_HPP
#define SCORE_HPP

#include <irrlicht.h>
#include <iostream>
#include <vector>
#include "../utils.hpp"

namespace ic = irr::core;
namespace is = irr::scene;
namespace iv = irr::video;
namespace ig = irr::gui;

class Score
{
  public:
    Score(irr::IrrlichtDevice *device, float x, float y, float scale);
    void update(int score);

  private:
    irr::IrrlichtDevice *m_device;
    int m_score;
    int MAX_NUMBER_OF_NUMBERS = 10;
    iv::ITexture *m_digits[10];

    float m_x;
    float m_y;
    float m_scale;

    std::vector<ig::IGUIImage *> m_images;
};

#endif //SCORE_HPP
