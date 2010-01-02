#ifndef WINDOWGAME_HPP
#define WINDOWGAME_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"
#include "GUI/Label.hpp"
#include "GUI/GUIImage.hpp"
#include "GUI/GUIWindow.hpp"

class WindowGame : public GUIWindow
{
    public:
        WindowGame();
        ~WindowGame();
        virtual void Run();

    protected:

    private:
        Button      *m_button,
                    *m_button_2,
                    *m_button_3,
                    *m_button_in,
                    *m_button_in_in;

        Label       *m_label;

        GUIImage    *m_panel_ressources,
                    *m_panel_map,
                    *m_panel_bejeweled,
                    *m_panel_city;

        GUIImage    *m_cadran_bejeweled,
                    *m_cadran_map,
                    *m_map;

};

#endif
