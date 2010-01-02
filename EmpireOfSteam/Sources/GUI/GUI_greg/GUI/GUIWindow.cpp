
#include "GUIWindow.hpp"
#include <iostream>

using namespace std;
using namespace sf;

GUIWindow::GUIWindow()
{
    m_window.Create(VideoMode(800, 600, 32), "Window", Style::Titlebar);
}

GUIWindow::GUIWindow(int x, int y, std::string title)
{
    m_window.Create(VideoMode(x, y, 32), title, Style::Titlebar);
}


GUIWindow::~GUIWindow()
{
}

void GUIWindow::Run()
{
    while(m_window.IsOpened())
    {
        mainEventManager->Update(m_window);
        Update();

        Show();
    }
}

void GUIWindow::Show()
{
    m_window.Clear();

    std::list<sf::Drawable *> drawables;

    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->Show(drawables);

    for(std::list<sf::Drawable*>::iterator i = drawables.begin() ; i != drawables.end() ; ++i)
        m_window.Draw(**i);

    m_window.Display();
}

