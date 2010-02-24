#ifndef GUIIMAGE_HPP
#define GUIIMAGE_HPP

#include "Widget.hpp"

class GUIImage : public Widget
{
    public:
        GUIImage(std::string image = "");
        GUIImage(int, int, std::string image = "");
        GUIImage(int, int, int, int, std::string image = "");
        virtual ~GUIImage();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);
        virtual void SetImage(const sf::Image &);
        virtual void SetImage(const std::string &);

    protected:
        sf::Sprite *m_drawable;
        sf::Image m_img;

    private:

};

#endif
