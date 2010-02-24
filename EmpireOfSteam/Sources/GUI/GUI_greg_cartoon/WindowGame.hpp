#ifndef WINDOWGAME_HPP
#define WINDOWGAME_HPP

#include "GamePanel.hpp"
#include "MenuPanel.hpp"
#include "OptionPanel.hpp"
#include "CustomisationPanel.hpp"

enum {MENU, GAME, OPTION, CUSTOMISATION};

class WindowGame : public GUIWindow
{
    public:
        WindowGame();
        ~WindowGame();
        virtual void Run();

    protected:

    private:
        GamePanel           m_game;
        MenuPanel           m_menu;
        OptionPanel         m_option;
        CustomisationPanel  m_customisation;

        sf::Vector2f        m_view;

        int m_screen;
};

#endif
