#ifndef WINDOWGAME_HPP
#define WINDOWGAME_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"

class WindowGame
{
    public:
        WindowGame();
        ~WindowGame();
        void Run();

    protected:

    private:
        sf::RenderWindow m_window;
        Button m_button;
};

#endif
