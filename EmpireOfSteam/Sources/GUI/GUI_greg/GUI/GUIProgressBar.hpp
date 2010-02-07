
#ifndef GUIPROGRESSBAR_HPP
#define GUIPROGRESSBAR_HPP

#include "GUIImage.hpp"
#include "Label.hpp"

enum{Img_Background, Img_Bar};

class GUIProgressBar : public Widget
{
    public:
        GUIProgressBar(std::string label = "");
        GUIProgressBar(int, int, std::string label = "");
        GUIProgressBar(int, int, int, int, std::string label = "");
        virtual ~GUIProgressBar();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetImage   (const sf::Image &, int type = Img_Bar);
        virtual void SetImage   (const std::string &, int type = Img_Bar);
        virtual void SetImage   (GUIImage *, int type = Img_Bar);

        virtual void SetMin     (int type);
        virtual void SetMax     (int type);
        virtual void SetValue   (int type);

        virtual int  GetValue   ();
        virtual int  GetMax     ();
        virtual int  GetMin     ();

        Label m_label;

    protected:
        GUIImage *m_img_background;
        GUIImage *m_img_bar;

        bool m_my_img_background;
        bool m_my_img_bar;

        int m_value;
        int m_min;
        int m_max;

    private:

};

#endif
