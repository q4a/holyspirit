
#ifndef RESSOURCEBAR_HPP
#define RESSOURCEBAR_HPP

#include "GUI/GUIProgressBar.hpp"

class RessourceBar : public GUIProgressBar
{
    public:
        RessourceBar(std::string label = "");
        RessourceBar(int, int, std::string label = "");
        RessourceBar(int, int, int, int, std::string label = "");
        virtual ~RessourceBar();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

    protected:
        GUIImage m_img_jauge_left;
        GUIImage m_img_jauge_right;
        Label    m_value_label;

    private:

};

#endif

