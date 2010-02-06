#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Widget.hpp"

class Button : public Widget
{
    public:
        Button();
        Button(int, int);
        Button(int, int, int, int);
        virtual ~Button();
        virtual sf::Sprite Show();
        virtual void Update();
        virtual void SetGeometry(int x, int y, int w, int h);
        bool Hover();
        bool Clicked();
        bool Released();

    protected:
        bool m_hover;
        bool m_clicked;
        bool m_released;

    private:
        sf::Image m_IbuttonForm;
};

#endif
