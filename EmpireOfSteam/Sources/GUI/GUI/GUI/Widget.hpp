#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics.hpp>
#include "../Global.hpp"

class Widget
{
    public:
        Widget();
        virtual ~Widget();
        virtual void Show(sf::RenderWindow &window) = 0;
        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetPosition(int x, int y);

    protected:
        int m_x;
        int m_y;
        int m_width;
        int m_height;

    private:


};

#endif
