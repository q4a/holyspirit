#include "GUIImage.hpp"
#include <iostream>
using namespace sf;

GUIImage::GUIImage(std::string chemin) : Widget()
{
    m_position  = sf::Vector2i(0,0);
    m_size      = sf::Vector2i(0,0);

    if(!chemin.empty())
        m_img.LoadFromFile(chemin);
    m_drawable = new sf::Sprite();
    m_drawable->SetImage(m_img);
    m_size     = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
GUIImage::GUIImage(int x, int y, std::string chemin) : Widget(x, y)
{
    if(!chemin.empty())
        m_img.LoadFromFile(chemin);
    m_drawable = new sf::Sprite();
    m_drawable->SetImage(m_img);
    m_size     = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);
}
GUIImage::GUIImage(int x, int y, int w, int h, std::string chemin) : Widget(x, y, w, h)
{
    if(!chemin.empty())
        m_img.LoadFromFile(chemin);
    m_drawable = new sf::Sprite();
    m_drawable->SetImage(m_img);
}

GUIImage::~GUIImage()
{
    delete m_drawable;
}

void GUIImage::Show(std::list<sf::Drawable *> &drawables)
{
    drawables.push_back(m_drawable);
    Widget::Show(drawables);
}

void GUIImage::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void GUIImage::SetImage(const sf::Image &img)
{
    m_img = img;
}

void GUIImage::SetImage(const std::string &img)
{
    if(!img.empty())
        m_img.LoadFromFile(img);
}
void GUIImage::Update()
{
    Widget::Update();

    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }
    m_drawable->SetImage(m_img);
    m_drawable->Resize(m_size.x, m_size.y);
    m_drawable->SetPosition(pos.x, pos.y);
}


