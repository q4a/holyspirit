#ifndef TAB_HPP
#define TAB_HPP

#include "Button.hpp"

class Tab : public Button
{
    public:
        Tab();
        Tab(int, int);
        Tab(int, int, int, int);
        virtual ~Tab();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetImage(const sf::Image &, int type = Button_released);
        virtual void SetImage(const std::string &, int type = Button_released);

        virtual void SetState(int type);

    protected:

    private:
        sf::Image m_img_hover;
        sf::Image m_img_clicked;
        sf::Image m_img_released;
};

#endif
