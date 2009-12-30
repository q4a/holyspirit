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
        virtual void SetGeometry(int x, int y, int w, int h);
        bool Clicked();

    protected:

    private:
        sf::Image m_IbuttonForm;
};

#endif
