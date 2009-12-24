#include "GUI_Object.h"

GUI_Object::GUI_Object()
{
    //ctor
}

GUI_Object::~GUI_Object()
{
    //dtor
}

void GUI_Object::Update(float time)
{
    for(std::list<GUI_Object*>::iterator iter = m_objects.begin() ; iter != m_objects.end() ; ++iter)
        iter->Update(time);
}

void GUI_Object::Add_Object(GUI_Object *object);
{
    m_objects.push_back(object);
}

const sf::Image &GUI_Object::GetImage()
{
    for(std::list<GUI_Object*>::iterator iter = m_objects.begin() ; iter != m_objects.end() ; ++iter)
    {
        sf::Sprite sprite;

        sprite.SetImage     (iter->GetImage());
        sprite.SetPosition  (iter->GetPosition());
        sprite.Resize       (iter->GetDimensions());

        m_render.Draw(sprite);
    }

    m_render.Display();
    return m_render.GetImage();
}

