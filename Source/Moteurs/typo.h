/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/



#ifndef TYPO_H
#define TYPO_H

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

struct Character
{
    sf::Vector2i pos;
    sf::Vector2i size;
    int space;
};

class Typo
{
    public:
    Typo();

    void Load(const std::string &path);
    //void Draw(const std::string &text, sf::Vector2f pos, int size, int layer, bool center = false, sf::Color color = sf::Color (255,255,255));
    void Draw(sf::String text, sf::Vector2f pos, int size, int layer, bool center = false, sf::Color color = sf::Color (255,255,255));
    float getSize(const std::string &text, int size);
    Character &getCharacter(char , bool &);


    private:
    int m_img;
    std::vector <Character> m_chars;
};

#endif

