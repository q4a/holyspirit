
#include "Bejeweled.hpp"
#include <iostream>
#include <sstream>
using namespace sf;


Bejeweled::Bejeweled(int x, int y) : Widget(x, y)
{
    for(int i = 1 ; i <= NBR_JEWELS ; ++i)
    {
        std::ostringstream buf;
        buf<<"pictures/Bejeweled/diamant_"<<i<<".png";
        m_jewelds[i-1].LoadFromFile(buf.str());
    }

    m_gear.LoadFromFile("pictures/Bejeweled/gear.png");
    m_sprite_gear.SetImage(m_gear);
    m_sprite_gear.SetOrigin(45,45);

    m_go_rotation           = false;
    m_gear_rotation         = 0;
    m_gear_rotation_total   = 0;

    for(int x = 0 ; x < TAB_WIDTH ; ++x)
        for(int y = 0 ; y < TAB_HEIGHT ; ++y)
        {
            m_tab_jewelds[x][y].m_type = rand() % NBR_JEWELS;
            m_tab_jewelds[x][y].m_position.x = x * 32;
            m_tab_jewelds[x][y].m_position.y = y * 32;
            m_tab_jewelds[x][y].m_tab_position.x = x;
            m_tab_jewelds[x][y].m_tab_position.y = y;
            m_tab_jewelds[x][y].m_sprite.SetImage(m_jewelds[m_tab_jewelds[x][y].m_type]);
            m_tab_jewelds[x][y].m_sprite.SetOrigin(16,16);
        }

    m_drawable = new sf::Sprite();

    CheckAlign(true);
}
Bejeweled::Bejeweled(int x, int y, int w, int h) : Widget(x, y, w, h)
{
    for(int i = 1 ; i <= NBR_JEWELS ; ++i)
    {
        std::ostringstream buf;
        buf<<"pictures/Bejeweled/diamant_"<<i<<".png";
        m_jewelds[i-1].LoadFromFile(buf.str());
    }

    m_go_rotation           = false;
    m_gear_rotation         = 0;
    m_gear_rotation_total   = 0;

    for(int x = 0 ; x < TAB_WIDTH ; ++x)
        for(int y = 0 ; y < TAB_HEIGHT ; ++y)
        {
            m_tab_jewelds[x][y].m_type = rand() % NBR_JEWELS;
            m_tab_jewelds[x][y].m_position.x = x * 32;
            m_tab_jewelds[x][y].m_position.y = y * 32;
            m_tab_jewelds[x][y].m_tab_position.x = x;
            m_tab_jewelds[x][y].m_tab_position.y = y;
            m_tab_jewelds[x][y].m_sprite.SetImage(m_jewelds[m_tab_jewelds[x][y].m_type]);
            m_tab_jewelds[x][y].m_sprite.SetOrigin(16,16);
        }
    m_gear.LoadFromFile("pictures/Bejeweled/gear.png");
    m_sprite_gear.SetImage(m_gear);
    m_sprite_gear.SetOrigin(45,45);

    m_drawable = new sf::Sprite();

    CheckAlign(true);
}

Bejeweled::~Bejeweled()
{
    delete m_drawable;
}

void Bejeweled::Show(std::list<sf::Drawable *> &drawables)
{
    for(int x = 0 ; x < TAB_WIDTH ; ++x)
        for(int y = 0 ; y < TAB_HEIGHT ; ++y)
            drawables.push_back(&m_tab_jewelds[x][y].m_sprite);

    drawables.push_back(&m_sprite_gear);

    for(std::vector<sf::Sprite>::iterator i = m_jeweleds_destruction.begin();
        i != m_jeweleds_destruction.end() ; ++i)
        drawables.push_back(&(*i));
    //drawables.push_back(m_drawable);
    Widget::Show(drawables);
}

void Bejeweled::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

int Bejeweled::GetLastJeweld()
{
    int value = -1;

    if(!m_lasts_jewelds.empty())
    {
        value = m_lasts_jewelds.front();
        m_lasts_jewelds.erase(m_lasts_jewelds.begin());
    }

    return (value);
}

void Bejeweled::Update()
{
    Widget::Update();

    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    if(!m_go_rotation)
    {
        m_position_gear.x = (mainEventManager->GetMousePosition().x - pos.x - 12 ) / 32;
        m_position_gear.y = (mainEventManager->GetMousePosition().y - pos.y - 12 ) / 32;
    }

    if(m_position_gear.x < 0)
        m_position_gear.x = 0;
    if(m_position_gear.y < 0)
        m_position_gear.y = 0;
    if(m_position_gear.x > TAB_WIDTH - 2)
        m_position_gear.x = TAB_WIDTH - 2;
    if(m_position_gear.y > TAB_HEIGHT - 2)
        m_position_gear.y = TAB_HEIGHT - 2;


    m_sprite_gear.SetRotation(m_gear_rotation_total + m_gear_rotation);

    if(m_sprite_gear.GetPosition().x == 0 && m_sprite_gear.GetPosition().y == 0)
        m_sprite_gear.SetPosition(pos.x + 64 - 12, pos.y + 64 - 12);

    m_sprite_gear.SetPosition(m_position_gear.x * 32 + pos.x + 32, m_position_gear.y * 32 + pos.y + 32);

    if(mainEventManager->GetMousePosition().x >= pos.x
    && mainEventManager->GetMousePosition().y >= pos.y
    && mainEventManager->GetMousePosition().x <= pos.x + m_size.x
    && mainEventManager->GetMousePosition().y <= pos.y + m_size.y)
    {
        mainEventManager->ShowCursor(false);
        if(mainEventManager->GetEvent(EventMouse, sf::Mouse::Left))
        {
            if(!m_go_rotation)
            {
                Jeweld buf = m_tab_jewelds[m_position_gear.x][m_position_gear.y];

                m_tab_jewelds[m_position_gear.x]     [m_position_gear.y]         = m_tab_jewelds[m_position_gear.x]    [m_position_gear.y + 1];

                m_tab_jewelds[m_position_gear.x]     [m_position_gear.y + 1]     = m_tab_jewelds[m_position_gear.x + 1][m_position_gear.y + 1];

                m_tab_jewelds[m_position_gear.x + 1] [m_position_gear.y + 1]     = m_tab_jewelds[m_position_gear.x + 1][m_position_gear.y];

                m_tab_jewelds[m_position_gear.x + 1] [m_position_gear.y]         = buf;

                m_right_rotation = false;
            }
            m_go_rotation = true;
        }
        else if(mainEventManager->GetEvent(EventMouse, sf::Mouse::Right))
        {
            if(!m_go_rotation)
            {
                Jeweld buf = m_tab_jewelds[m_position_gear.x][m_position_gear.y];

                m_tab_jewelds[m_position_gear.x]    [m_position_gear.y]         = m_tab_jewelds[m_position_gear.x + 1]    [m_position_gear.y];

                m_tab_jewelds[m_position_gear.x + 1][m_position_gear.y]         = m_tab_jewelds[m_position_gear.x + 1][m_position_gear.y + 1];

                m_tab_jewelds[m_position_gear.x + 1][m_position_gear.y + 1]     = m_tab_jewelds[m_position_gear.x][m_position_gear.y + 1];

                m_tab_jewelds[m_position_gear.x]    [m_position_gear.y + 1]     = buf;

                m_right_rotation = true;
            }
            m_go_rotation = true;
        }
    }
    else
    {
        mainEventManager->ShowCursor(true);
    }


    if(m_go_rotation)
    {
        if(m_right_rotation)
            m_gear_rotation += mainEventManager->GetTime() * 300;
        else
            m_gear_rotation -= mainEventManager->GetTime() * 300;

        if(fabs(m_gear_rotation) > 90)
            m_gear_rotation_total += 90 * (1-2*(m_gear_rotation < 0)), m_gear_rotation = 0, m_go_rotation = false;
        if(m_gear_rotation_total < -360)
            m_gear_rotation_total += 360;
        if(m_gear_rotation_total > 360)
            m_gear_rotation_total -= 360;
    }

    CheckAlign();

    for(std::vector<sf::Sprite>::iterator i = m_jeweleds_destruction.begin();
        i != m_jeweleds_destruction.end() ; ++i)
    {
        if(i->GetColor().a-mainEventManager->GetTime() * 200 < 0)
            m_jeweleds_destruction.erase(i), i--;
        else
        {
            i->Scale(1+mainEventManager->GetTime(),1+mainEventManager->GetTime());
            i->SetColor(sf::Color(255,255,255,(int)((float)i->GetColor().a-mainEventManager->GetTime() * 200)));
        }
    }
}

void Bejeweled::CheckAlign(bool no_destruction)
{
    sf::Vector2i pos = m_position;
    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    for(int x = 0 ; x < TAB_WIDTH ; ++x)
        for(int y = 0 ; y < TAB_HEIGHT ; ++y)
        {
            if(m_tab_jewelds[x][y].m_type < 0 && y == 0)
            {
                m_tab_jewelds[x][y].m_type           = rand() % NBR_JEWELS;
                m_tab_jewelds[x][y].m_position.x     = x * 32;
                m_tab_jewelds[x][y].m_position.y     = -31;
                m_tab_jewelds[x][y].m_tab_position.x = x;
                m_tab_jewelds[x][y].m_tab_position.y = 0;

                m_tab_jewelds[x][y].m_sprite.SetImage(m_jewelds[m_tab_jewelds[x][y].m_type]);
            }

            if(y < TAB_HEIGHT - 1)
                if(m_tab_jewelds[x][y + 1].m_type < 0)
                {
                    for(int Y = y + 1 ; Y >=1 ; Y--)
                        m_tab_jewelds[x][Y] = m_tab_jewelds[x][Y - 1];
                    m_tab_jewelds[x][0].m_type = -1;
                }

            if(m_tab_jewelds[x][y].m_position.y != y * 32)
            {
                if(m_tab_jewelds[x][y].m_position.y > y * 32 )
                {
                    if(x >= m_position_gear.x && x <= m_position_gear.x+1
                    && y >= m_position_gear.y && y <= m_position_gear.y+1
                    && m_go_rotation)
                        m_tab_jewelds[x][y].m_position.y -= mainEventManager->GetTime() * 100;
                    else
                        m_tab_jewelds[x][y].m_position.y -= mainEventManager->GetTime() * 200;

                    if(m_tab_jewelds[x][y].m_position.y < y * 32)
                        m_tab_jewelds[x][y].m_position.y = y *32;
                }
                else
                {
                    if(x >= m_position_gear.x && x <= m_position_gear.x+1
                    && y >= m_position_gear.y && y <= m_position_gear.y+1
                    && m_go_rotation)
                        m_tab_jewelds[x][y].m_position.y += mainEventManager->GetTime() * 100;
                    else
                        m_tab_jewelds[x][y].m_position.y += mainEventManager->GetTime() * 200;

                    if(m_tab_jewelds[x][y].m_position.y > y * 32)
                        m_tab_jewelds[x][y].m_position.y = y *32;
                }
            }

            if(m_tab_jewelds[x][y].m_position.x != x * 32)
            {
                if(m_tab_jewelds[x][y].m_position.x > x * 32 )
                {
                    if(x >= m_position_gear.x && x <= m_position_gear.x+1
                    && y >= m_position_gear.y && y <= m_position_gear.y+1
                    && m_go_rotation)
                        m_tab_jewelds[x][y].m_position.x -= mainEventManager->GetTime() * 100;
                    else
                        m_tab_jewelds[x][y].m_position.x -= mainEventManager->GetTime() * 200;
                    if(m_tab_jewelds[x][y].m_position.x < x * 32)
                        m_tab_jewelds[x][y].m_position.x = x *32;
                }
                else
                {
                    if(x >= m_position_gear.x && x <= m_position_gear.x+1
                    && y >= m_position_gear.y && y <= m_position_gear.y+1
                    && m_go_rotation)
                        m_tab_jewelds[x][y].m_position.x += mainEventManager->GetTime() * 100;
                    else
                        m_tab_jewelds[x][y].m_position.x += mainEventManager->GetTime() * 200;
                    if(m_tab_jewelds[x][y].m_position.x > x * 32)
                        m_tab_jewelds[x][y].m_position.x = x *32;
                }
            }

            m_tab_jewelds[x][y].m_sprite.SetPosition(   m_tab_jewelds[x][y].m_position.x + (float)pos.x + 16.0f,
                                                        m_tab_jewelds[x][y].m_position.y + (float)pos.y + 16.0f);

            if( fabs(m_tab_jewelds[x][y].m_position.x - x * 32) <= 4
             && fabs(m_tab_jewelds[x][y].m_position.y - y * 32) <= 4)
             {
                 int chain = 1;
                 for(int X = x + 1 ; X < TAB_WIDTH ; ++X)
                 {
                    if(fabs(m_tab_jewelds[X][y].m_position.x - X *32) > 4 || fabs(m_tab_jewelds[X][y].m_position.y - y *32) > 4)
                        X = TAB_WIDTH, chain = 0;

                    if(m_tab_jewelds[X][y].m_type == m_tab_jewelds[x][y].m_type)
                        chain++;
                    else
                        X = TAB_WIDTH;
                 }
                 if(chain >= 3)
                    for(int X = x ; X < x + chain ; ++X)
                    {
                        if(!no_destruction)
                        {
                            m_jeweleds_destruction.push_back(m_tab_jewelds[X][y].m_sprite);
                            m_lasts_jewelds.push_back(m_tab_jewelds[X][y].m_type);
                        }
                        m_tab_jewelds[X][y].m_type = -1;
                    }


                 chain = 1;
                 for(int Y = y + 1 ; Y < TAB_HEIGHT ; ++Y)
                 {
                    if(fabs(m_tab_jewelds[x][Y].m_position.x - x *32) > 4 || fabs(m_tab_jewelds[x][Y].m_position.y - Y *32) > 4)
                        Y = TAB_HEIGHT, chain = 0;

                    if(m_tab_jewelds[x][Y].m_type == m_tab_jewelds[x][y].m_type)
                        chain++;
                    else
                        Y = TAB_HEIGHT;
                 }

                 if(chain >= 3)
                    for(int Y = y ; Y < y + chain ; ++Y)
                    {
                        if(!no_destruction)
                        {
                            m_lasts_jewelds.push_back(m_tab_jewelds[x][Y].m_type);
                            m_jeweleds_destruction.push_back(m_tab_jewelds[x][Y].m_sprite);
                        }
                        m_tab_jewelds[x][Y].m_type = -1;
                    }
             }
        }
}
