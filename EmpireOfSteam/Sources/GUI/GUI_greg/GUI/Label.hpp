#ifndef LABEL_HPP
#define LABEL_HPP

#include "Widget.hpp"

class Label : public Widget
{
    public:
        Label();
        Label(std::string);
        Label(int, int, std::string);
        Label(int, int, int, int, std::string);
        virtual ~Label();

        virtual sf::Sprite Show();

        void SetText            (std::string);
        void SetColor           (sf::Color);
        void SetCharacterSize   (int);

        const std::string   &GetText();
        const sf::Color     &GetColor();
        const int           &GetCharacterSize();

    protected:
        std::string     m_text;
        sf::Color       m_color;
        int             m_size;

    private:
};

#endif

