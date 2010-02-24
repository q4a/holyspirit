#ifndef CUSTOMISATIONPANEL_HPP
#define CUSTOMISATIONPANEL_HPP

#include "Global.hpp"
#include "GUI/Button.hpp"
#include "GUI/GUIProgressBar.hpp"
#include "GUI/TabBar.hpp"
#include "GUI/Label.hpp"
#include "GUI/GUIImage.hpp"
#include "GUI/GUIAnimatedImage.hpp"
#include "GUI/GUIWindow.hpp"

class CustomisationPanel : public Widget
{
    public:
        CustomisationPanel();
        ~CustomisationPanel();
        void Update();

    private:
        GUIImage    *m_background,
                    *m_deco;
};

#endif


