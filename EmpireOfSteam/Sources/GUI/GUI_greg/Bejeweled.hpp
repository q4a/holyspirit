#ifndef BEJEWELED_HPP
#define BEJEWELED_HPP


#define NBR_JEWELS 8
#define TAB_WIDTH 10
#define TAB_HEIGHT 10

#include "GUI/Widget.hpp"

struct Jeweld
{
    int             m_type;
    sf::Vector2f    m_position;
    sf::Vector2i    m_tab_position;
    sf::Sprite      m_sprite;
};

class Bejeweled : public Widget
{
    public:
        Bejeweled();
        Bejeweled(int, int);
        Bejeweled(int, int, int, int);
        virtual ~Bejeweled();

        virtual void Update();
        virtual void Show(std::list<sf::Drawable*> &drawables);

        virtual void SetGeometry(int x, int y, int w, int h);

        void CheckAlign(bool no_destruction = false);

    protected:
        sf::Sprite *m_drawable;

        sf::Image m_gear;
        sf::Image m_jewelds[NBR_JEWELS];

        Jeweld      m_tab_jewelds[TAB_WIDTH][TAB_HEIGHT];
        sf::Sprite  m_sprite_gear;

        std::vector<sf::Sprite> m_jeweleds_destruction;

        sf::Vector2i m_position_gear;

        bool        m_go_rotation;
        bool        m_right_rotation;
        float       m_gear_rotation;
        float       m_gear_rotation_total;

    private:

};

#endif
