
#include "CustomisationPanel.hpp"

CustomisationPanel::CustomisationPanel()
{
    m_background    = new GUIImage("pictures/GUI/customBackground.png");
    m_deco    = new GUIImage(48,16,"pictures/GUI/customDeco.png");
    AddWidget(m_background);
    AddWidget(m_deco);
}

CustomisationPanel::~CustomisationPanel()
{
    delete m_background;
    delete m_deco;
}

void CustomisationPanel::Update()
{
    Widget::Update();

}


