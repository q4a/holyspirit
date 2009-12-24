#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H


class GUI_Object
{
    public:
        GUI_Object();
        virtual ~GUI_Object();

        void Update(float time = 0);

        const sf::Image &GetImage();

    protected:
    private:

    sf::RenderImage m_render;
};

#endif // GUI_OBJECT_H
