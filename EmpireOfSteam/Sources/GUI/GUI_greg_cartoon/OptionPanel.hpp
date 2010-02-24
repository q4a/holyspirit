#ifndef OPTIONPANEL_HPP
#define OPTIONPANEL_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"
#include "GUI/GUIProgressBar.hpp"
#include "GUI/TabBar.hpp"
#include "GUI/Label.hpp"
#include "GUI/GUIImage.hpp"
#include "GUI/GUIAnimatedImage.hpp"
#include "GUI/GUIWindow.hpp"

class OptionPanel : public Widget
{
    public:
        OptionPanel();
        ~OptionPanel();
        void Update();

    private:
        GUIImage    *m_background,
                    *m_menu,
                    *m_screen;
};

#endif


