#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"

class Button : public Widget
{
    public:
        Button();
        virtual ~Button();
        virtual void Show(sf::RenderWindow &window);
        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetPosition(int x, int y);
        bool Clicked();

    protected:

    private:
        sf::Image m_IbuttonForm;
        sf::Sprite m_buttonForm;
        sf::Sprite m_buttonImage;
};

#endif
