#ifndef MENUPANEL_HPP
#define MENUPANEL_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"
#include "GUI/GUIProgressBar.hpp"
#include "GUI/TabBar.hpp"
#include "GUI/Label.hpp"
#include "GUI/GUIImage.hpp"
#include "GUI/GUIAnimatedImage.hpp"
#include "GUI/GUIWindow.hpp"
#include "GUI/GUIInput.hpp"

class MenuPanel : public Widget
{
    public:
        MenuPanel();
        ~MenuPanel();
        void Update();

    private:
        GUIImage *m_background,
                 *m_deco,
                 *m_iris,
                 *m_iris_open,
                 *m_login;

        Button   *m_iris_button;

        GUIInput *m_login_input,
                 *m_pass_input;
};

#endif

