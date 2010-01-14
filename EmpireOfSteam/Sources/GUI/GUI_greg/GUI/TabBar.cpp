#include "TabBar.hpp"
#include <iostream>
using namespace sf;


TabBar::TabBar(int x, int y) : Widget(x, y)
{
    m_choose = -1;
}
TabBar::TabBar(int x, int y, int w, int h) : Widget(x, y, w, h)
{
    m_choose = -1;
}

TabBar::~TabBar()
{
}

void TabBar::Show(std::list<sf::Drawable *> &drawables)
{
    Widget::Show(drawables);

    if(m_choose >= 0)
        m_tabs_widgets[m_choose]->Show(drawables);

    for( std::vector<Tab*>::iterator i = m_tabs.begin();
         i != m_tabs.end();
         ++i )
            (*i)->Show(drawables);
}

void TabBar::SetGeometry(int x, int y, int w, int h)
{
    Widget::SetGeometry(x, y, w, h);
}

void TabBar::Update()
{
    Widget::Update();

    if(m_choose >= 0)
    {
        m_tabs_widgets[m_choose]->Update();
        m_tabs[m_choose]->SetState(Button_clicked);
    }


    int no = 0;
    for( std::vector<Tab*>::iterator i = m_tabs.begin();
         i != m_tabs.end();
         ++i, ++no )
         {
             (*i)->Update();
             if((*i)->Clicked() && no != m_choose)
             {
                 m_choose = no;
                 for( std::vector<Tab*>::iterator j = m_tabs.begin();
                     j != m_tabs.end();
                     ++j )
                     (*j)->SetState(Button_released);
             }
         }
}

void TabBar::AddTab(Tab *tab, Widget *widget)
{
    m_tabs.push_back(tab);
    m_tabs_widgets.push_back(widget);

    if(m_choose == -1)
        m_choose = 0;

    m_tabs.back()        ->SetParent(this);
    m_tabs_widgets.back()->SetParent(this);
}

