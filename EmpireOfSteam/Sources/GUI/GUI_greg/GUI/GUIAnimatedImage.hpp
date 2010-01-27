
#ifndef GUIANIMATEDIMAGE_HPP
#define GUIANIMATEDIMAGE_HPP

#include "GUIImage.hpp"

class GUIAnimatedImage : public GUIImage
{
    public:
        GUIAnimatedImage(int = 0, std::string image = "");
        GUIAnimatedImage(int, int, int = 0, std::string image = "");
        GUIAnimatedImage(int, int, int, int, int = 0, std::string image = "");
        virtual ~GUIAnimatedImage();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetImage(int, const std::string &);

    protected:
        sf::Sprite *m_drawable;
        std::vector <sf::Image> m_list_img;

        int m_number_frame;
        float m_time_frame;
        float m_framerate;

    private:

};

#endif
