#include "label.h"
#include "../globale.h"

#include <fstream>

using namespace std;

Label::Label()
{
    m_draw = false;
}

void Label::Draw()
{
    if(m_draw)
    {
        if(m_clock.getElapsedTime().asSeconds() < 2)
        {
            moteurGraphique->special_typo_h.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,min((float)m_clock.getElapsedTime().asSeconds(),1.f)*255));
        }
        else if(m_clock.getElapsedTime().asSeconds() < 4)
        {
            moteurGraphique->special_typo_h.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,max(0.f, min(3.f-(float)m_clock.getElapsedTime().asSeconds(),1.f))*255));
            moteurGraphique->special_typo_p.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,min((float)m_clock.getElapsedTime().asSeconds()-2.f,1.f)*255));
        }
        else if(m_clock.getElapsedTime().asSeconds() < 5)
            moteurGraphique->special_typo_p.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,(5.f - m_clock.getElapsedTime().asSeconds())*255));
        else
            m_draw = false;
    }
}

void Label::Reset()
{
    m_draw = true;
    m_clock.restart();
}

void Label::SetText(std::string t)
{
    m_text = t;
}
