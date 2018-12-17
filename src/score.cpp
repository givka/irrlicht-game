#include "score.hpp"

Score::Score(irr::IrrlichtDevice *device)
    : m_device(device)
{
    iv::IVideoDriver *driver = device->getVideoDriver();
    ig::IGUIEnvironment *gui = device->getGUIEnvironment();

    m_digits[0] = driver->getTexture("data/numbers/0.png");
    m_digits[1] = driver->getTexture("data/numbers/1.png");
    m_digits[2] = driver->getTexture("data/numbers/2.png");
    m_digits[3] = driver->getTexture("data/numbers/3.png");
    m_digits[4] = driver->getTexture("data/numbers/4.png");
    m_digits[5] = driver->getTexture("data/numbers/5.png");
    m_digits[6] = driver->getTexture("data/numbers/6.png");
    m_digits[7] = driver->getTexture("data/numbers/7.png");
    m_digits[8] = driver->getTexture("data/numbers/8.png");
    m_digits[9] = driver->getTexture("data/numbers/9.png");

    irr::core::dimension2df size = (irr::core::dimension2df)device->getVideoDriver()->getScreenSize();
    size *= 0.025;
}

void Score::update(int score)
{
    // remove old images
    for (size_t i = 0; i < m_images.size(); i++)
    {
        m_images[i]->remove();
        m_images[i] = 0;
    }
    m_images.clear();

    ig::IGUIEnvironment *gui = m_device->getGUIEnvironment();
    irr::core::dimension2df size = (irr::core::dimension2df)m_device->getVideoDriver()->getScreenSize();

    int width = 0.01 * size.Width;
    int height = 0.025 * size.Height;
    bool delete_number = false;
    bool stop_deleting = false;

    for (size_t i = 0; i < MAX_NUMBER_OF_NUMBERS; i++)
    {
        int power_of_ten = pow(10, MAX_NUMBER_OF_NUMBERS - 1 - i);

        if (!stop_deleting)
        {
            delete_number = ((score / power_of_ten) % 10) == 0;
            if (delete_number && i != MAX_NUMBER_OF_NUMBERS - 1)
                continue;
        }
        stop_deleting = true;

        ig::IGUIImage *image = gui->addImage(
            ic::rect<irr::s32>(10 + i * width, 10, 10 + i * width + width, 2 * height));
        image->setImage(m_digits[(score / power_of_ten) % 10]);
        image->setScaleImage(true);
        m_images.push_back(image);
    }
}