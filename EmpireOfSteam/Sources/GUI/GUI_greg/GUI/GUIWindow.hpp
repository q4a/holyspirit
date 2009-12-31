#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../Global.hpp"
#include "Widget.hpp"

class GUIWindow : public Widget
{
    public:
        GUIWindow();
        GUIWindow(int , int , std::string title = "");
        virtual ~GUIWindow();
        //void AddWidget(Widget *widget);
        virtual sf::Sprite Show();
        //void Update();

        virtual void Run();

    protected:

    private:
        sf::RenderWindow m_window;
        //std::vector <Widget *> m_widgets;
};

#endif
