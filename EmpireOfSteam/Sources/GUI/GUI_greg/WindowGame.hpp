#ifndef WINDOWGAME_HPP
#define WINDOWGAME_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"
#include "GUI/GUIWindow.hpp"

class WindowGame : public GUIWindow
{
    public:
        WindowGame();
        ~WindowGame();
        virtual void Run();

    protected:

    private:
        sf::RenderWindow m_window;
        Button *m_button, *m_button_2, *m_button_3, *m_button_in,  *m_button_in_in;

        std::vector <Widget *> m_widgets;
};

#endif
