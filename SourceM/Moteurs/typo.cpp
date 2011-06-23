#include "typo.h"
#include "../globale.h"

#include <fstream>

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
                console->Ajouter("Erreur : Typo \" "+path+" \" Invalide",1);
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
                console->Ajouter("Erreur : Typo \" "+path+" \" Invalide",1);
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

    for(unsigned i = 0 ; i < text.size() ; ++i)
    {
        bool maj = false;

        Character buf = getCharacter(text[i], maj);

        sprites.push_back(sf::Sprite ());
        sprites.back().SetColor(color);
        sprites.back().SetImage(*moteurGraphique->getImage(m_img));
        sprites.back().SetPosition(cur_pos);
        sprites.back().SetSubRect(sf::IntRect(buf.pos, buf.size));

        sprites.back().Scale((float)size/72,(float)size/72);

        if(maj)
            sprites.back().Scale(1.15,1.15), sprites.back().Move(0,-(float)size*0.1);

        if(text[i] == 'v' && i+1 < text.size() && text[i+1] == 'a')
          cur_pos.x -=  buf.space * ((float)size/72) * (maj ? 1.1 : 1.0) * 0.25;

        cur_pos.x += buf.space * ((float)size/72) * (maj ? 1.1 : 1.0);
    }

    if(center)
        for(unsigned i = 0 ; i < sprites.size() ; ++i)
            sprites[i].Move((pos.x - cur_pos.x)/2,0);


    for(unsigned i = 0 ; i < sprites.size() ; ++i)
        moteurGraphique->AjouterCommande(&sprites[i], layer, false);
}


float Typo::getSize(const std::string &text, int size)
{
    std::vector<sf::Sprite> sprites;

    float cur_size = 0;

    for(unsigned i = 0 ; i < text.size() ; ++i)
    {
        bool maj = false;

        Character buf = getCharacter(text[i], maj);
        cur_size += buf.space * ((float)size/72) * (maj ? 1.1 : 1.0);
    }
    return cur_size;
}

Character &Typo::getCharacter(char car, bool &maj)
{
    if(car >= 'A' && car <= 'Z')
        maj = true, car += ('a' - 'A');

    if(car == ' ')
        return m_chars[1];
    if(car == 'a')
        return m_chars[2];
    if(car == 'b')
        return m_chars[3];
    if(car == 'c')
        return m_chars[4];
    if(car == 'd')
        return m_chars[5];
    if(car == 'e')
        return m_chars[6];
    if(car == 'f')
        return m_chars[7];
    if(car == 'g')
        return m_chars[8];
    if(car == 'h')
        return m_chars[9];
    if(car == 'i')
        return m_chars[10];
    if(car == 'j')
        return m_chars[11];
    if(car == 'k')
        return m_chars[12];
    if(car == 'l')
        return m_chars[13];
    if(car == 'm')
        return m_chars[14];
    if(car == 'n')
        return m_chars[15];
    if(car == 'o')
        return m_chars[16];
    if(car == 'p')
        return m_chars[17];
    if(car == 'q')
        return m_chars[18];
    if(car == 'r')
        return m_chars[19];
    if(car == 's')
        return m_chars[20];
    if(car == 't')
        return m_chars[21];
    if(car == 'u')
        return m_chars[22];
    if(car == 'v')
        return m_chars[23];
    if(car == 'w')
        return m_chars[24];
    if(car == 'x')
        return m_chars[25];
    if(car == 'y')
        return m_chars[26];
    if(car == 'z')
        return m_chars[27];
    if(car == 'é')
        return m_chars[28];
    if(car == 'è')
        return m_chars[29];
    if(car == '1')
        return m_chars[30];
    if(car == '2')
        return m_chars[31];
    if(car == '3')
        return m_chars[32];
    if(car == '4')
        return m_chars[33];
    if(car == '5')
        return m_chars[34];
    if(car == '6')
        return m_chars[35];
    if(car == '7')
        return m_chars[36];
    if(car == '8')
        return m_chars[37];
    if(car == '9')
        return m_chars[38];
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
    if(car == 'ê')
        return m_chars[44];

    return m_chars.front();
}
