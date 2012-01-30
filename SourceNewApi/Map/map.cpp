

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


#include "map.h"

#include <iostream>
#include <fstream>
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"


sf::Packet& operator <<(sf::Packet& Packet,  Personnage& C);
sf::Packet& operator >>(sf::Packet& Packet, Personnage& C);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}

Map::Map()
{
    console->Ajouter("Creation minimap :");

    /*for(int y = 0 ; y < MINIMAP_SIZE ; ++y)
    for(int x = 0 ; x < MINIMAP_SIZE ; ++x)
    {
        m_render_minimap[y][x].Create(1024,1024);
        m_render_minimap[y][x].Clear(sf::Color(0,0,0,0));
    }*/

    m_etat_chargement   = 0;
    m_loaded            = false;

    m_monstreIllumine   = -1;

    console->Ajouter("");
    console->Ajouter("Chargements d'images diverses :");
}

Map::~Map()
{
    Detruire();
}


void Map::CalculerOmbresEtLumieres()
{
    if (configuration->heure+1<24)
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].intensite*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].rouge*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].vert*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].bleu*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[configuration->heure+1].hauteur*configuration->minute))*0.016666666666666666666666666666667f;
    }
    else
    {
        moteurGraphique->m_soleil.intensite=(int)(((float)m_lumiere[configuration->heure].intensite*(60-configuration->minute)+((float)m_lumiere[0].intensite*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.rouge=(int)(((float)m_lumiere[configuration->heure].rouge*(60-configuration->minute)+((float)m_lumiere[0].rouge*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.vert=(int)(((float)m_lumiere[configuration->heure].vert*(60-configuration->minute)+((float)m_lumiere[0].vert*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.bleu=(int)(((float)m_lumiere[configuration->heure].bleu*(60-configuration->minute)+((float)m_lumiere[0].bleu*configuration->minute))*0.016666666666666666666666666666667f);
        moteurGraphique->m_soleil.hauteur=((float)m_lumiere[configuration->heure].hauteur*(60-configuration->minute)+((float)m_lumiere[0].hauteur*configuration->minute))*0.016666666666666666666666666666667f;
    }

    int r = moteurGraphique->m_soleil.rouge;
    int g = moteurGraphique->m_soleil.vert;
    int b = moteurGraphique->m_soleil.bleu;
    int i = moteurGraphique->m_soleil.intensite;
    int h = (int)moteurGraphique->m_soleil.hauteur;


    for(unsigned j = 0 ; j < m_climates.size() ; ++j)
        if(m_climates[j].m_actif)
        {
            r = (int)((float)r*(255 - (float)m_climates[j].m_lumiereModificater.rouge * m_climates[j].GetState())/255);
            g = (int)((float)g*(255 - (float)m_climates[j].m_lumiereModificater.vert * m_climates[j].GetState())/255);
            b = (int)((float)b*(255 - (float)m_climates[j].m_lumiereModificater.bleu * m_climates[j].GetState())/255);
            i = (int)((float)i*(255 - (float)m_climates[j].m_lumiereModificater.intensite * m_climates[j].GetState())/255);
            h = (int)((float)h*(255 - (float)m_climates[j].m_lumiereModificater.hauteur * m_climates[j].GetState())/255);
        }


    if(r > 255) r = 255; if(r < 0) r = 0;
    if(g > 255) g = 255; if(g < 0) g = 0;
    if(b > 255) b = 255; if(b < 0) b = 0;
    if(i > 255) i = 255; if(i < 0) i = 0;
    if(h > 255) h = 255; if(h < 0) h = 0;

    moteurGraphique->m_soleil.rouge = r;
    moteurGraphique->m_soleil.vert = g;
    moteurGraphique->m_soleil.bleu = b;
    moteurGraphique->m_soleil.intensite = i;
    moteurGraphique->m_soleil.hauteur = h;

    moteurGraphique->m_angleOmbreSoleil=((float)configuration->heure*60+configuration->minute)*180/720+135;

}

void Map::AfficherSac(coordonnee positionSac,float decalage,coordonnee position_sac_inventaire,Caracteristique caract, std::string cheminClasse, Border &border)
{
    Sprite Sprite;
    Text texte;

    m_objetPointe=-1;
    coordonnee position;

    if (positionSac.x>=0&&positionSac.x<m_dimensions.x
     && positionSac.y>=0&&positionSac.y<m_dimensions.y)
        if (m_decor[1][positionSac.y][positionSac.x].getNombreObjets()>0)
        {

            if (m_defilerObjets>m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-position_sac_inventaire.h+1)
                m_defilerObjets=m_decor[1][positionSac.y][positionSac.x].getNombreObjets()-position_sac_inventaire.h+1;
            if (m_defilerObjets<0)
                m_defilerObjets=0;

            for (int z=m_defilerObjets;
                    z<m_decor[1][positionSac.y][positionSac.x].getNombreObjets()
                 && z<position_sac_inventaire.h+m_defilerObjets;z++)
            {
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(position_sac_inventaire.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(position_sac_inventaire.x + position_sac_inventaire.w * 32,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,position_sac_inventaire.y  + (z - m_defilerObjets) * 20).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,position_sac_inventaire.y  + (z - m_defilerObjets) * 20 + 20).y)
                {
                    Sprite.SetTexture(*moteurGraphique->getImage(0));
                    Sprite.SetColor(sf::Color(255,255,255,128));
                    Sprite.Resize(position_sac_inventaire.w,20);
                    Sprite.SetPosition(AutoScreenAdjust(position_sac_inventaire.x,
                                                        position_sac_inventaire.y+(z-m_defilerObjets)*20, decalage));

                    moteurGraphique->AjouterCommande(&Sprite,16,0);

                    if(!(z == m_defilerObjets && m_defilerObjets > 0)
                    && !((z-m_defilerObjets == position_sac_inventaire.h-1
                        &&z < m_decor[1][positionSac.y][positionSac.x].getNombreObjets() - 1)))
                    {
                        m_objetPointe=z;
                        m_decor[1][positionSac.y][positionSac.x].getObjet(z)->AfficherCaracteristiques(eventManager->getPositionSouris(),border,caract,NULL,cheminClasse);
                    }
                }

                texte.SetColor(GetItemColor(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getRarete()));
                texte.SetFont(moteurGraphique->m_font);

                if (z == m_defilerObjets && m_defilerObjets > 0)
                    texte.SetString("...");
                else if ((z-m_defilerObjets == position_sac_inventaire.h-1
                       && z < m_decor[1][positionSac.y][positionSac.x].getNombreObjets() - 1))
                    texte.SetString("...");
                else
                    texte.SetString(m_decor[1][positionSac.y][positionSac.x].getObjet(z)->getNom());
                texte.SetCharacterSize(16);

                texte.SetPosition(AutoScreenAdjust(position_sac_inventaire.x,
                                                   position_sac_inventaire.y+(z-m_defilerObjets)*20, decalage));
                texte.Move((position_sac_inventaire.w-texte.GetRect().Width)*0.5,0);

                moteurGraphique->AjouterTexte(&texte,16);
            }
        }
}

void Map::Afficher(Hero *hero,std::list<Hero> &players,bool alt,float alpha)
{
    coordonnee positionHero;

    Sprite sprite;
    String texte;

    positionHero.y=(int)((hero->m_personnage.getCoordonneePixel().x+hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
    positionHero.x=(int)(((hero->m_personnage.getCoordonneePixel().x-hero->m_personnage.getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1)*64);

    coordonnee position;

    bool modif_minimap = false;

    if (alpha>0)
    {
        for(int y = 0 ; y < MINIMAP_SIZE ; ++y)
        for(int x = 0 ; x < MINIMAP_SIZE ; ++x)
        {
            sf::Sprite map;
            map.SetTexture(m_render_minimap[y][x].GetTexture());
            map.SetPosition(configuration->Resolution.x*0.5f + (-positionHero.x) * 0.125f + x * 1024 - MINIMAP_SIZE * 512,
                            configuration->Resolution.y*0.5f + (-positionHero.y) * 0.125f + y * 1024);
            map.SetColor(sf::Color(255,255,255,(int)(alpha * 0.5f)));
            moteurGraphique->AjouterCommande(&map,12,0);
        }


        sf::Sprite minimap;
        minimap.SetTexture(*moteurGraphique->getImage(hero->m_classe.icone_mm.image));
        minimap.SetSubRect(sf::IntRect(hero->m_classe.icone_mm.position.x, hero->m_classe.icone_mm.position.y,
                                       hero->m_classe.icone_mm.position.w, hero->m_classe.icone_mm.position.h));

        minimap.SetColor(sf::Color(255,255,255,(int)(alpha * 0.5f)));
        minimap.SetPosition((int)(configuration->Resolution.x*0.5f) ,
                            (int)(configuration->Resolution.y*0.5f));
        minimap.SetOrigin(hero->m_classe.icone_mm.position.w/2, hero->m_classe.icone_mm.position.h/2);
        moteurGraphique->AjouterCommande(&minimap,12,0);

        for(unsigned i = 0 ; i < hero->m_amis.size() ; ++i)
        {
            coordonnee pos;

            pos.y=(int)((hero->m_amis[i]->getCoordonneePixel().x+hero->m_amis[i]->getCoordonneePixel().y)*DIVISEUR_COTE_TILE*32);
            pos.x=(int)(((hero->m_amis[i]->getCoordonneePixel().x-hero->m_amis[i]->getCoordonneePixel().y)*DIVISEUR_COTE_TILE-1)*64);

            minimap.SetColor(sf::Color(255,0,192));
            minimap.SetPosition((int)(configuration->Resolution.x*0.5f + (pos.x - positionHero.x) * 0.125f),
                                (int)(configuration->Resolution.y*0.5f + (pos.y - positionHero.y) * 0.125f));
            moteurGraphique->AjouterCommande(&minimap,12,0);
        }
    }

    if(m_img_sky >= 0)
    {
        sf::Sprite sky;
        sky.SetTexture(*moteurGraphique->getImage(m_img_sky));
        sky.Resize(configuration->Resolution.x,configuration->Resolution.y);
        moteurGraphique->AjouterCommande(&sky,0,0);
    }

    for(unsigned i = 0 ; i < m_climates.size() ; ++i)
        m_climates[i].Draw();

    const int maxY = hero->m_personnage.getCoordonnee().y + (int)(14 * configuration->zoom * configuration->Resolution.x/800);
    const int maxX = hero->m_personnage.getCoordonnee().x + (int)(14 * configuration->zoom * configuration->Resolution.x/800);

    for (int couche=0;couche<NOMBRE_COUCHE_MAP;couche++)
    {
        for (int j=hero->m_personnage.getCoordonnee().y - (int)(12 * configuration->zoom * configuration->Resolution.x/800) ;j<maxY;++j)
        {
            for (int k=hero->m_personnage.getCoordonnee().x - (int)(12 * configuration->zoom * configuration->Resolution.x/800) ;k<maxX ;++k)
            {
                if (j>=0&&j<(int)m_decor[couche].size()
                 && k>=0&&k<(int)m_decor[couche][j].size())
                {
                    //if(m_decor[couche][j][k].added_minimap)
                    if(m_decor[couche][j][k].m_entite_graphique.m_tileset != NULL)
                    {
                        if (m_decor[couche][j][k].m_entite_graphique.m_tileset->getTransparentDuTile(m_decor[couche][j][k].m_entite_graphique.m_noAnimation))
                        {
                            int alpha2=(int)(positionHero.y-(k+j)*32 - 16)+224;

                            if(m_decor[couche][j][k].m_entite_graphique.m_tileset->getOrientationDuTile(m_decor[couche][j][k].m_entite_graphique.m_noAnimation) == 'x')
                                alpha2 = (int)(hero->m_personnage.getCoordonneePixel().y-j*COTE_TILE + COTE_TILE * 3);
                            else if(m_decor[couche][j][k].m_entite_graphique.m_tileset->getOrientationDuTile(m_decor[couche][j][k].m_entite_graphique.m_noAnimation) == 'y')
                                alpha2 = (int)(hero->m_personnage.getCoordonneePixel().x-k*COTE_TILE + COTE_TILE * 3);

                            if (alpha2<configuration->alpha)
                                alpha2=configuration->alpha;
                            if (alpha2>255)
                                alpha2=255;

                            m_decor[couche][j][k].m_entite_graphique.SetColor(sf::Color(255,255,255,alpha2));
                        }
                        moteurGraphique->AjouterEntiteGraphique(&m_decor[couche][j][k].m_entite_graphique);
                    }

                    ///SOURCE DE CRASH
                    if(!m_decor[couche][j][k].added_minimap)
                        if(fabs(j-hero->m_personnage.getCoordonnee().y) < 8 && fabs(k-hero->m_personnage.getCoordonnee().x) < 8)
                            if(TileVisible(k,j,hero->m_personnage.getCoordonnee()))
                            {
                                if(m_decor[couche][j][k].m_spriteMinimap.GetSize().x> 1)
                                {
                                    int cur_y = 0;
                                    int cur_x = MINIMAP_SIZE / 2;

                                    sf::Sprite minimap = m_decor[couche][j][k].m_spriteMinimap;

                                    minimap.SetPosition((minimap.GetPosition().x) * 0.125f,
                                                        (minimap.GetPosition().y) * 0.125f);

                                    while(minimap.GetPosition().y > 1024)
                                        minimap.Move(0, -1024), cur_y ++;
                                    while(minimap.GetPosition().x > 1024)
                                        minimap.Move(-1024, 0), cur_x ++;
                                    while(minimap.GetPosition().x - minimap.GetSize().x / 2 < 0)
                                        minimap.Move(1024, 0), cur_x --;
                                    while(minimap.GetPosition().y - minimap.GetSize().y / 2 < 0)
                                        minimap.Move(0, 1024), cur_y --;


                                    if(m_render_minimap[cur_y][cur_x].GetHeight() != 1024)
                                    {
                                        m_render_minimap[cur_y][cur_x].Create(1024,1024);
                                        m_render_minimap[cur_y][cur_x].Clear(sf::Color(0,0,0,0));
                                    }
                                    m_render_minimap[cur_y][cur_x].Draw(minimap);

                                    bool redraw = false;

                                    if(minimap.GetPosition().y + minimap.GetSize().y > 1024)
                                        cur_y++, minimap.Move(0,-1024), redraw = true;
                                    if(minimap.GetPosition().x + minimap.GetSize().x > 1024)
                                        cur_x++, minimap.Move(-1024,0), redraw = true;


                                    if(m_render_minimap[cur_y][cur_x].GetHeight() != 1024)
                                    {
                                        m_render_minimap[cur_y][cur_x].Create(1024,1024);
                                        m_render_minimap[cur_y][cur_x].Clear(sf::Color(0,0,0,0));
                                    }
                                    if(redraw)
                                        m_render_minimap[cur_y][cur_x].Draw(minimap);


                                    modif_minimap = true;
                                }

                                m_decor[couche][j][k].added_minimap = true;
                            }
                    ///

                    if (couche==1)
                    {
                        if(configuration->Herbes)
                            moteurGraphique->AjouterEntiteGraphique(&m_decor[0][j][k].m_entite_herbe);

                        //if(TileVisible(k,j,hero->m_personnage.getCoordonnee()))
                        for (unsigned o = 0 ; o < m_decor[1][j][k].getMonstre().size() ; ++o)
                            if (m_decor[1][j][k].getMonstre()[o]>=0&&m_decor[1][j][k].getMonstre()[o]<(int)m_monstre.size())
                            if (m_monstre[m_decor[1][j][k].getMonstre()[o]].getModele()>=0
                             && m_monstre[m_decor[1][j][k].getMonstre()[o]].getModele()<(int)m_ModeleMonstre.size())
                                m_monstre[m_decor[1][j][k].getMonstre()[o]].Afficher(&m_ModeleMonstre[m_monstre[m_decor[1][j][k].getMonstre()[o]].getModele()],
                                                                                     hero->m_classe.border,
                                                                                     m_decor[1][j][k].getMonstre()[o]==m_monstreIllumine);

                        if ((int)((hero->m_personnage.getCoordonneePixel().x + COTE_TILE * 0.5f) / COTE_TILE) == k
                        &&  (int)((hero->m_personnage.getCoordonneePixel().y + COTE_TILE * 0.5f) / COTE_TILE) == j)
                                hero->Afficher();

                        for (std::list<Hero>::iterator p = players.begin(); p != players.end(); ++p)
                        if ((int)((p->m_personnage.getCoordonneePixel().x + COTE_TILE * 0.5f) / COTE_TILE) == k
                        &&  (int)((p->m_personnage.getCoordonneePixel().y + COTE_TILE * 0.5f) / COTE_TILE) == j)
                                p->Afficher();

                        if(configuration->Herbes)
                            moteurGraphique->AjouterEntiteGraphique(&m_decor[1][j][k].m_entite_herbe);

                        for (unsigned o = 0 ; o < m_decor[1][j][k].getProjectile().size() ; ++o)
                            if (m_decor[1][j][k].getProjectile()[o]>=0&&m_decor[1][j][k].getProjectile()[o]<(int)m_projectile.size())
                                m_projectile[m_decor[1][j][k].getProjectile()[o]].Afficher();

                        for (unsigned o = 0 ; o < m_decor[1][j][k].getEffetGraphique().size() ; ++o)
                            if (m_decor[1][j][k].getEffetGraphique()[o]>=0&&m_decor[1][j][k].getEffetGraphique()[o]<(int)m_effets.size())
                                m_effets[m_decor[1][j][k].getEffetGraphique()[o]].Afficher();

                        if (m_decor[1][j][k].getNombreObjets()>0&&couche==1)
                        {
                            position.x=(k-j-1)*64+64;
                            position.y=(k+j)*32+32;
                            for (int o=0;o<m_decor[1][j][k].getNombreObjets();o++)
                                m_decor[1][j][k].getObjet(o)->Afficher(position);

                            int objetPointe=-1;

                            objetPointe=m_decor[1][j][k].AfficherTexteObjets(hero->m_classe.border, position,m_monstreIllumine==-1/*,alt*/);

                            if (alt || objetPointe >= 0
                                    || (eventManager->getCasePointee().x == k
                                    && eventManager->getCasePointee().y == j
                                    && m_monstreIllumine==-1
                                    &&!eventManager->getEvenement(sf::Mouse::Left,EventClicA)))
                                m_decor[1][j][k].AlphaObjets(255);


                            if (objetPointe>=0&&!eventManager->getEvenement(sf::Mouse::Left,EventClicA)&&m_monstreIllumine==-1/* && alt*/)
                            {
                                m_sacPointe.x=k;
                                m_sacPointe.y=j;

                                if (eventManager->getEvenement(sf::Keyboard::LControl,EventKey))
                                    m_decor[1][j][k].getObjet(objetPointe)->AfficherCaracteristiques(eventManager->getPositionSouris(),hero->m_classe.border,hero->m_caracteristiques,&hero->m_coffre,hero->m_cheminClasse);

                                m_objetPointe=objetPointe;
                            }
                        }
                    }
                }
                else if (couche==0)
                {

                    int w=j,z=k;

                    if (j>=m_dimensions.y)
                        w=m_dimensions.y-1;
                    if (k>=m_dimensions.x)
                        z=m_dimensions.x-1;
                    if (j<0)
                        w=0;
                    if (k<0)
                        z=0;

                    sf::Sprite buffer(m_decor[couche][w][z].m_entite_graphique);

                    buffer.SetX((k-j)*64);
                    buffer.SetY((k+j+1)*32);

                    if (buffer.GetSize().x>0)
                        if(buffer.GetPosition().x+buffer.GetSize().x - buffer.GetOrigin().x>=GetViewRect(moteurGraphique->m_camera).Left
                        && buffer.GetPosition().x-buffer.GetOrigin().x < GetViewRect(moteurGraphique->m_camera).Left + GetViewRect(moteurGraphique->m_camera).Width
                        && buffer.GetPosition().y+buffer.GetSize().y - buffer.GetOrigin().y>=GetViewRect(moteurGraphique->m_camera).Top
                        && buffer.GetPosition().y-buffer.GetOrigin().y < GetViewRect(moteurGraphique->m_camera).Top  + GetViewRect(moteurGraphique->m_camera).Height)
                            moteurGraphique->AjouterCommande(&buffer,m_decor[couche][w][z].getCouche(),1);
                }
            }
        }
    }

    if(modif_minimap)
    {
        for(int y = 0 ; y < MINIMAP_SIZE ; ++y)
        for(int x = 0 ; x < MINIMAP_SIZE ; ++x)
            m_render_minimap[y][x].Display();
    }
}


int Map::AjouterProjectile(coordonneeDecimal positionReel,coordonneeDecimal cible,coordonnee lanceur,int couche,float  vitesse,float decalageAngle,bool monstre,Tileset *tileset)
{
    m_projectile.push_back(Projectile ());

    m_projectile.back().m_position.x = positionReel.x;
    m_projectile.back().m_position.y = positionReel.y;
    m_projectile.back().m_position.h=0;

    m_projectile.back().m_depart = lanceur;

    m_projectile.back().m_monstre=monstre;

    m_projectile.back().m_actif     = true;
    m_projectile.back().m_supprime  = false;

    m_projectile.back().m_effet.m_couche = couche;

    coordonneeDecimal position,position2;

    double m=atan2(cible.y-lanceur.y*COTE_TILE,cible.x-lanceur.x*COTE_TILE);

    m+=decalageAngle;

    position.x=cos(m)*vitesse/10;
    position.y=sin(m)*vitesse/10;

    m_projectile.back().m_vecteur=position;

    m_projectile.back().m_rotationReelle=m;




    position.x=(positionReel.x-positionReel.y);
    position.y=(positionReel.x+positionReel.y)/2;

    position2.x=((positionReel.x + m_projectile.back().m_vecteur.x)-(positionReel.y + m_projectile.back().m_vecteur.y));
    position2.y=((positionReel.x + m_projectile.back().m_vecteur.x)+(positionReel.y + m_projectile.back().m_vecteur.y))/2;


    m=atan2(position2.y-position.y,position2.x-position.x);

    m_projectile.back().m_rotation=m;

    m_projectile.back().m_positionCase=lanceur;

    position.x=(positionReel.x-positionReel.y) * 64 / COTE_TILE;
    position.y=(positionReel.x+positionReel.y) * 64 / COTE_TILE;

    m_projectile.back().m_effet.m_tileset = tileset;
    m_projectile.back().m_effet.Initialiser(position);

    if (couche>=0&&couche<2)
        if (lanceur.y>=0&&lanceur.y<m_dimensions.y)
            if (lanceur.x>=0&&lanceur.x<m_dimensions.x)
            {
                m_projectile.back().m_positionCase.x=lanceur.x;
                m_projectile.back().m_positionCase.y=lanceur.y;
                m_decor[1][lanceur.y][lanceur.x].setProjectile(m_projectile.size()-1);
            }

    return m_projectile.size()-1;
}

void Map::Animer(Hero *hero,float temps)
{
    /*for(unsigned i = 0 ; i < m_ambiances.size();++i)
        if(m_playAmbiances[i])
            moteurSons->JouerSon(m_ambiances[i],coordonnee (), 1, 0, 100);*/

    coordonnee positionHero;
    positionHero.x=(hero->m_personnage.getCoordonnee().x-hero->m_personnage.getCoordonnee().y-1)/5;
    positionHero.y=(hero->m_personnage.getCoordonnee().x+hero->m_personnage.getCoordonnee().y)/5;

    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-12;
    vueMin.y=hero->m_personnage.getCoordonnee().y-12;
    vueMax.x=hero->m_personnage.getCoordonnee().x+14;
    vueMax.y=hero->m_personnage.getCoordonnee().y+14;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>m_dimensions.x)
        vueMax.x=m_dimensions.x;
    if (vueMax.y>m_dimensions.y)
        vueMax.y=m_dimensions.y;

    for (int i=0;i<2;++i)
        for (int j=vueMin.y;j<vueMax.y;j++)
            for (int k=vueMin.x;k<vueMax.x;k++)
            {
                m_decor[i][j][k].m_entite_graphique.Animer(temps);
                m_decor[i][j][k].m_entite_herbe.Animer(temps);

                m_decor[i][j][k].AnimerObjets(temps);

                /*for (int z=0;z<(int)m_decor[i][j][k].getMonstre().size();z++)
                {
                    int monstre=m_decor[i][j][k].getMonstre()[z];
                    if (monstre>=0&&monstre<(int)m_monstre.size())
                    {
                        m_monstre[monstre].m_vientDeFrapper = NULL;
                        m_monstre[monstre].m_vientDAttaquer.x = -1;
                        m_monstre[monstre].m_degatsInflige  = 0;

                        int degats = m_monstre[monstre].Gerer(&m_ModeleMonstre[m_monstre[monstre].getModele()],temps);
                        if (degats>0)
                        {
                            if (m_monstre[monstre].m_miracleALancer == -1)
                            {
                                if (m_monstre[monstre].m_cible != NULL)
                                {
                                    if (m_monstre[monstre].m_shooter||(!m_monstre[monstre].m_shooter&&fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)<=1
                                                                       &&fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)<=1))
                                        if ((!m_monstre[monstre].m_friendly && rand() % 100 < (float)((float)(m_monstre[monstre].getCaracteristique().dexterite + 100)/(float)(m_monstre[monstre].m_cible->getCaracteristique().dexterite + 100))*25 )
                                        ||  ( m_monstre[monstre].m_friendly && rand() % 100 < (float)((float)(m_monstre[monstre].getCaracteristique().dexterite + 100)/(float)(m_monstre[monstre].m_cible->getCaracteristique().dexterite + 100))*50 ))
                                        {
                                            m_monstre[monstre].m_vientDeFrapper = m_monstre[monstre].m_cible;
                                            m_monstre[monstre].m_vientDAttaquer = m_monstre[monstre].m_cible->getCoordonnee();
                                            m_monstre[monstre].m_degatsInflige  = degats;

                                            m_monstre[monstre].m_cible->m_vientDetreTouche = &m_monstre[monstre];
                                            InfligerDegats(m_monstre[monstre].m_cible, &m_monstre[monstre], degats, 0, hero, 0);

                                            m_monstre[monstre].InfligerDegats(-degats * m_monstre[monstre].getCaracteristique().volVie, 4);
                                        }
                                }
                            }
                            else
                            {
                                m_monstre[monstre].m_miracleEnCours.push_back(EntiteMiracle ());
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                                m_monstre[monstre].m_miracleEnCours.back().m_modele=m_monstre[monstre].m_miracleALancer;

                                m_monstre[monstre].m_miracleALancer = -1;

                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.x=m_monstre[monstre].getCoordonneePixel().x;
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.y=m_monstre[monstre].getCoordonneePixel().y;

                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.x += m_monstre[monstre].m_entite_graphique.m_decalage.y*2 + m_monstre[monstre].m_entite_graphique.m_decalage.x;
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.y += m_monstre[monstre].m_entite_graphique.m_decalage.y*2 - m_monstre[monstre].m_entite_graphique.m_decalage.x;

                                m_monstre[monstre].m_miracleEnCours.back().m_coordonneeCible = hero->m_personnage.getProchaineCase();
                                m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_cible = m_monstre[monstre].m_cible;
                            }

                        }
                        m_monstre[monstre].m_nombreInvocation=0;

                        if(configuration->Lumiere)
                            moteurGraphique->LightManager->Generate(m_monstre[monstre].m_entite_graphique.m_light);

                        if(!m_monstre[monstre].m_noDistanceRestriction)
                            GererMiracle(&m_monstre[monstre],m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles,temps,hero);

                        if(m_monstre[monstre].m_doitMourir && m_monstre[monstre].EnVie() )
                            InfligerDegats(&m_monstre[monstre], NULL, m_monstre[monstre].getCaracteristique().vie, 4, hero, 0);
                    }
                }*/
            }
}


void Map::MusiquePlay()
{
    if (m_musiqueEnCours>=0 && m_musiqueEnCours<(int)m_musiques.size())
    {
        if (moteurSons->GetMusicStatus()==0)
        {
            m_musiqueEnCours++;
            if (m_musiqueEnCours >= (int)m_musiques.size())
                m_musiqueEnCours=0;

            if (m_musiqueEnCours >=0 && m_musiqueEnCours < (int)m_musiques.size())
                moteurSons->PlayNewMusic(m_musiques[m_musiqueEnCours]);
        }
    }
    else
        m_musiqueEnCours=0;
}

void Map::GererAmbiance(float temps)
{
    for(unsigned i = 0 ; i < m_climates.size() ; ++i)
        m_climates[i].Update(temps);

    for(unsigned i = 0 ; i < m_ambiances.size() ; ++i)
    {
        if(m_playAmbiances[i])
        {
            m_volumeAmbiances[i] += temps * 20;
            if(m_volumeAmbiances[i] > 100)
                m_volumeAmbiances[i] = 100;
        }
        else
        {
            m_volumeAmbiances[i] -= temps * 20;
            if(m_volumeAmbiances[i] < 0)
                m_volumeAmbiances[i] = 0;
        }
        moteurSons->JouerSon(m_ambiances[i],coordonnee (), 1, 0, m_volumeAmbiances[i]);
    }
}

bool Map::SetClimate(int no, bool actif)
{
    if(no >= 0 && no < m_climates.size())
    {
        if(actif)
        {
            if(m_climates[no].m_actif)
            {
                if(m_climates[no].GetState() == 1.0)
                {
                    m_climates[no].Continue();
                    return (true);
                }
            }
            else
            {
                m_climates[no].m_actif = true;
                return (true);
            }
        }
        else
        {
            m_climates[no].Stop();
            return (true);
        }
    }

    return (false);
}

void Map::DelEffet(int no)
{
    if(no >= 0 && no < (int)m_effets.size())
        m_effets[no].m_actif = false;
}

void Map::GererProjectilesEtEffets(Jeu *jeu,Hero *hero,float temps)
{
    coordonnee vueMin,vueMax;

    vueMin.x=hero->m_personnage.getCoordonnee().x-15;
    vueMin.y=hero->m_personnage.getCoordonnee().y-15;
    vueMax.x=hero->m_personnage.getCoordonnee().x+15;
    vueMax.y=hero->m_personnage.getCoordonnee().y+15;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>=m_dimensions.x)
        vueMax.x=m_dimensions.x-1;
    if (vueMax.y>=m_dimensions.y)
        vueMax.y=m_dimensions.y-1;

    if (!m_projectile.empty())
    {
        int nombreInactif=0;
        int i=0;
        for (std::vector <Projectile>::iterator projectile = m_projectile.begin();
                projectile != m_projectile.end();++i, ++projectile)
        {
            projectile->m_dejaDeplace=false;
            if (projectile->m_supprime)
                nombreInactif++;

            if (!projectile->m_dejaDeplace)
            {
                if (projectile->m_actif)
                {
                    projectile->m_dejaDeplace=true;

                    int temp=i;
                    m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].delProjectile(i);

                    if (projectile->m_position.y/COTE_TILE >= vueMin.y
                     && projectile->m_position.y/COTE_TILE <  vueMax.y
                     && projectile->m_position.x/COTE_TILE >= vueMin.x
                     && projectile->m_position.x/COTE_TILE <  vueMax.x)
                    {
                        if (projectile->m_cible.x == -1 && projectile->m_cible.y == -1)
                        {
                            coordonnee temp_pos = projectile->m_positionCase;

                            projectile->m_positionCase.y=(int)((projectile->m_position.y+32)/COTE_TILE);
                            projectile->m_positionCase.x=(int)((projectile->m_position.x+32)/COTE_TILE);

                            if (!m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre().empty())
                            {
                                bool collision = false;
                                for (unsigned o = 0 ; o < m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre().size() && !collision ; ++o)
                                    if ( m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o] < (int)m_monstre.size())
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].EnVie())
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].m_friendly == projectile->m_monstre)
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].getCaracteristique().niveau >= 0)
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].m_selectable)
                                        if(m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]].m_actif)
                                        {
                                            collision = true;
                                            if(rand()%100 > projectile->m_transperce)
                                                projectile->m_actif=false;
                                            projectile->m_entite_cible = &m_monstre[m_decor[1][(int)(projectile->m_positionCase.y)][(int)(projectile->m_positionCase.x)].getMonstre()[o]];
                                        }
                            }

                            projectile->m_positionCase = temp_pos;

                            if (hero->m_personnage.getCoordonnee().x==(int)((projectile->m_position.x+32)/COTE_TILE)
                             && hero->m_personnage.getCoordonnee().y==(int)((projectile->m_position.y+32)/COTE_TILE)
                             && projectile->m_monstre)
                            {
                                if(rand()%100 > projectile->m_transperce)
                                    projectile->m_actif=false;
                                projectile->m_entite_cible = &hero->m_personnage;
                            }

                            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                            if (p->m_personnage.getCoordonnee().x==(int)((projectile->m_position.x+32)/COTE_TILE)
                             && p->m_personnage.getCoordonnee().y==(int)((projectile->m_position.y+32)/COTE_TILE)
                             && projectile->m_monstre)
                            {
                                if(rand()%100 > projectile->m_transperce)
                                    projectile->m_actif=false;
                                projectile->m_entite_cible = &p->m_personnage;
                            }
                        }
                        else
                        {
                            bool x = false;
                            bool y = false;

                            coordonnee temp_pos = projectile->m_positionCase;

                            projectile->m_positionCase.y=(int)((projectile->m_position.y+16)/COTE_TILE);
                            projectile->m_positionCase.x=(int)((projectile->m_position.x+16)/COTE_TILE);

                            if ( projectile->m_positionCase.x >= projectile->m_cible.x && projectile->m_cible.x >= projectile->m_depart.x)
                                x = true;
                            else if ( projectile->m_positionCase.x <= projectile->m_cible.x && projectile->m_cible.x <= projectile->m_depart.x)
                                x = true;

                            if ( projectile->m_positionCase.y >= projectile->m_cible.y && projectile->m_cible.y >= projectile->m_depart.y)
                                y = true;
                            else if ( projectile->m_positionCase.y <= projectile->m_cible.y && projectile->m_cible.y <= projectile->m_depart.y)
                                y = true;

                            if (x && y)
                                projectile->m_actif = false;

                            projectile->m_positionCase = temp_pos;
                        }

                        if (getTypeCase((int)(projectile->m_positionCase.x),(int)(projectile->m_positionCase.y))==1

                        && (projectile->m_position.y + 32 > ((float)projectile->m_positionCase.y+0.25)*COTE_TILE
                          ||getTypeCase((int)(projectile->m_positionCase.x),(int)(projectile->m_positionCase.y-1))==1)
                        && (projectile->m_position.y + 32 < ((float)projectile->m_positionCase.y+0.75)*COTE_TILE
                          ||getTypeCase((int)(projectile->m_positionCase.x),(int)(projectile->m_positionCase.y+1))==1)

                        && (projectile->m_position.x + 32 > ((float)projectile->m_positionCase.x+0.25)*COTE_TILE
                          ||getTypeCase((int)(projectile->m_positionCase.x-1),(int)(projectile->m_positionCase.y))==1)
                        && (projectile->m_position.x + 32 < ((float)projectile->m_positionCase.x+0.75)*COTE_TILE
                          ||getTypeCase((int)(projectile->m_positionCase.x+1),(int)(projectile->m_positionCase.y))==1)

                       /* && (projectile->m_position.y - (projectile->m_positionCase.y+0.5)*COTE_TILE) *
                           (projectile->m_position.y - (projectile->m_positionCase.y+0.5)*COTE_TILE) +
                           (projectile->m_position.x - (projectile->m_positionCase.x+0.5)*COTE_TILE) *
                           (projectile->m_position.x - (projectile->m_positionCase.x+0.5)*COTE_TILE) < COTE_TILE * COTE_TILE * 0.5*/)
                            projectile->m_actif=false;

                        if (projectile->m_actif)
                        {
                            projectile->m_positionCase.y=(int)((projectile->m_position.y+48)/COTE_TILE);
                            projectile->m_positionCase.x=(int)((projectile->m_position.x+48)/COTE_TILE);

                            m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].setProjectile(temp);
                        }
                    }
                    else
                        projectile->m_actif=false;

                    projectile->Deplacer(temps);
                }
                else
                {
                    moteurGraphique->LightManager->Delete_Light(projectile->m_effet.m_light);
                    m_decor[1][projectile->m_positionCase.y][projectile->m_positionCase.x].delProjectile(i);
                }
            }
        }
        if (nombreInactif==(int)m_projectile.size())
            m_projectile.clear();
    }

    if (!m_effets.empty())
    {
        int nombreInactif=0;
        for (int i=0;i<(int)m_effets.size();++i)
            if (!m_effets[i].m_actif)
            {
                if(m_effets[i].m_position_case.x >= 0
                && m_effets[i].m_position_case.x <  m_dimensions.x
                && m_effets[i].m_position_case.y >= 0
                && m_effets[i].m_position_case.y <  m_dimensions.y)
                    m_decor[1][m_effets[i].m_position_case.y][m_effets[i].m_position_case.x].delEffetGraphique(i);

                nombreInactif++;
                moteurGraphique->LightManager->Delete_Light(m_effets[i].m_light);
            }
        if (nombreInactif==(int)m_effets.size())
            m_effets.clear();
    }
}

bool Map::DistanceWithHeros(Jeu *jeu, coordonnee pos, int t)
{
    if(fabs(jeu->hero.m_personnage.getCoordonnee().x - pos.x) < t
    && fabs(jeu->hero.m_personnage.getCoordonnee().y - pos.y) < t)
        return true;

    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin();
         p != jeu->m_personnageClients.end(); ++p)
    if(fabs(p->m_personnage.getCoordonnee().x - pos.x) < t
    && fabs(p->m_personnage.getCoordonnee().y - pos.y) < t)
        return true;

    return false;
}

void Map::GererMonstres(Jeu *jeu,Hero *hero,float temps,Menu *menu)
{
    sf::Packet packet;
    int nbr = 0;

    //hero->m_cibleInt = -1;

    for(std::vector<Monstre>::iterator Iter_monstre = m_monstre.begin();Iter_monstre!=m_monstre.end();++Iter_monstre) {
    /*if((fabs(hero->m_personnage.getCoordonnee().x - Iter_monstre->getCoordonnee().x) < 20
     && fabs(hero->m_personnage.getCoordonnee().y - Iter_monstre->getCoordonnee().y) < 20)*/

    if(DistanceWithHeros(jeu,Iter_monstre->getCoordonnee(),20)
    || Iter_monstre->m_noDistanceRestriction)
    {
        int monstre = -1;
        int x = (int)(Iter_monstre->getCoordonneePixel().x/COTE_TILE + 0.5);
        int y = (int)(Iter_monstre->getCoordonneePixel().y/COTE_TILE + 0.5);

        if(x >= 0 && y >= 0)
        if(x <  m_dimensions.x && y < m_dimensions.y)
            for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; o++)
                if(&m_monstre[m_decor[1][y][x].getMonstre()[o]] == &*Iter_monstre)
                    monstre = m_decor[1][y][x].getMonstre()[o];

        //if(hero->m_personnage.m_cible == &*Iter_monstre)
          //  hero->m_cibleInt = monstre;

        if(!configuration->hote)
            Iter_monstre->EmulerDeplacement(temps);

        if (monstre>=0&&monstre<(int)m_monstre.size())
        {
            m_monstre[monstre].m_vientDeFrapper = NULL;
            m_monstre[monstre].m_vientDAttaquer.x = -1;
            m_monstre[monstre].m_degatsInflige  = 0;

            int degats = m_monstre[monstre].Gerer(&m_ModeleMonstre[m_monstre[monstre].getModele()],temps,true);
            if (degats>0 || (!configuration->hote && m_monstre[monstre].m_miracleALancer >= 0))
            {
                if (m_monstre[monstre].m_miracleALancer == -1)
                {
                    if (m_monstre[monstre].m_cible != NULL)
                    {
                        if (m_monstre[monstre].m_shooter||(!m_monstre[monstre].m_shooter&&fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)<=1
                                                           &&fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)<=1))
                            if ((!m_monstre[monstre].m_friendly && rand() % 100 < (float)((float)(m_monstre[monstre].getCaracteristique().dexterite + 100)/(float)(m_monstre[monstre].m_cible->getCaracteristique().dexterite + 100))*25 )
                            ||  ( m_monstre[monstre].m_friendly && rand() % 100 < (float)((float)(m_monstre[monstre].getCaracteristique().dexterite + 100)/(float)(m_monstre[monstre].m_cible->getCaracteristique().dexterite + 100))*50 ))
                            {
                                m_monstre[monstre].m_vientDeFrapper = m_monstre[monstre].m_cible;
                                m_monstre[monstre].m_vientDAttaquer = m_monstre[monstre].m_cible->getCoordonnee();
                                m_monstre[monstre].m_degatsInflige  = degats;

                                m_monstre[monstre].m_cible->m_vientDetreTouche = &m_monstre[monstre];
                                InfligerDegats(m_monstre[monstre].m_cible, &m_monstre[monstre], degats, 0, jeu, 0);

                                m_monstre[monstre].InfligerDegats(-degats * m_monstre[monstre].getCaracteristique().volVie, 4);
                            }
                    }
                }
                else
                {
                    net->SendUseMiracle(monstre,m_monstre[monstre].m_miracleALancer);

                    m_monstre[monstre].m_miracleEnCours.push_back(EntiteMiracle ());
                    m_monstre[monstre].m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                    m_monstre[monstre].m_miracleEnCours.back().m_modele=m_monstre[monstre].m_miracleALancer;

                    m_monstre[monstre].m_miracleALancer = -1;

                    m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.x=m_monstre[monstre].getCoordonneePixel().x;
                    m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.y=m_monstre[monstre].getCoordonneePixel().y;

                    m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.x += m_monstre[monstre].m_entite_graphique.m_decalage.y*2 + m_monstre[monstre].m_entite_graphique.m_decalage.x;
                    m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_position.y += m_monstre[monstre].m_entite_graphique.m_decalage.y*2 - m_monstre[monstre].m_entite_graphique.m_decalage.x;

                    if (m_monstre[monstre].m_cible != NULL)
                        m_monstre[monstre].m_miracleEnCours.back().m_coordonneeCible = m_monstre[monstre].m_cible->getProchaineCase();
                    else
                        m_monstre[monstre].m_miracleEnCours.back().m_coordonneeCible = m_monstre[monstre].getProchaineCase();

                    m_monstre[monstre].m_miracleEnCours.back().m_infos.back()->m_cible = m_monstre[monstre].m_cible;
                }

            }
            m_monstre[monstre].m_nombreInvocation=0;

            if(configuration->Lumiere)
                moteurGraphique->LightManager->Generate(m_monstre[monstre].m_entite_graphique.m_light);

            if(!m_monstre[monstre].m_noDistanceRestriction)
                GererMiracle(&m_monstre[monstre],m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles,temps,jeu);

            if(m_monstre[monstre].m_doitMourir && m_monstre[monstre].EnVie() )
                InfligerDegats(&m_monstre[monstre], NULL, m_monstre[monstre].getCaracteristique().vie, 4, jeu, 0);
        /*}

        if(monstre >= 0)
        {*/
            if(Iter_monstre->m_noDistanceRestriction)
                GererMiracle(&m_monstre[monstre],m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles,temps,jeu);

            if(Iter_monstre->m_inexistant)
                m_decor[1][y][x].delMonstre(monstre);
            else
            {
                if (Iter_monstre->m_cible != NULL)
                {
                    if (Iter_monstre->m_cible->EnVie())
                    {
                        if (Iter_monstre->m_friendly == Iter_monstre->m_cible->m_friendly)
                            Iter_monstre->setVu(0);
                    }
                    else
                        Iter_monstre->setVu(0);
                }
                else
                    Iter_monstre->setVu(0);

                if (Iter_monstre->getVu() == 0)
                    TestVisionMonstre(monstre, hero, jeu->m_personnageClients);

                if (Iter_monstre->m_cible != NULL)
                    Iter_monstre->TesterVision(Iter_monstre->m_cible->getCoordonnee());

                if (Iter_monstre->m_attente<=0)
                {
                    TesterPoussable(*Iter_monstre, temps, monstre);

                    bool seDeplacer = false;

                    if(Iter_monstre->m_actif && configuration->hote)
                        seDeplacer = Iter_monstre->SeDeplacer(temps*100);

                    if(configuration->hote)
                    if(m_monstre[monstre].m_miracleALancer >= 0)
                    if (m_monstre[monstre].EnVie())
                    {
                        if (seDeplacer)
                        {
                            m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());
                            if(m_monstre[monstre].m_cible)
                                m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonneePixel(),m_monstre[monstre].m_cible->getCoordonneePixel());
                        }
                        else
                            m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
                    }

                    Script *script=&Iter_monstre->m_scriptAI;
                    if ((int)script->m_instructions.size()>0)
                        for (int a=0;a<(int)script->m_instructions[0].m_valeurs.size();a++)
                            if (script->m_instructions[0].m_valeurs[a]>=0&&script->m_instructions[0].m_valeurs[a]<(int)script->m_instructions.size())
                               GererInstructions(jeu,script,(int)script->m_instructions[0].m_valeurs[a],monstre,hero,temps,menu,seDeplacer);

                    if (Iter_monstre->getErreurPathfinding())
                        Script_RandomDisplace(jeu,script,0,monstre,hero,temps,menu,seDeplacer);

                    if (seDeplacer)
                        Iter_monstre->Pathfinding(getAlentourDuPersonnage(jeu,Iter_monstre->getCoordonnee(),true));

                    Iter_monstre->m_touche = false;
                }
                else
                    Iter_monstre->m_attente-=temps*100;

                if (Iter_monstre->getErreurPathfinding())
                    Iter_monstre->m_compteur++;
                else
                    Iter_monstre->m_compteur=0;

                //if (Iter_monstre->EnVie())
                DeplacerMonstreCase(monstre, x, y);
            }

            //net->SendInfosMonstre(monstre, m_monstre[monstre]);

            if(jeu->m_net_send)
            {
                packet<<(sf::Int8)P_INFOSMONSTRE<<(sf::Int16)monstre<<*Iter_monstre;

                nbr++;
               /* if(nbr >= 100)
                {
                    net->SendInfosMonstre(packet);

                    nbr = 0;
                    packet.Clear();
                }*/
            }
        }
    }
    else
    {
        Iter_monstre->setVu(0);
        if(Iter_monstre->EnVie())
            Iter_monstre->regenererVie(Iter_monstre->getCaracteristique().maxVie * temps / 30);
    }

    }

    if(jeu->m_net_send && !packet.EndOfPacket())
        net->SendPacket(packet);

    jeu->m_net_send = false;
}


void Map::DeplacerMonstreCase(int monstre,int x,int y)
{
    if(monstre >= 0 && monstre < (int)m_monstre.size())
    if ((int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5) != y
     || (int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5) != x)
        if ( (int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5) >= 0
          && (int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5) < m_dimensions.x
          && (int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5) >= 0
          && (int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5) < m_dimensions.y)
        {
            m_decor[1][y][x].delMonstre(monstre);
            m_decor[1][(int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5)][(int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5)].setMonstre(monstre);
        }
}

void Map::GererScript(Jeu *jeu,Hero *hero,float temps,Menu *menu)
{
    if ((int)m_script.m_instructions.size()>0)
        for (int a=0;a<(int)m_script.m_instructions[0].m_valeurs.size();a++)
            if (m_script.m_instructions[0].m_valeurs[a]>=0&&m_script.m_instructions[0].m_valeurs[a]<(int)m_script.m_instructions.size())
                GererInstructions(jeu,&m_script,(int)m_script.m_instructions[0].m_valeurs[a],-1,hero,temps,menu,0);
}

void Map::TestVisionMonstre(int numero, Hero *hero,std::list<Hero> &players)
{
    if (numero >= 0 && numero < (int)m_monstre.size())
        for (int y = m_monstre[numero].getCoordonnee().y - 5 ;
                y < m_monstre[numero].getCoordonnee().y + 5 ; ++y)
            for (int x = m_monstre[numero].getCoordonnee().x - 5 ;
                    x < m_monstre[numero].getCoordonnee().x + 5 ; ++x)
                if (y >= 0 && x >= 0 && y < (int)m_decor[0].size() && x < (int)m_decor[0][0].size())
                {
                    if (m_monstre[numero].m_friendly)
                    {
                        for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o )
                        {
                            if (m_decor[1][y][x].getMonstre()[o] >= 0)
                                if (!m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie()
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].m_actif)
                                {
                                    if (m_monstre[numero].m_cible == NULL)
                                        m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    else
                                    {
                                        int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                        int x2 = m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y2 = m_monstre[m_decor[1][y][x].getMonstre()[o]].getCoordonnee().y - m_monstre[numero].getCoordonnee().y;
                                        if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                            m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    }
                                }
                        }
                    }
                    else
                    {
                        if(hero->m_personnage.EnVie())
                            if (hero->m_personnage.getCoordonnee().y == y && hero->m_personnage.getCoordonnee().x == x)
                            {
                                if (m_monstre[numero].m_cible == NULL)
                                    m_monstre[numero].m_cible = &hero->m_personnage;
                                else
                                {
                                    int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                    int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                    int x2 = x - m_monstre[numero].getCoordonnee().x;
                                    int y2 = y - m_monstre[numero].getCoordonnee().y;
                                    if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                        m_monstre[numero].m_cible = &hero->m_personnage;
                                }
                            }

                        for (std::list<Hero>::iterator p = players.begin(); p != players.end(); ++p)
                        if(p->m_personnage.EnVie())
                            if (p->m_personnage.getCoordonnee().y == y && p->m_personnage.getCoordonnee().x == x)
                            {
                                if (m_monstre[numero].m_cible == NULL)
                                    m_monstre[numero].m_cible = &p->m_personnage;
                                else
                                {
                                    int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                    int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                    int x2 = x - m_monstre[numero].getCoordonnee().x;
                                    int y2 = y - m_monstre[numero].getCoordonnee().y;
                                    if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                        m_monstre[numero].m_cible = &p->m_personnage;
                                }
                            }

                        for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o )
                        {
                            if (m_decor[1][y][x].getMonstre()[o] >= 0)
                                if (m_monstre[m_decor[1][y][x].getMonstre()[o]].m_friendly
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].EnVie()
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].getCaracteristique().niveau > 0
                                    && m_monstre[m_decor[1][y][x].getMonstre()[o]].m_actif)
                                {
                                    if (m_monstre[numero].m_cible == NULL)
                                        m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    else
                                    {
                                        int x1 = m_monstre[numero].m_cible->getCoordonnee().x - m_monstre[numero].getCoordonnee().x;
                                        int y1 = m_monstre[numero].m_cible->getCoordonnee().y - m_monstre[numero].getCoordonnee().y;

                                        int x2 = x - m_monstre[numero].getCoordonnee().x;
                                        int y2 = y - m_monstre[numero].getCoordonnee().y;
                                        if ( x1 * x1 + y1 * y1 > x2 * x2 + y2 * y2)
                                            m_monstre[numero].m_cible = &m_monstre[m_decor[1][y][x].getMonstre()[o]];
                                    }
                                }
                        }
                    }
                }
}

bool Map::TileVisible(int x,int y, coordonnee pos)
{
    int d = ((x - pos.x)*(x - pos.x) + (y - pos.y)*(y - pos.y));

    for(int i = 0 ; i < d ; ++i)
        if(getTypeCase((int)(pos.x + (x - pos.x)*i/d),(int)(pos.y + (y - pos.y)*i/d))==1)
            return false;

    return true;
}

bool Map::InfligerDegats(int numero, Personnage *cible, float degats, int type, Jeu *jeu, float temps)
{
    if (numero >= 0 && numero < (int)m_monstre.size())
    {
        return InfligerDegats(&m_monstre[numero], cible, degats, type, jeu, temps);

        if (!m_monstre[numero].EnVie())
            if (m_monstreIllumine == numero)
                m_monstreIllumine=-1;
    }
    return (false);
}

bool Map::InfligerDegats(Personnage *monstre, Personnage *cible, float degats, int type, Jeu *jeu, float temps)
{
    if(!configuration->hote)
    {
        if(cible == &jeu->hero.m_personnage)
            net->SendDegats(monstre->m_no,degats, type, temps);
        if(monstre == &jeu->hero.m_personnage)
            monstre->InfligerDegats(degats, type, temps);
    }

    if(configuration->hote || temps == 0)
    {
        float viePrecedente = monstre->getCaracteristique().vie;

        bool noMulti = true;

        for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
            if(monstre == &p->m_personnage)
            {
                noMulti = false;
                net->SendDegats(&*p, degats, type, temps);
            }

        if(noMulti)
            monstre->InfligerDegats(degats, type, temps);

        if(cible != NULL && monstre != &jeu->hero.m_personnage)
            monstre->m_cible = cible;

        for (int x=monstre->getCoordonnee().x-10;x<=10+monstre->getCoordonnee().x;x++)
            for (int y=monstre->getCoordonnee().y-10;y<=10+monstre->getCoordonnee().y;y++)
                if (x>=0&&y>=0&&x<m_dimensions.x&&y<m_dimensions.y)
                    for (unsigned o = 0 ; o < m_decor[1][y][x].getMonstre().size() ; ++o)
                        if (m_decor[1][y][x].getMonstre()[o]>=0&&m_decor[1][y][x].getMonstre()[o]<(int)m_monstre.size())
                            m_monstre[m_decor[1][y][x].getMonstre()[o]].setVu(1);

        if (!monstre->EnVie() && viePrecedente > 0 && monstre != &jeu->hero.m_personnage)
        {
            double m = 0;
            if(cible != NULL)
                m=atan2(monstre->getCoordonneePixel().y-cible->getCoordonneePixel().y,
                        monstre->getCoordonneePixel().x-cible->getCoordonneePixel().x);

            float angle;

            angle=(int)(0+(m*180)/M_PI);
            if (angle>=360)
                angle=0;
            if (angle<0)
                angle=360+angle;

            angle+=45;

            KillMonstre(monstre, (int)angle, degats, jeu);
            return 1;
        }
    }

    return 0;
}

void Map::KillMonstre(Personnage *monstre, int angle, float degats, Jeu *jeu)
{
    if(monstre)
    if(degats != 0)
    {
        float force=((degats*3)/monstre->getCaracteristique().maxVie)*10;

        if (force<7)
            force=7;
        if (force>20)
            force=20;

        if (configuration->particules&&m_ModeleMonstre[monstre->getModele()].m_particules>=0)
        {
            coordonnee position2;
            position2.x=(int)(((monstre->getCoordonneePixel().x-monstre->getCoordonneePixel().y)*64/COTE_TILE));
            position2.y=(int)(((monstre->getCoordonneePixel().x+monstre->getCoordonneePixel().y)*64/COTE_TILE)/2);

            sf::Color buffer(255,255,255);
            moteurGraphique->AjouterSystemeParticules(m_ModeleMonstre[monstre->getModele()].m_particules,position2,buffer,force,angle);
        }

        if(configuration->hote)
            monstre->Pousser(coordonneeDecimal(cos((float)angle*M_PI/180) * force * 0.1f,
                                               sin((float)angle*M_PI/180) * force * 0.1f));
    }

    if(monstre)
    if(!monstre->dejaMort)
    {
        monstre->dejaMort = true;

        if(jeu->hero.m_personnage.m_cible == monstre)
            jeu->hero.m_personnage.m_cible = NULL;
        for (int i=0;i<(int)monstre->m_miracleEnCours.size();++i)
        {
            for (int o=0;o<(int)monstre->m_miracleEnCours[i].m_infos.size();o++)
                if (monstre->m_miracleEnCours[i].m_infos[o]->m_effetEnCours>=0)
                    if (m_ModeleMonstre[monstre->getModele()].m_miracles[monstre->m_miracleEnCours[i].m_modele].m_effets[monstre->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type==INVOCATION)
                        if (monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet>=0&&monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet<(int)m_monstre.size())
                            InfligerDegats(monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet, monstre, m_monstre[monstre->m_miracleEnCours[i].m_infos[o]->m_IDObjet].getCaracteristique().vie, 4,jeu);
        }

        if (monstre->getCaracteristique().pointAme>0)
            jeu->hero.m_personnage.AjouterPointAme(monstre->getCaracteristique().pointAme);

        if (monstre->getCoordonnee().x>=0&&monstre->getCoordonnee().x<m_dimensions.x&&monstre->getCoordonnee().y>=0&&monstre->getCoordonnee().y<m_dimensions.y)
            for (int i=0;i<(int)monstre->getObjets().size();++i)
                m_decor[1][monstre->getCoordonnee().y][monstre->getCoordonnee().x].AjouterObjet(monstre->getObjets()[i]);

        if(configuration->hote)
            net->SendKillMonstre(monstre->m_no,angle,degats);
    }
}

bool Map::RamasserObjet(Hero *hero,bool enMain)
{
    coordonnee position;

    if (enMain)
        position=hero->m_personnage.getCoordonnee();
    else
        position=hero->getChercherSac();


    if (position.x>=0&&position.x<m_dimensions.x
            &&position.y>=0&&position.y<m_dimensions.y)
        if (m_objetPointe>=0&&m_objetPointe<m_decor[1][position.y][position.x].getNombreObjets())
        {
            Objet temp = *m_decor[1][position.y][position.x].getObjet(m_objetPointe);
            m_decor[1][position.y][position.x].supprimerObjet(m_objetPointe);

            if (hero->AjouterObjet(temp,enMain))
                return true;
            else
                m_decor[1][position.y][position.x].AjouterObjet(temp);
        }
    return false;
}


void Map::AjouterObjet(Objet objet)
{
    if (objet.getPosition().x>=0
      &&objet.getPosition().x<m_dimensions.x
      &&objet.getPosition().y>=0
      &&objet.getPosition().y<m_dimensions.y)
        m_decor[1][objet.getPosition().y][objet.getPosition().x].AjouterObjet(objet);
}

void Map::AjouterMonstre(Monstre monstre)
{
    m_monstre.push_back(monstre);
    m_monstre.back().m_no = m_monstre.size() - 1;
    if(m_monstre.back().getCoordonnee().x >= 0 && m_monstre.back().getCoordonnee().x < (int)m_decor[0][0].size()
    && m_monstre.back().getCoordonnee().y >= 0 && m_monstre.back().getCoordonnee().y < (int)m_decor[0].size())
        m_decor[1][m_monstre.back().getCoordonnee().y][m_monstre.back().getCoordonnee().x].setMonstre(m_monstre.size()-1);
}

void Map::AjouterModeleMonstre(Modele_Monstre monstre)
{
    m_ModeleMonstre.push_back(monstre);

    for(unsigned i = 0 ; i < m_ModeleMonstre.back().m_miracles.size() ; ++i)
            m_ModeleMonstre.back().m_miracles[i].RechargerTileset();
}

const coordonnee &Map::getSacPointe()
{
    return m_sacPointe;
}
int Map::getObjetPointe()
{
    return m_objetPointe;
}
int Map::getNombreObjets(coordonnee position)
{
    if (position.x>=0&&position.x<m_dimensions.x
            &&position.y>=0&&position.y<m_dimensions.y)
        return m_decor[1][position.y][position.x].getNombreObjets();
    else
        return 0;
}

int Map::getNombreMonstres()
{
    return m_monstre.size();
}

int Map::getNombreModeleMonstres()
{
    return m_ModeleMonstre.size();
}

const coordonnee &Map::getPositionMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return m_monstre[numeroMonstre].getCoordonnee();

    return m_monstre[0].getCoordonnee();
}

Monstre *Map::getEntiteMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_monstre.size())
        return &m_monstre[numeroMonstre];
    else
        return (NULL);
}

Modele_Monstre &Map::getModeleMonstre(int numeroMonstre)
{
    if (numeroMonstre>=0&&numeroMonstre<(int)m_ModeleMonstre.size())
        return m_ModeleMonstre[numeroMonstre];
    else
        return m_ModeleMonstre[0];
}

casePathfinding** Map::getAlentourDuPersonnage(Jeu *jeu, coordonnee positionPersonnage, bool monstre)
{
    casePathfinding **grille = new casePathfinding* [10];

    for (int y=positionPersonnage.y-5;y<positionPersonnage.y+5;y++)
    {
        grille[y-positionPersonnage.y+5] = new casePathfinding[10];
        for (int x=positionPersonnage.x-5;x<positionPersonnage.x+5;x++)
        {
            grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].hauteur=0;
            if (y>=0&&x>=0&&x<m_dimensions.x&&y<m_dimensions.y)
            {
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision=getCollision(x,y);
                //if(m_decor[0][y][x].getHauteur()>m_decor[1][y][x].getHauteur())
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].hauteur=m_decor[0][y][x].getHauteur();
                // else
                // grille[y-positionPersonnage.y+10][x-positionPersonnage.x+10].hauteur=m_decor[1][y][x].getHauteur();

                if(monstre)
                {
                    if(jeu->hero.m_personnage.getProchaineCase().x == x && jeu->hero.m_personnage.getProchaineCase().y == y)
                        grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision = true;
                    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                        if(p->m_personnage.getProchaineCase().x == x && p->m_personnage.getProchaineCase().y == y)
                            grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision = true;
                }
            }
            else
                grille[y-positionPersonnage.y+5][x-positionPersonnage.x+5].collision=1;
        }
    }

    return grille;
}

int Map::getMonstreIllumine()
{
    return m_monstreIllumine;
}

const coordonnee &Map::getDimensions()
{
    return m_dimensions;
}

bool Map::getCollision(int positionX,int positionY, int exception)
{
    if(positionX >= 0 && positionX < (int)m_decor[0][0].size()
    && positionY >= 0 && positionY < (int)m_decor[0].size())
    for (int i=0;i<2;++i)
    {
        if (m_decor[i][positionY][positionX].getTileset()>=0&&m_decor[i][positionY][positionX].getTileset()<(int)m_tileset.size())
            if (moteurGraphique->getTileset(m_tileset[m_decor[i][positionY][positionX].getTileset()])->getCollisionTile(m_decor[i][positionY][positionX].getTile()))
                return 1;

        if (i == 1)
        {

            for (unsigned o = 0 ; o < m_decor[i][positionY][positionX].getMonstre().size() ; ++o)
                if (m_decor[i][positionY][positionX].getMonstre()[o]>-1&&m_decor[i][positionY][positionX].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][positionY][positionX].getMonstre()[o] != exception)
                    if (m_monstre[m_decor[i][positionY][positionX].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][positionY][positionX].getMonstre()[o]].m_collision)
                        return 1;

            coordonnee enCours;

            enCours.x=positionX+1;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX+1;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX;
            enCours.y=positionY+1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX+1;
            enCours.y=positionY;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX-1;
            enCours.y=positionY;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
            enCours.x=positionX;
            enCours.y=positionY-1;
            if (enCours.x>=0&&enCours.y>=0&&enCours.x<m_dimensions.x&&enCours.y<m_dimensions.y)
                for (unsigned o = 0 ; o < m_decor[i][ enCours.y][enCours.x].getMonstre().size() ; ++o)
                    if (m_decor[i][ enCours.y][enCours.x].getMonstre()[o]>-1&&m_decor[i][ enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size() && m_decor[i][enCours.y][enCours.x].getMonstre()[o] != exception)
                        if (m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].EnVie()&&m_monstre[m_decor[i][ enCours.y][enCours.x].getMonstre()[o]].getCaracteristique().vitesse>0)
                            if (m_decor[i][enCours.y][enCours.x].getMonstre()[o]<(int)m_monstre.size())
                                if ((m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getProchaineCase().y==positionY)||(m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().x==positionX&&m_monstre[m_decor[i][enCours.y][enCours.x].getMonstre()[o]].getCoordonnee().y==positionY))
                                    return 1;
        }
    }
    return 0;
}

int Map::getTypeCase(int positionX,int positionY)
{
    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            //if(m_tileset[m_decor[0][positionY][positionX].getTileset()].getCollisionTile(m_decor[0][positionY][positionX].getTile()))
            if ((int)moteurGraphique->getTileset(m_tileset[m_decor[0][positionY][positionX].getTileset()])->getLumiereDuTile(m_decor[0][positionY][positionX].getTile()).hauteur>=32)
                return 1;

        if (m_decor[0][positionY][positionX].getNombreObjets())
            return 4;
    }
    else
        return 1;

    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
            //if(m_tileset[m_decor[1][positionY][positionX].getTileset()].getCollisionTile(m_decor[1][positionY][positionX].getTile()))
            if ((int)moteurGraphique->getTileset(m_tileset[m_decor[1][positionY][positionX].getTileset()])->getLumiereDuTile(m_decor[1][positionY][positionX].getTile()).hauteur>=32)
                return 1;

        for (unsigned o = 0 ; o < m_decor[1][positionY][positionX].getMonstre().size() ; ++o)
            if (m_decor[1][positionY][positionX].getMonstre()[o]>-1&&m_decor[1][positionY][positionX].getMonstre()[o]<(int)m_monstre.size())
                if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].EnVie())
                {
                    if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].m_friendly)
                        return 5;
                    else if (m_ModeleMonstre[m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].getModele()].m_minimap)
                        return 2;
                }

        if (m_decor[1][positionY][positionX].getNombreObjets())
            return 4;
    }
    else
        return 1;


    return 0;
}

bool Map::getCollisionPousse(int positionX,int positionY, int id)
{
    if (positionY>=0&&positionY<m_dimensions.y&&positionX>=0&&positionX<m_dimensions.x)
    {
        if (m_decor[0][positionY][positionX].getTileset()>=0&&m_decor[0][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(moteurGraphique->getTileset(m_tileset[m_decor[0][positionY][positionX].getTileset()])->getCollisionTile(m_decor[0][positionY][positionX].getTile()))
                return 1;

        if (m_decor[1][positionY][positionX].getTileset()>=0&&m_decor[1][positionY][positionX].getTileset()<(int)m_tileset.size())
            if(moteurGraphique->getTileset(m_tileset[m_decor[1][positionY][positionX].getTileset()])->getCollisionTile(m_decor[1][positionY][positionX].getTile()))
                return 1;

        for (unsigned o = 0 ; o < m_decor[1][positionY][positionX].getMonstre().size() ; ++o)
            if (m_decor[1][positionY][positionX].getMonstre()[o]>-1&&m_decor[1][positionY][positionX].getMonstre()[o]<(int)m_monstre.size())
                if (m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].EnVie()
                 && m_monstre[m_decor[1][positionY][positionX].getMonstre()[o]].m_impenetrable
                 && m_decor[1][positionY][positionX].getMonstre()[o] != id)
                    return 1;
    }
    else
        return 1;
    return 0;
}

void Map::TesterPoussable(Personnage &personnage, float temps, int id)
{
    if(personnage.getPousse().x != 0 || personnage.getPousse().y != 0)
    if(getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                          (int)((personnage.getCoordonneePixel().y + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                           id))
    {
        if(personnage.getPousse().x != 0
        && !getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                               (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                id))
            personnage.setPousse(coordonneeDecimal(personnage.getPousse().x, 0));
        else if(personnage.getPousse().y != 0
             && !getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    (int)((personnage.getCoordonneePixel().y + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0) )/COTE_TILE),
                                    id))
            personnage.setPousse(coordonneeDecimal(0, personnage.getPousse().y));
        else if(personnage.getPousse().x != 0)
        {
            if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    (int)((personnage.getCoordonneePixel().y + personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                    id))
                personnage.setPousse(coordonneeDecimal(0, personnage.getPousse().x));
            else if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                        (int)((personnage.getCoordonneePixel().y - personnage.getPousse().x * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().x >= 0))/COTE_TILE),
                                        id))
                personnage.setPousse(coordonneeDecimal(0, -personnage.getPousse().x));
            else
                personnage.setPousse(coordonneeDecimal(0, 0));
        }
        else if(personnage.getPousse().y != 0)
        {
            if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x + personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                   (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                    id))
                personnage.setPousse(coordonneeDecimal(personnage.getPousse().y, 0));
            else if(!getCollisionPousse((int)((personnage.getCoordonneePixel().x - personnage.getPousse().y * temps * COTE_TILE * 5 + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                        (int)((personnage.getCoordonneePixel().y + COTE_TILE * 0.5f * (personnage.getPousse().y >= 0))/COTE_TILE),
                                        id))
                personnage.setPousse(coordonneeDecimal(-personnage.getPousse().y, 0));
            else
                personnage.setPousse(coordonneeDecimal(0, 0));
        }
    }
}

int Map::getMonstre(coordonnee casePointee)
{
    float   distance=100000;
    int     meilleur=-1;
    bool    meilleurIsFriendly = true;
    coordonnee vueMin,vueMax;

    vueMin.x = casePointee.x;
    vueMin.y = casePointee.y;
    vueMax.x = casePointee.x+4;
    vueMax.y = casePointee.y+4;

    if (vueMin.x<0)
        vueMin.x=0;
    if (vueMin.y<0)
        vueMin.y=0;
    if (vueMax.x>m_dimensions.x)
        vueMax.x=m_dimensions.x;
    if (vueMax.y>m_dimensions.y)
        vueMax.y=m_dimensions.y;

    m_sacPointe.x = -1;
    m_sacPointe.y = -1;

    for (int j=vueMin.y;j<vueMax.y;j++)
        for (int k=vueMin.x;k<vueMax.x;k++)
        {
            for (unsigned o = 0 ; o < m_decor[1][j][k].getMonstre().size() ; ++o)
                if (m_decor[1][j][k].getMonstre()[o]>=0
                 && m_decor[1][j][k].getMonstre()[o]<(int)m_monstre.size())
                {
                    int no = m_decor[1][j][k].getMonstre()[o];

                    if (m_monstre[no].EnVie()
                     && m_monstre[no].getCaracteristique().rang>=0
                     && m_monstre[no].m_actif
                     && m_monstre[no].m_selectable)
                    {
                        coordonneeDecimal temp;
                        temp.x =((m_monstre[no].getCoordonneePixel().x-m_monstre[no].getCoordonneePixel().y)*64/COTE_TILE);
                        temp.y =((m_monstre[no].getCoordonneePixel().x+m_monstre[no].getCoordonneePixel().y)*32/COTE_TILE)+32;
                        temp.x += m_monstre[no].m_entite_graphique.m_decalage.x;
                        temp.y += m_monstre[no].m_entite_graphique.m_decalage.y;

                        sf::Vector2f positionSourisTotale = moteurGraphique->getPositionSouris();

                        if(positionSourisTotale.x>temp.x-96
                        && positionSourisTotale.x<temp.x+96
                        && positionSourisTotale.y>temp.y-160
                        && positionSourisTotale.y<temp.y+32)
                        {
                            float temp2=0;
                            temp2=( (temp.x-(positionSourisTotale.x))
                                   *(temp.x-(positionSourisTotale.x))
                                   +(temp.y-(positionSourisTotale.y+24))
                                   *(temp.y-(positionSourisTotale.y+24)));

                            if ( (distance>temp2 && m_monstre[no].m_friendly == meilleurIsFriendly)
                               ||(m_monstre[no].m_friendly != true && meilleurIsFriendly == true) )
                            {
                                meilleurIsFriendly = m_monstre[no].m_friendly;
                                meilleur=no,distance=temp2;
                            }
                        }
                    }
                }
        }

    m_monstreIllumine=meilleur;
    return meilleur;
}

const  std::string  &Map::getNom()
{
    return m_nom_fichier;
}
