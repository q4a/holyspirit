#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <SFML/Graphics.hpp>
#include "../Global.hpp"
#include <list>

class Widget
{
    public:
        Widget();
        Widget(int, int);
        Widget(int, int, int, int);
        virtual ~Widget();

        virtual void Show(std::list<sf::Drawable*> &drawables);
        virtual void            Update();

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetPosition(int x, int y);

        virtual void SetImage(const sf::Image &);
        virtual void SetImage(const std::string &);

        virtual const sf::Vector2i &GetSize();
        virtual const sf::Vector2i &GetPosition();
        virtual       sf::Vector2i  GetGlobalPosition();

                       void         SetParent(Widget *widget);

        virtual        void         AddWidget(Widget *widget);

    protected:

        sf::Vector2i m_position;
        sf::Vector2i m_size;

        sf::Image m_background;

        std::vector <Widget *> m_widgets;

        Widget*         m_parent;

        sf::Drawable   *m_drawable;

    private:


};

#endif
