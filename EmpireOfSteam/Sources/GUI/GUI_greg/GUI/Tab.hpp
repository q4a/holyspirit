#ifndef TAB_HPP
#define TAB_HPP

#include "Button.hpp"

class Tab : public Button
{
    public:
        Tab(std::string label = "");
        Tab(int, int, std::string label = "");
        Tab(int, int, int, int, std::string label = "");
        virtual ~Tab();

        virtual void Update();

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
