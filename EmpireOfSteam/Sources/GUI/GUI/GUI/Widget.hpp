#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics.hpp>
#include "../Global.hpp"

class Widget
{
    public:
        Widget();
        Widget(int, int);
        Widget(int, int, int, int);
        virtual ~Widget();
        virtual sf::Sprite Show();
        virtual void Update();

        void UpdateGlobalPosition();

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetPosition(int x, int y);
        virtual void SetGlobalPosition(int x, int y);

        virtual const sf::Vector2f &GetSize();
        virtual const sf::Vector2f &GetPosition();

        void AddWidget(Widget *widget);

    protected:





        sf::Vector2f m_position;
        sf::Vector2f m_position_global;
        sf::Vector2f m_size;

        std::vector <Widget *> m_widgets;

        sf::RenderImage m_image;
        sf::Sprite      m_sprite;

    private:


};

#endif
