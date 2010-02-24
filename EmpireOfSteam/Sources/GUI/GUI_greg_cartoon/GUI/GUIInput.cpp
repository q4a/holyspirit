#include "GUIInput.hpp"
#include <iostream>
using namespace sf;

GUIInput::GUIInput(std::string label) :  Widget()
{
    m_text = label;
    m_label.SetText(m_text);
    m_label.SetParent(this);

    AddWidget(&m_label);

    m_last_event        = -1;
    m_repetition_time   = 0;
    m_cur_time          = 0;
    m_cur_pos           = m_text.size();

    m_max = 255;
}
GUIInput::GUIInput(int x, int y, std::string label ) : Widget(x, y)
{
    m_text = label;
    m_label.SetText(m_text);
    m_label.SetParent(this);

    AddWidget(&m_label);

    m_last_event        = -1;
    m_repetition_time   = 0;
    m_cur_time          = 0;
    m_cur_pos           = m_text.size();

    m_max = 255;
}
GUIInput::GUIInput(int x, int y, int w, int h, std::string label) :  Widget(x, y, w, h)
{
    m_text = label;
    m_label.SetText(m_text);
    m_label.SetParent(this);

    AddWidget(&m_label);

    m_last_event        = -1;
    m_repetition_time   = 0;
    m_cur_time          = 0;
    m_cur_pos           = m_text.size();

    m_max = 255;
}

GUIInput::~GUIInput()
{
}


void GUIInput::Update()
{
    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    if(mainEventManager->GetEvent(EventMouse, Mouse::Left))
        if(  mainEventManager->GetMousePosition().x > pos.x
         &&  mainEventManager->GetMousePosition().x < pos.x + m_size.x
         &&  mainEventManager->GetMousePosition().y > pos.y
         &&  mainEventManager->GetMousePosition().y < pos.y + m_size.y)
            mainEventManager->SetFocus(this);

    Widget::Update();

    m_repetition_time -= mainEventManager->GetTime();
    if(m_repetition_time < 0)
        m_repetition_time = 0;

    m_cur_time += mainEventManager->GetTime();

    if(m_cur_time > 0.8)
        m_cur_time = 0;

    if(!mainEventManager->GetEvent(EventKey, sf::Key::Back)
     &&!mainEventManager->GetEvent(EventKey, sf::Key::Left)
     &&!mainEventManager->GetEvent(EventKey, sf::Key::Right)
     && mainEventManager->GetChar() < 0 )
        m_last_event = -1;

    if(mainEventManager->GetFocus() == this)
    {
        if(mainEventManager->GetEvent(EventKey, sf::Key::Back) && m_text.size() >= 1 && m_cur_pos > 0)
        {
            if(m_last_event != sf::Key::Back)
            {
                m_text.erase(m_text.begin() + m_cur_pos - 1);
                m_cur_pos--;
                m_repetition_time = 0.5;
            }

            m_last_event = sf::Key::Back;

            if(m_repetition_time <= 0)
            {
                m_text.erase(m_text.begin() + m_cur_pos - 1);
                m_cur_pos--;
                m_repetition_time = 0.1;
            }
        }

        if(mainEventManager->GetEvent(EventKey, sf::Key::Left) && m_cur_pos >= 1)
        {
            if(m_last_event != sf::Key::Left)
            {
                m_cur_pos--;
                m_repetition_time = 0.5;
            }

            m_last_event = sf::Key::Left;

            if(m_repetition_time <= 0)
            {
                m_cur_pos--;
                m_repetition_time = 0.1;
            }
        }

        if(mainEventManager->GetEvent(EventKey, sf::Key::Right) && m_cur_pos < (int)m_text.size())
        {
            if(m_last_event != sf::Key::Right)
            {
                m_cur_pos++;
                m_repetition_time = 0.5;
            }

            m_last_event = sf::Key::Right;

            if(m_repetition_time <= 0)
            {
                m_cur_pos++;
                m_repetition_time = 0.1;
            }
        }

        if(mainEventManager->GetChar() >= 0 && (int)m_text.size() < m_max)
        {
            if(m_last_event != mainEventManager->GetChar())
            {
                m_text.insert(m_cur_pos,1,mainEventManager->GetChar());
                m_cur_pos++;
                m_repetition_time = 0.5;
            }

            m_last_event = mainEventManager->GetChar();

            if(m_repetition_time <= 0)
            {
                m_text.insert(m_cur_pos,1,mainEventManager->GetChar());
                m_cur_pos++;
                m_repetition_time = 0.1;
            }
        }

        if(m_cur_time > 0.4)
        {
            std::string temp = m_text;
            temp.insert(m_cur_pos,1,'|');
            m_label.SetText(temp);
        }
        else
            m_label.SetText(m_text);
    }
    else
        m_label.SetText(m_text);
}

void GUIInput::SetText(std::string text)
{
    m_text = text;
}

void GUIInput::SetMax(int max)
{
    if(max >= 0)
        m_max = max;
}
