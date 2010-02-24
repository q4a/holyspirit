
#include "RessourceBar.hpp"
#include <iostream>
#include <sstream>
using namespace sf;

RessourceBar::RessourceBar(std::string label) :  GUIProgressBar()
{
    m_img_jauge_left    .SetImage("pictures/GUI/jaugeGauche.png");
    m_img_jauge_left    .SetGeometry(-116, -32, 288, 69);
    m_img_jauge_right   .SetImage("pictures/GUI/jaugeDroite.png");
    m_img_jauge_right   .SetGeometry(-108, -32, 288, 69);
    m_value_label       .SetPosition(256,0);

    m_img_jauge_left    .SetParent(this);
    m_img_jauge_right   .SetParent(this);
    m_value_label       .SetParent(this);
}
RessourceBar::RessourceBar(int x, int y, std::string label ) : GUIProgressBar(x, y, label)
{
    m_img_jauge_left    .SetImage("pictures/GUI/jaugeGauche.png");
    m_img_jauge_left    .SetGeometry(-116, -32, 288, 69);
    m_img_jauge_right   .SetImage("pictures/GUI/jaugeDroite.png");
    m_img_jauge_right   .SetGeometry(-108, -32, 288, 69);
    m_value_label       .SetPosition(256,0);

    m_img_jauge_left    .SetParent(this);
    m_img_jauge_right   .SetParent(this);
    m_value_label       .SetParent(this);
}
RessourceBar::RessourceBar(int x, int y, int w, int h, std::string label) :  GUIProgressBar(x, y, w, h, label)
{
    m_img_jauge_left    .SetImage("pictures/GUI/jaugeGauche.png");
    m_img_jauge_left    .SetGeometry(-116, -32, 288, 69);
    m_img_jauge_right   .SetImage("pictures/GUI/jaugeDroite.png");
    m_img_jauge_right   .SetGeometry(-108, -32, 288, 69);
    m_value_label       .SetPosition(192,0);

    m_img_jauge_left    .SetParent(this);
    m_img_jauge_right   .SetParent(this);
    m_value_label       .SetParent(this);
}

RessourceBar::~RessourceBar()
{

}

void RessourceBar::Show(std::list<sf::Drawable *> &drawables)
{
    GUIProgressBar::Show(drawables);

    m_img_jauge_left.Show(drawables);
    m_img_jauge_right.Show(drawables);
    m_value_label.Show(drawables);
}

void RessourceBar::Update()
{
    GUIProgressBar::Update();

    std::ostringstream buf;
    buf<<m_value<<"\\"<<m_max;
    m_value_label.SetText(buf.str());

    m_value_label.Update();
    m_img_jauge_left.Update();
    m_img_jauge_right.Update();
}

