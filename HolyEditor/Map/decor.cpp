

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


#include "decor.h"
#include "../globale.h"


Decor::Decor()
{
    m_tileset=-1;
    m_tile.clear();
    m_monstre.clear();
    m_herbe=-1;
    m_numeroHerbe=-1;
    m_animation=0;
    m_couche=0;
    m_hauteur=0;

    m_herbe = 0;
        m_numeroHerbe = 0;
        m_herbe_taille = 0;
        m_hauteur = 0;
        m_herbe_decalage.x = 0;
        m_herbe_decalage.y = 0;

    m_animation=0;

    m_projectile=-1;
    m_effet=-1;

    random_animation = 0;
}


Decor::Decor(int tileset,std::vector<int> tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur)
{
    m_tileset=tileset;
    m_tile=tile;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_animation=0;
    m_couche=couche;
    m_hauteur=hauteur;

    m_animation=0;

    m_projectile=-1;
    m_effet=-1;


    random_animation = 0;
}

Decor::Decor(int tileset,std::vector<int> tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
    m_tileset=tileset;
    m_tile=tile;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_animation=0;
    m_couche=couche;
    m_hauteur=hauteur;

    m_objets=objets;

    m_projectile=-1;
    m_effet=-1;


    random_animation = 0;
}

Decor::~Decor()
{
    m_objets.clear();
}



void Decor::setDecor(int tileset,std::vector<int> tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur)
{
    m_tileset=tileset;
    m_tile=tile;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    if (m_couche>4)
        m_couche=4;
    if (m_couche<0)
        m_couche=0;
}

void Decor::setDecor(int tileset,std::vector<int> tile,const std::vector<int> &monstre,int herbe, int couche,int hauteur,std::vector <Objet> objets)
{
    m_tileset=tileset;
    m_tile=tile;
    m_monstre=monstre;
    m_herbe=herbe;
    m_numeroHerbe=0;
    m_couche=couche;
    m_hauteur=hauteur;

    if (m_couche>4)
        m_couche=4;
    if (m_couche<0)
        m_couche=0;
    m_objets=objets;
}

bool Decor::AfficherTexteObjet(coordonnee position,int objet, float *decalage)
{
    bool retour=false;
    if(objet>=0&&objet<(int)m_objets.size())
    {

        float alpha = m_objets[objet].m_alpha;
        if(alpha > 255)
            alpha = 255;
        alpha /= 255;

        sf::Text texte;
        sf::Sprite sprite;

        texte.SetColor(sf::Color(   GetItemColor(m_objets[objet].getRarete()).r,
                                    GetItemColor(m_objets[objet].getRarete()).g,
                                    GetItemColor(m_objets[objet].getRarete()).b,
                                    (int)(255.0f*alpha)));

        texte.SetFont(moteurGraphique->m_font);

        texte.SetString(m_objets[objet].getNom());
        texte.SetCharacterSize(14*configuration->Resolution.w/800);
        texte.SetY((position.y-GetViewRect(moteurGraphique->m_camera).Top)/configuration->zoom*configuration->Resolution.h/600);
        texte.SetX((position.x-GetViewRect(moteurGraphique->m_camera).Left)/configuration->zoom*configuration->Resolution.w/800);

        moteurGraphique->AjouterTexteNonChevauchable(&texte,12);

        sprite.SetImage(*moteurGraphique->getImage(0));

        sprite.SetY(texte.GetPosition().y);
        sprite.SetX(texte.GetPosition().x-4);
        sprite.SetColor(sf::Color(0,0,0,(int)(224.0f*alpha)));
        sprite.Resize(texte.GetRect().Right-texte.GetRect().Left +8 , texte.GetRect().Bottom-texte.GetRect().Top+4);

        if(eventManager->getEvenement(sf::Key::LAlt,"ET"))
            if(eventManager->getPositionSouris().x>sprite.GetPosition().x
             &&eventManager->getPositionSouris().y>sprite.GetPosition().y
             &&eventManager->getPositionSouris().x<sprite.GetPosition().x+texte.GetRect().Right-texte.GetRect().Left +8
             &&eventManager->getPositionSouris().y<sprite.GetPosition().y+texte.GetRect().Bottom-texte.GetRect().Top +4)
                retour = true,sprite.SetColor(sf::Color(32,32,32,224));

        moteurGraphique->AjouterCommande(&sprite,12,0);

        if(decalage != NULL)
            *decalage = (sprite.GetPosition().y*configuration->zoom + GetViewRect(moteurGraphique->m_camera).Top) - sprite.GetSize().y;
    }
    return (retour);
}

int Decor::AfficherTexteObjets(coordonnee position, int objetPointe)
{
    int retour = -1;
    float decalage = position.y-18;

    for (int z=0;z<(int)m_objets.size();z++)
    {
        if(m_objets[z].m_alpha > 0)
            if(AfficherTexteObjet(coordonnee ((int)position.x, (int)decalage), z, &decalage))
                retour=z;
    }

    return (retour);
}

void Decor::AlphaObjets(int alpha)
{
    for(unsigned i = 0; i < m_objets.size() ; ++i)
        if(m_objets[i].m_alpha < alpha)
            m_objets[i].m_alpha = alpha;
}


void Decor::setNumeroHerbe(int numero)
{
    m_numeroHerbe=numero;
    m_herbe_taille = (rand()% (125 - 75 )) + 75;
    m_herbe_decalage.x = 16 - (rand()%32);
    m_herbe_decalage.y = 0/*16 - (rand()%32)*/;

    int randomIntensite=rand() % ( 255 - 128 +1 ) + 128;
    int rougatre=rand() % (64);

    m_herbe_couleur.r=randomIntensite;
    m_herbe_couleur.g=randomIntensite - rougatre;
    m_herbe_couleur.b=randomIntensite - rougatre;
}
void Decor::setMonstre(int monstre)
{
    m_monstre.push_back(monstre);
}

void Decor::delMonstre(int monstre)
{
    for(int i = 0 ; i < (int)m_monstre.size() ; ++i)
        if(m_monstre[i]==monstre)
            m_monstre.erase(m_monstre.begin() + i), i = -1;
}

void Decor::setProjectile(int projectile)
{
    m_projectile=projectile;
}
void Decor::setEffetGraphique(int effet)
{
    m_effet=effet;
}
void Decor::setTileset(int tileset)
{
    m_tileset=tileset;
}
void Decor::setTile(std::vector<int> tile)
{
    m_tile=tile;
}
void Decor::setCouche(int couche)
{
    m_couche=couche;
}
void Decor::AjouterObjet(Objet objet)
{
    coordonnee position;

    m_objets.push_back(objet);
    bool continuer = true;
    for(position.y = 0;continuer;++position.y)
        for(position.x = 0;position.x < 2 && continuer;++position.x)
        {
            bool ok = true;
            for(int i = 0;i < (int)m_objets.size() - 1;++i)
                if(m_objets[i].getPosition().x == position.x && m_objets[i].getPosition().y == position.y)
                    ok = false;

            if(ok)
                continuer = false, m_objets.back().setPosition(position.x, position.y);
        }


    m_objets.back().JouerSon();
    m_objets.back().m_alpha = 512;
}


void Decor::supprimerObjet(int numero)
{
    if (numero>=0&&numero<(int)m_objets.size())
    {
        m_objets.erase(m_objets.begin()+numero);

        int x = 0, y = 0;
        if((int)m_objets.size() == 4)
            for(int i = 0;i < (int)m_objets.size();++i)
            {
                m_objets[i].setPosition(x, y);
                x++;
                if(x>=2)
                    x = 0, y++;
            }
    }
}


std::vector<int> Decor::getTile()
{
    return m_tile;
}
int Decor::getTileset()
{
    return m_tileset;
}
const std::vector<int> &Decor::getMonstre()
{
    return m_monstre;
}
int Decor::getProjectile()
{
    return m_projectile;
}
int Decor::getEffetGraphique()
{
    return m_effet;
}
int Decor::getCouche()
{
    return m_couche;
}

int Decor::getHerbe()
{
    return m_herbe;
}
int Decor::getNumeroHerbe()
{
    return m_numeroHerbe;
}
int Decor::getTailleHerbe()
{
    return m_herbe_taille;
}
const sf::Color &Decor::getCouleurHerbe()
{
    return m_herbe_couleur;
}
const coordonnee &Decor::getDecalageHerbe()
{
    return m_herbe_decalage;
}

float Decor::getAnimation()
{
    return m_animation;
}
Objet* Decor::getObjet(int numero)
{
    if (numero>=0&&numero<(int)m_objets.size()) return &m_objets[numero];
    return NULL;
}
const std::vector<Objet> &Decor::getObjets()
{
    return m_objets;
}
std::vector<Objet>* Decor::getPointeurObjets()
{
    return &m_objets;
}
int Decor::getNombreObjets()
{
    return m_objets.size();
}
int Decor::getHauteur()
{
    return m_hauteur;
}

void Decor::DecrementerAnimation(float nombre)
{
    m_animation-=nombre;
}
void Decor::AugmenterAnimation(float temps)
{
    m_animation+=temps;
    for(unsigned i = 0; i < m_objets.size() ; ++i)
    {
        m_objets[i].m_alpha -= temps*200;
        if(m_objets[i].m_alpha < 0)
            m_objets[i].m_alpha = 0;
    }

}



