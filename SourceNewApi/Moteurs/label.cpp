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
        if(m_clock.GetElapsedTime() < 2000)
        {
            moteurGraphique->special_typo_h.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,min((float)m_clock.GetElapsedTime(),1000.f)*255/1000));
        }
        else if(m_clock.GetElapsedTime() < 4000)
        {
            moteurGraphique->special_typo_h.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,max(0.f, min(3000.f-(float)m_clock.GetElapsedTime(),1000.f))*255/1000));
            moteurGraphique->special_typo_p.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,min((float)m_clock.GetElapsedTime()-2000.f,1000.f)*255/1000));
        }
        else if(m_clock.GetElapsedTime() < 5000)
            moteurGraphique->special_typo_p.Draw( m_text, sf::Vector2f( configuration->Resolution.w/2,
                                                                        configuration->Resolution.h/2-128),
                                                48, 19, true, sf::Color(255,255,255,(5000 - m_clock.GetElapsedTime())*255/1000));
        else
            m_draw = false;
    }
}

void Label::Reset()
{
    m_draw = true;
    m_clock.Reset();
}

void Label::SetText(std::string t)
{
    m_text = t;
}
