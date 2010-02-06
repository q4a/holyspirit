#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "GUIImage.hpp"
#include "Label.hpp"

enum{Button_released, Button_hover, Button_clicked};

class Button : public Widget
{
    public:
        Button(std::string label = "");
        Button(int, int, std::string label = "");
        Button(int, int, int, int, std::string label = "");
        virtual ~Button();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetImage(const sf::Image &, int type = Button_released);
        virtual void SetImage(const std::string &, int type = Button_released);
        virtual void SetImage(GUIImage *, int type = Button_released);

        virtual void SetState(int type);

        bool Hover();
        bool Clicked();
        bool Released();

        Label m_label;

    protected:
        bool m_hover;
        bool m_clicked;
        bool m_released;

        GUIImage *m_img_hover;
        GUIImage *m_img_clicked;
        GUIImage *m_img_released;

        bool m_my_img_hover;
        bool m_my_img_clicked;
        bool m_my_img_released;

        //sf::Sprite *m_drawable;
    private:

};

#endif
