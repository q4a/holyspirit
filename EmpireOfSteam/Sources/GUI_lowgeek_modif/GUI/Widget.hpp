#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics.hpp>

class Widget
{
    public:
        Widget();
        virtual ~Widget();
        virtual void Show(sf::RenderWindow &window) = 0;

    protected:

    private:

};

#endif
