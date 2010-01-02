
#include "Label.hpp"
#include <iostream>

using namespace sf;

Label::Label() : Widget(), m_size(12)
{
    m_drawable = new sf::Text;
}

Label::Label(std::string text) : Widget(), m_size(12)
{
    m_drawable = new sf::Text;
    m_text = text;
}

Label::Label(int x, int y, std::string text) : Widget(x, y), m_size(12)
{
    m_drawable = new sf::Text;
    m_text = text;
}

Label::Label(int x, int y, int w, int h, std::string text) : Widget(x, y, w, h), m_size(12)
{
    m_drawable = new sf::Text;
    m_text = text;
}

Label::~Label()
{
    delete m_drawable;
}

void Label::Update()
{
    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    m_drawable->SetPosition(pos.x, pos.y);
    m_drawable->SetString          (m_text);
    m_drawable->SetCharacterSize   (m_size);
    m_drawable->SetColor           (m_color);
}

void Label::Show(std::list<sf::Drawable *> &drawables)
{
    drawables.push_back(m_drawable);
    Widget::Show(drawables);
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

