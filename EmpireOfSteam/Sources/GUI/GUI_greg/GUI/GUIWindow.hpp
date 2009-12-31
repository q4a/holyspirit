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
        virtual sf::Sprite Show();
        virtual void Run();

    protected:
        sf::RenderWindow m_window;
    private:
};

#endif
