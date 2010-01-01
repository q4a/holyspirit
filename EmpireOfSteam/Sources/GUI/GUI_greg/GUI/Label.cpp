
#include "Label.hpp"
#include <iostream>

using namespace sf;

Label::Label() : Widget(), m_size(12)
{
}

Label::Label(std::string text) : Widget(), m_size(12)
{
    m_text = text;
}

Label::Label(int x, int y, std::string text) : Widget(x, y), m_size(12)
{
    m_text = text;
}

Label::Label(int x, int y, int w, int h, std::string text) : Widget(x, y, w, h), m_size(12)
{
    m_text = text;
}

Label::~Label()
{
}

sf::Sprite Label::Show()
{
    m_image.Clear();

    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            m_image.Draw((*i)->Show());

    sf::Text text;

    text.SetString          (m_text);
    text.SetCharacterSize   (m_size);
    text.SetColor           (m_color);

    m_image.Draw(text);

    m_image.Display();

    m_sprite.SetImage(m_image.GetImage());
    return  (m_sprite);
}

void Label::SetText(std::string text)
{
    m_text = text;
}

void Label::SetColor(sf::Color color)
{
    m_color = color;
}

void Label::SetCharacterSize(int size)
{
    m_size = size;
}


const std::string &Label::GetText()
{
    return (m_text);
}

const sf::Color &Label::GetColor()
{
    return (m_color);
}

const int &Label::GetCharacterSize()
{
    return (m_size);
}

