#include "typo.h"
#include "../globale.h"

#include <fstream>
#include <libintl.h>

using namespace std;

Typo::Typo()
{

}

void Typo::Load(const std::string &path)
{
    ifstream fichier;
    fichier.open(path.c_str(), ios::in);
    if (fichier)
    {
        char caractere;

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                string img_path;
                fichier>>img_path;
                m_img = moteurGraphique->AjouterImage(img_path, -1);
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Typo \" "+path+" \" is invalid",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier.get(caractere);
            if (caractere=='*')
            {
                m_chars.push_back(Character ());
                do
                {
                    fichier.get(caractere);

                    if(caractere == 'x')
                        fichier>>m_chars.back().pos.x;
                    else if(caractere == 'y')
                        fichier>>m_chars.back().pos.y;
                    else if(caractere == 'w')
                        fichier>>m_chars.back().size.x;
                    else if(caractere == 'h')
                        fichier>>m_chars.back().size.y;
                    else if(caractere == 'e')
                        fichier>>m_chars.back().space;
                }
                while(caractere!='$');

                caractere = ' ';
            }
            if (fichier.eof())
            {
                console->Ajouter("Error : Typo \" "+path+" \" is invalid",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        fichier.close();
    }
}


void Typo::Draw(const std::string &text, sf::Vector2f pos, int size, int layer, bool center, sf::Color color)
{
    std::vector<sf::Sprite> sprites;

    sf::Vector2f cur_pos = pos;

	sf::String utftext = toUtf32(text);

    for(unsigned i = 0 ; i < utftext.getSize() ; ++i)
    {
        bool maj = false;

        Character buf = getCharacter(utftext[i], maj);

        sprites.push_back(sf::Sprite ());
        sprites.back().setColor(color);
        sprites.back().setTexture(*moteurGraphique->getImage(m_img));
        sprites.back().setPosition(cur_pos);
        sprites.back().setTextureRect(sf::IntRect(buf.pos, buf.size));

        sprites.back().scale((float)size/72,(float)size/72);

        if(maj)
            sprites.back().scale(1.15,1.15), sprites.back().move(0,-(float)size*0.1);

        if(utftext[i] == 'v' && i+1 < utftext.getSize() && utftext[i+1] == 'a')
            cur_pos.x -=  buf.space * ((float)size/72) * (maj ? 1.1 : 1.0) * 0.25;
        if(i+1 < utftext.getSize() && utftext[i+1] == '.')
            cur_pos.x -= 8 * ((float)size/72) * (maj ? 1.1 : 1.0);

        cur_pos.x += buf.space * ((float)size/72) * (maj ? 1.1 : 1.0);
    }

    if(center)
        for(unsigned i = 0 ; i < sprites.size() ; ++i)
            sprites[i].move((pos.x - cur_pos.x)/2,0);


    for(unsigned i = 0 ; i < sprites.size() ; ++i)
        moteurGraphique->AjouterCommande(&sprites[i], layer, false);
}


float Typo::getSize(const sf::String &text, int size)
{
    std::vector<sf::Sprite> sprites;

    float cur_size = 0;

    for(unsigned i = 0 ; i < text.getSize() ; ++i)
    {
        bool maj = false;

        Character buf = getCharacter(text[i], maj);
        cur_size += buf.space * ((float)size/72) * (maj ? 1.1 : 1.0);
    }
    return cur_size;
}

Character &Typo::getCharacter(int car, bool &maj)
{
    if(car >= 'A' && car <= 'Z')
        maj = true, car += ('a' - 'A');
    if(car >= 0x0410 && car <= 0x042F)
        maj = true, car += (0x0430 - 0x0410);
    if(car >= 0x0400 && car <= 0x0409)
        maj = true, car += (0x0450 - 0x0400);

    if(car == ' ')
        return m_chars[1];
    if(car >= 'a' && car <= 'z')
        return m_chars[2 + car - 'a'];


    if(car == 233/*'é'*/)
        return m_chars[28];
    if(car == 232/*'è'*/)
        return m_chars[29];
    if(car >= '1' && car <= '9')
        return m_chars[30 + car - '1'];
    if(car == '0')
        return m_chars[39];
    if(car == '|')
        return m_chars[40];
    if(car == '\'')
        return m_chars[41];
    if(car == '-')
        return m_chars[42];
    if(car == ':')
        return m_chars[43];
    if(car == '.')
        return m_chars[44];
    if(car == 234/*'ê'*/)
        return m_chars[45];


    if(car >= 0x0430 && car <= 0x0435)
        return m_chars[46 + car - 0x0430];

    if(car == 0x0454)
        return m_chars[52];

    if(car >= 0x0436 && car <= 0x0438)
        return m_chars[53 + car - 0x0436];

    if(car == 0x0456)
        return m_chars[56];
    if(car == 0x0457)
        return m_chars[57];

    if(car >= 0x0439 && car <= 0x044F)
        return m_chars[58 + car - 0x0439];


    return m_chars.front();
}
