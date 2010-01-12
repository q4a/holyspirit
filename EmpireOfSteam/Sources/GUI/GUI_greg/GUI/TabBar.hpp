#ifndef TABBAR_HPP
#define TABBAR_HPP

#include "Tab.hpp"

class TabBar : public Widget
{
    public:
        TabBar(int, int);
        TabBar(int, int, int, int);
        virtual ~TabBar();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);

                void AddTab(Tab *tab, Widget *widget);


    protected:
        std::vector <Tab *>     m_tabs;
        std::vector <Widget *>  m_tabs_widgets;

    private:
        int  m_choose;

};

#endif
