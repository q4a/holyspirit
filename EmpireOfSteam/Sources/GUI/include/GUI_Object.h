#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include "Utils.h"

class GUI_Object
{
    public:
        GUI_Object();
        virtual ~GUI_Object();

                void            Update(float time = 0);

        const   sf::Image      &GetImage();

        const   sf::Vector2f   &GetPosition();
        const   sf::Vector2f   &GetDimensions();

                void            Add_Object(GUI_Object *object);

    protected:
    private:

    sf::RenderImage             m_render;

    sf::Vector2f                m_position;
    sf::Vector2f                m_dimensions;

    std::list<GUI_Object*>      m_objects;
};

#endif // GUI_OBJECT_H
