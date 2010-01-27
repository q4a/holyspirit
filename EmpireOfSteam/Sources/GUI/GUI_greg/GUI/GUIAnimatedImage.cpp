
#include "GUIAnimatedImage.hpp"
#include <iostream>
#include <sstream>
using namespace sf;

GUIAnimatedImage::GUIAnimatedImage(int nbr, std::string chemin) : GUIImage()
{
    m_position  = sf::Vector2i(0,0);
    m_size      = sf::Vector2i(0,0);

    m_drawable = new sf::Sprite();
    m_size     = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);

    SetImage(nbr, chemin);

    m_number_frame = 0;
    m_time_frame   = 0;
    m_framerate    = 0.1;
}
GUIAnimatedImage::GUIAnimatedImage(int x, int y, int nbr, std::string chemin) : GUIImage(x, y)
{
    m_drawable = new sf::Sprite();
    m_size     = sf::Vector2i((int)m_drawable->GetSize().x, (int)m_drawable->GetSize().y);

    SetImage(nbr, chemin);

    m_number_frame = 0;
    m_time_frame   = 0;
    m_framerate    = 0.1;
}
GUIAnimatedImage::GUIAnimatedImage(int x, int y, int w, int h, int nbr, std::string chemin) : GUIImage(x, y, w, h)
{
    m_drawable = new sf::Sprite();

    SetImage(nbr, chemin);

    m_number_frame = 0;
    m_time_frame   = 0;
    m_framerate    = 0.1;
}

GUIAnimatedImage::~GUIAnimatedImage()
{
    delete m_drawable;
}

void GUIAnimatedImage::Show(std::list<sf::Drawable *> &drawables)
{
    drawables.push_back(m_drawable);
    GUIImage::Show(drawables);
}

void GUIAnimatedImage::SetImage(int nbr, const std::string &img)
{
    for(int i = 1 ; i <= nbr ; i ++)
    {
        std::ostringstream buf;

        for(int j = 0 ; j < img.size() ; ++j)
        {
            if(img[j] == '.')
                buf<<i;
            buf<<img[j];
        }

        m_list_img.push_back(sf::Image ());
        m_list_img.back().LoadFromFile(buf.str());
    }

}

void GUIAnimatedImage::Update()
{
    GUIImage::Update();

    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    m_time_frame += mainEventManager->GetTime();

    if(m_time_frame >= m_framerate)
    {
        m_number_frame++;
        if(m_number_frame >= m_list_img.size())
            m_number_frame = 0;
        m_time_frame = 0;
        m_drawable->SetImage(m_list_img[m_number_frame]);
    }

    m_drawable->Resize(m_size.x, m_size.y);
    m_drawable->SetPosition(pos.x, pos.y);
}


