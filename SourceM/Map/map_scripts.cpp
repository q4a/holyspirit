#include "map.h"

#include <iostream>
#include <fstream>
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"


void Map::Script_RandomDisplace(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x
          &&m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
        {
            coordonnee arrivee;
            arrivee=m_monstre[monstre].getDepart();
            arrivee.x+=(3-rand()%(6));
            arrivee.y+=(3-rand()%(6));
            m_monstre[monstre].setArrivee(arrivee);
        }
    }
}

void Map::Script_Evasion(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (!m_monstre[monstre].frappeEnCours || (int)script->getValeur(noInstruction, 0))
        {
            if((m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x
             && m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y) || (int)script->getValeur(noInstruction, 0))
            {
                coordonnee arrivee;
                arrivee=m_monstre[monstre].getCoordonnee();
                arrivee.x+=(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x) * 3;
                arrivee.y+=(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y) * 3;

                if((TileVisible(arrivee.x, arrivee.y, m_monstre[monstre].getCoordonnee())
                && !getCollision(arrivee.x,arrivee.y)) || !(int)script->getValeur(noInstruction, 0))
                    m_monstre[monstre].setArrivee(arrivee);
                else
                {
                    arrivee.x-=(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x) * 6;
                    arrivee.y-=(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y) * 6;
                    if(TileVisible(arrivee.x, arrivee.y, m_monstre[monstre].getCoordonnee())
                    && !getCollision(arrivee.x,arrivee.y))
                        m_monstre[monstre].setArrivee(arrivee);
                }
            }
        }
    }
}

void Map::Script_Follow(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if (!m_monstre[monstre].frappeEnCours)
        {
            m_monstre[monstre].setArrivee(hero->m_personnage.getCoordonnee());

            if (m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x&&m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
                m_monstre[monstre].setDepart();
        }
    }
}

void Map::Script_Teleport(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].EnVie())
    {
        if((seDeplacer && !m_monstre[monstre].frappeEnCours) || (int)script->getValeur(noInstruction, 0))
        {
            m_decor[1]  [(int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5)]
                        [(int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5)].delMonstre(monstre);
            m_monstre[monstre].setCoordonnee(m_monstre[monstre].getArrivee());
            m_monstre[monstre].setDepart();
            m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());
            m_decor[1]  [(int)(m_monstre[monstre].getCoordonneePixel().y/COTE_TILE + 0.5)]
                        [(int)(m_monstre[monstre].getCoordonneePixel().x/COTE_TILE + 0.5)].setMonstre(monstre);
        }
    }
}



void Map::Script_UseMiracle(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    /*if (m_monstre[monstre].EnVie())
    {
        if (seDeplacer)
        {
            m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());

           // if (m_monstre[monstre].m_miracleALancer == -1)
             //   m_monstre[monstre].setEtat(2);

            //m_monstre[monstre].frappeEnCours = true;

            if(m_monstre[monstre].m_cible)
                m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonneePixel(),m_monstre[monstre].m_cible->getCoordonneePixel());
        }
        else
            m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
    }*/

    if (m_monstre[monstre].m_miracleALancer == -1)
    {
        m_monstre[monstre].m_miracleALancer = (int)script->getValeur(noInstruction, 0);
        jeu->SendUseMiracle(monstre,(int)script->getValeur(noInstruction, 0));
    }
}

void Map::Script_SetState(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getEtat()!=script->getValeur(noInstruction, 0))
        m_monstre[monstre].setJustEtat((int)script->getValeur(noInstruction, 0)), m_monstre[monstre].m_etatForce = true;
    if(script->m_instructions[noInstruction].m_valeurs.size() >= 2)
        m_monstre[monstre].setPose((int)script->getValeur(noInstruction, 1));
}




void Map::Script_PlaySound(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (m_monstre[monstre].getModele()>=0&&m_monstre[monstre].getModele()<(int)m_ModeleMonstre.size())
    {
        coordonnee position;
        position.x=(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].getCoordonnee().y-1)/5;
        position.y=(m_monstre[monstre].getCoordonnee().x+m_monstre[monstre].getCoordonnee().y)/5;
        m_monstre[monstre].m_entite_graphique.m_tileset->JouerSon((int)script->getValeur(noInstruction, 0),position,true);
    }
}

void Map::Script_Shoot(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
    if (seDeplacer)
    {
        m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());

        m_monstre[monstre].m_miracleALancer=-1;
        if(m_monstre[monstre].m_cible)
            m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonneePixel(),m_monstre[monstre].m_cible->getCoordonneePixel());
        m_monstre[monstre].m_shooter=true;
    }
}

void Map::Script_Fight(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (!m_monstre[monstre].getErreurPathfinding())
    {
        if (m_monstre[monstre].EnVie())
        if (m_monstre[monstre].m_miracleALancer == -1)
        {
            coordonnee arrivee;

            if (seDeplacer)
            {
                m_monstre[monstre].setDepart();

                if (m_monstre[monstre].m_cible
                 &&(fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)>1
                 || fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)>1))
                    m_monstre[monstre].setArrivee(m_monstre[monstre].m_cible->getProchaineCase());
                else
                {

                    if (m_monstre[monstre].m_cible && m_monstre[monstre].m_cible->EnVie()<=0)
                        m_monstre[monstre].setVu(0);
                    if (!m_monstre[monstre].frappeEnCours && !m_monstre[monstre].m_etatForce)
                        m_monstre[monstre].setEtat(2);



                    coordonneeDecimal new_coord = m_monstre[monstre].getCoordonneePixel();

                    if(m_monstre[monstre].m_cible->getCoordonnee().x == m_monstre[monstre].getCoordonnee().x - 1
                    && m_monstre[monstre].m_cible->getCoordonnee().y == m_monstre[monstre].getCoordonnee().y + 1)
                    {
                        if(new_coord.x > m_monstre[monstre].getCoordonnee().x * COTE_TILE - COTE_TILE/4)
                            new_coord.x -= temps * 30;

                        if(new_coord.y < m_monstre[monstre].getCoordonnee().y * COTE_TILE + COTE_TILE/4)
                            new_coord.y += temps * 30;
                    }
                    if(m_monstre[monstre].m_cible->getCoordonnee().x == m_monstre[monstre].getCoordonnee().x + 1
                    && m_monstre[monstre].m_cible->getCoordonnee().y == m_monstre[monstre].getCoordonnee().y - 1)
                    {
                        if(new_coord.x < m_monstre[monstre].getCoordonnee().x * COTE_TILE + COTE_TILE/4)
                            new_coord.x += temps * 30;

                        if(new_coord.y > m_monstre[monstre].getCoordonnee().y * COTE_TILE - COTE_TILE/4)
                            new_coord.y -= temps * 30;
                    }

                    m_monstre[monstre].setCoordonneePixel2(new_coord);

                    if(m_monstre[monstre].m_cible)
                        m_monstre[monstre].Frappe(m_monstre[monstre].getCoordonneePixel(),m_monstre[monstre].m_cible->getCoordonneePixel());
                    m_monstre[monstre].setArrivee(m_monstre[monstre].getCoordonnee());
                    m_monstre[monstre].m_shooter=false;
                }
            }

            if(m_monstre[monstre].m_cible)
                if (fabs(m_monstre[monstre].getCoordonnee().x-m_monstre[monstre].m_cible->getCoordonnee().x)>1
                 || fabs(m_monstre[monstre].getCoordonnee().y-m_monstre[monstre].m_cible->getCoordonnee().y)>1)
                {
                    if(m_monstre[monstre].getArrivee().x==m_monstre[monstre].getCoordonnee().x
                    && m_monstre[monstre].getArrivee().y==m_monstre[monstre].getCoordonnee().y)
                        m_monstre[monstre].setArrivee(m_monstre[monstre].m_cible->getProchaineCase());
                }
        }
    }
}

void Map::Script_Trade(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
   // jeu->hero.m_personnage.m_cible = NULL;
    bool trader = true;
    if(!script->m_instructions[noInstruction].m_valeurs.empty())
        if(script->getValeur(noInstruction, 0) == 1)
            jeu->m_inventaire->setTrader(&hero->m_coffre), trader = false;

    if(trader)
        jeu->m_inventaire->setTrader(m_monstre[monstre].getPointeurObjets());

    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_inventaire;
    jeu->menu.m_cur_talk_hauteur = jeu->hero.m_classe.talk.position.h;
}

void Map::Script_Potale(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    //jeu->hero.m_personnage.m_cible = NULL;

    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_potales;
    jeu->menu.m_cur_talk_hauteur = jeu->hero.m_classe.talk.position.h;
}

void Map::Script_Craft(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    //jeu->hero.m_personnage.m_cible = NULL;

    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_craft;
    jeu->menu.m_cur_talk_hauteur = jeu->hero.m_classe.talk.position.h;
}

void Map::Script_Bless(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    //jeu->hero.m_personnage.m_cible = NULL;

    eventManager->StopEvenement(sf::Mouse::Left, EventClic);
    eventManager->StopEvenement(sf::Mouse::Left, EventClicA);
    jeu->Clock.Reset();
    jeu->m_contexte=jeu->m_bless;
    jeu->menu.m_cur_talk_hauteur = jeu->hero.m_classe.talk.position.h;
}

std::string DecouperTexte(std::string texte, int tailleCadran, int tailleTexte)
{
    sf::Text temp;
    temp.SetCharacterSize(tailleTexte);
    temp.SetFont(moteurGraphique->m_font);

    std::string buf;
    std::string bufMot = "";
    for (int p = 0;p < (int)texte.size();p++)
    {
        if (texte[p] != ' ' && texte[p] != '\0' && texte[p] != '\n')
        {
            if(texte[p] == '\\')
                texte[p] = '\n';
            bufMot += texte[p];

            temp.SetString(buf + bufMot);
            if (temp.GetRect().Width > tailleCadran)
                bufMot = '\n' + bufMot.substr(1,bufMot.size()-1);
        }
        else
        {
            buf += bufMot;
            if(texte[p] == '\0' || texte[p] == '\n')
                bufMot = "";
            else
                bufMot = " ";
        }

    }
    buf += bufMot;

    return buf;
}


void Map::GererInstructions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        if (script->m_instructions[noInstruction].nom=="fight" && monstre != -1 && configuration->hote)
            Script_Fight(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="evasion" && monstre != -1 && configuration->hote)
            Script_Evasion(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="followHero" && monstre != -1 && configuration->hote)
            Script_Follow(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="teleport" && monstre != -1 && (configuration->hote || !configuration->hote))
            Script_Teleport(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="useMiracle" && monstre != -1 && (configuration->hote /*|| !configuration->hote*/))
            Script_UseMiracle(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="stopMiracle" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            for (int i=0;i<(int)m_monstre[monstre].m_miracleEnCours.size();++i)
            if(m_monstre[monstre].m_miracleEnCours[i].m_modele == script->getValeur(noInstruction, 0))
            {
                for (int o=0;o<(int)m_monstre[monstre].m_miracleEnCours[i].m_infos.size();o++)
                    if (m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours>=0)
                        if (m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type==EFFET)
                            m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_cible->m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet].m_effet.m_actif = false;
            }

        }
        else if (script->m_instructions[noInstruction].nom=="setState" && monstre != -1 && configuration->hote)
            Script_SetState(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="dammages" && monstre != -1 && (configuration->hote || !configuration->hote))
            InfligerDegats(monstre, NULL, script->getValeur(noInstruction, 0), 4, jeu, false);
        else if (script->m_instructions[noInstruction].nom=="shoot" && monstre != -1 && configuration->hote)
            Script_Shoot(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="randomDisplace" && monstre != -1 && configuration->hote)
            Script_RandomDisplace(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="setInvocationID" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) >= 0)
            for (int i=0;i<(int)m_monstre[monstre].m_miracleEnCours.size();++i)
            {
                for (int o=0;o<(int)m_monstre[monstre].m_miracleEnCours[i].m_infos.size();o++)
                    if (m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours>=0)
                        if (m_ModeleMonstre[m_monstre[monstre].getModele()].m_miracles[m_monstre[monstre].m_miracleEnCours[i].m_modele].m_effets[m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type==INVOCATION)
                            if (m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet>=0&&m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet<(int)m_monstre.size())
                            {
                                if((int)m_listID.size() <= (int)script->getValeur(noInstruction, 0))
                                    m_listID.resize((int)script->getValeur(noInstruction, 0) + 1);

                                bool add = true;
                                for(unsigned j = 0 ; j < m_listID[(int)script->getValeur(noInstruction, 0)].size() ; ++j)
                                    if(m_listID[(int)script->getValeur(noInstruction, 0)][j] == m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet)
                                        add = false;
                                if(add)
                                    m_listID[(int)script->getValeur(noInstruction, 0)].push_back(m_monstre[monstre].m_miracleEnCours[i].m_infos[o]->m_IDObjet);
                            }
            }
        }
        else if (script->m_instructions[noInstruction].nom=="setActif" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            m_monstre[monstre].m_actif = script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="setSpeed" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            Caracteristique temp = m_monstre[monstre].getCaracteristique();
            temp.vitesse = script->getValeur(noInstruction, 0)/100;
            m_monstre[monstre].setCaracteristique(temp);
        }
        else if (script->m_instructions[noInstruction].nom=="look_hero" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            float m=atan2(-(double)(hero->m_personnage.getCoordonneePixel().y-m_monstre[monstre].getCoordonneePixel().y),
                           (double)(hero->m_personnage.getCoordonneePixel().x-m_monstre[monstre].getCoordonneePixel().x));
            m-=M_PI/4;

            m_monstre[monstre].setAngle((int)(m*180/M_PI));
        }
        else if (script->m_instructions[noInstruction].nom=="gift_all_items" && monstre != -1)
        {
            if (m_monstre[monstre].getCoordonnee().x>=0
              &&m_monstre[monstre].getCoordonnee().x<m_dimensions.x
              &&m_monstre[monstre].getCoordonnee().y>=0
              &&m_monstre[monstre].getCoordonnee().y<m_dimensions.y)
                for (int i=0;i<(int)m_monstre[monstre].getObjets().size();++i)
                    m_decor[1][m_monstre[monstre].getCoordonnee().y][m_monstre[monstre].getCoordonnee().x].AjouterObjet(m_monstre[monstre].getObjets()[i]);
        }
        else if (script->m_instructions[noInstruction].nom=="set_entityActif" && monstre == -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < m_monstre.size())
                m_monstre[(int)script->getValeur(noInstruction, 0)].m_actif = (bool)script->getValeur(noInstruction, 1);
        }
        else if (script->m_instructions[noInstruction].nom=="playSound" && monstre != -1)
            Script_PlaySound(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer); //PLAYSOUND(script->getValeur(noInstruction, 0))
        else if (script->m_instructions[noInstruction].nom=="trade" && monstre != -1)
            Script_Trade(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="craft" && monstre != -1)
            Script_Craft(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="bless" && monstre != -1)
            Script_Bless(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else  if (script->m_instructions[noInstruction].nom=="setFriendly" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            m_monstre[monstre].m_friendly = script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="teleportation_menu")
            Script_Potale(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="add_checkpoint")
        {
            hero->addPotale((int)script->getValeur(noInstruction, 0),
                            (int)script->getValeur(noInstruction, 1),
                            (int)script->getValeur(noInstruction, 2),
                            script->m_instructions[noInstruction].valeurString);
        }
        else if (script->m_instructions[noInstruction].nom=="if")
            GererConditions(jeu,script,noInstruction,monstre,hero,temps,menu,seDeplacer);
        else if (script->m_instructions[noInstruction].nom=="variable" && (configuration->hote || !configuration->hote))
            script->setVariable((int)script->getValeur(noInstruction, 0), script->getValeur(noInstruction, 1));
        else if (script->m_instructions[noInstruction].nom=="incrementVariable" && (configuration->hote || !configuration->hote))
            script->setVariable((int)script->getValeur(noInstruction, 0),
                                script->getVariable((int)script->getValeur(noInstruction, 0)) + script->getValeur(noInstruction, 1));
        else if (script->m_instructions[noInstruction].nom=="setCollision" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            m_monstre[monstre].m_collision = script->getValeur(noInstruction, 0);
            m_monstre[monstre].m_impenetrable = script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="regenerate_inventory" && monstre != -1)
        {
            if(m_monstre[monstre].getModele() >= 0 && m_monstre[monstre].getModele() < (int)m_ModeleMonstre.size())
                m_monstre[monstre].GenererInventaire(&m_ModeleMonstre[m_monstre[monstre].getModele()]);

            TrierInventaire(m_monstre[monstre].getPointeurObjets(),hero->m_classe.position_contenu_marchand.w);
        }
        else if (script->m_instructions[noInstruction].nom=="tell" && monstre != -1)
        {
            m_monstre[monstre].m_speak      = configuration->getText(4, (int)script->getValeur(noInstruction, 0));
            m_monstre[monstre].m_speak_time = script->getValeur(noInstruction, 1);
        }
        else if (script->m_instructions[noInstruction].nom=="speak")
        {
            if (jeu->menu.m_dialogue.empty())
            {
                jeu->menu.m_dialogue = DecouperTexte(configuration->getText(4, (int)script->getValeur(noInstruction, 0)),
                                                                            hero->m_classe.position_contenu_dialogue.w, 14);
                eventManager->StopEvenement(Mouse::Left,EventClic);

                hero->m_personnage.setArrivee(hero->m_personnage.getProchaineCase());
                if(monstre >= 0 && monstre < (int)m_monstre.size())
                    m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());

                jeu->menu.m_dialogue_position = hero->m_personnage.getCoordonnee();
                //hero->m_personnage.m_cible = NULL;

                jeu->menu.m_forced_dialogue = (int)script->getValeur(noInstruction, 1);
            }
        }
        else if (script->m_instructions[noInstruction].nom=="stop_speak")
        {
            jeu->menu.m_dialogue.clear();
            jeu->menu.ClearSpeakChoice();
        }
        else if (script->m_instructions[noInstruction].nom=="speak_choice")
        {
            jeu->menu.AddSpeakChoice(   DecouperTexte(configuration->getText(4, (int)script->getValeur(noInstruction, 0)),
                                                      hero->m_classe.position_contenu_dialogue.w, 14),
                                        (int)script->getValeur(noInstruction, 1));
            eventManager->StopEvenement(Mouse::Left,EventClic);
            hero->m_personnage.setArrivee(hero->m_personnage.getProchaineCase());

            m_monstre[monstre].setArrivee(m_monstre[monstre].getProchaineCase());
        }
        else if (script->m_instructions[noInstruction].nom=="newQuest")
        {
            hero->NewQuest((int)script->getValeur(noInstruction, 0));
            jeu->SendQuest((int)script->getValeur(noInstruction, 0), 0);
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestName")
        {
            hero->SetQuestName((int)script->getValeur(noInstruction, 0),
                           (int)script->getValeur(noInstruction, 1));
            jeu->SendQuest((int)script->getValeur(noInstruction, 0), 1,
                           (int)script->getValeur(noInstruction, 1));
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestState")
        {
            hero->SetQuestState((int)script->getValeur(noInstruction, 0),
                           (int)script->getValeur(noInstruction, 1),
                           (int)script->getValeur(noInstruction, 2));

            jeu->SendQuest((int)script->getValeur(noInstruction, 0), 2,
                           (int)script->getValeur(noInstruction, 1),
                           (int)script->getValeur(noInstruction, 2));
        }
        else if (script->m_instructions[noInstruction].nom=="setQuestActif")
        {
            hero->SetQuestActif((int)script->getValeur(noInstruction, 0),
                           (int)script->getValeur(noInstruction, 1));
            jeu->SendQuest((int)script->getValeur(noInstruction, 0), 3,
                           (int)script->getValeur(noInstruction, 1));
        }
        else if (script->m_instructions[noInstruction].nom=="giftItem" && monstre != -1)
        {
            if (script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < (int)(*m_monstre[monstre].getPointeurObjets()).size())
                if (!hero->AjouterObjet((*m_monstre[monstre].getPointeurObjets())[(unsigned)script->getValeur(noInstruction, 0)], false))
                    m_decor[1][hero->m_personnage.getCoordonnee().y][hero->m_personnage.getCoordonnee().x].AjouterObjet((*m_monstre[monstre].getPointeurObjets())[(unsigned)script->getValeur(noInstruction, 0)]);
        }
        else if (script->m_instructions[noInstruction].nom=="stop_talk")
        {
            jeu->hero.m_personnage.m_cible = NULL;
            eventManager->StopEvenement(Mouse::Left,EventClic);
            eventManager->StopEvenement(Mouse::Left,EventClicA);
        }
        else if (script->m_instructions[noInstruction].nom=="stop_interact")
        {
            if(hero->m_cibleInt == monstre)
            {
                jeu->hero.m_personnage.m_cible = NULL;
                eventManager->StopEvenement(Mouse::Left,EventClic);
                eventManager->StopEvenement(Mouse::Left,EventClicA);
                hero->m_cibleInt = -1;
            }

            for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                if (p->m_cibleInt == monstre)
                        p->m_cibleInt = -1;
        }
        else if (script->m_instructions[noInstruction].nom=="addCash")
        {
            hero->m_argent += (int)script->getValeur(noInstruction, 0);
        }
        else if (script->m_instructions[noInstruction].nom=="heal")
        {
            hero->m_personnage.InfligerDegats(-script->getValeur(noInstruction, 0),0,0);
        }
        else if (script->m_instructions[noInstruction].nom=="reset_miracle")
        {
            Caracteristique temp = hero->m_personnage.getCaracteristique();
            for(unsigned i = 1 ; i < hero->m_lvl_miracles.size() ; ++i)
                temp.miracles_restant += hero->m_lvl_miracles[i],hero->m_lvl_miracles[i] = 0;
            hero->m_personnage.setCaracteristique(temp);
        }
        else if (script->m_instructions[noInstruction].nom=="entity_variable" && monstre == -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[(unsigned)script->getValeur(noInstruction, 0)].size() ; ++i)
                    m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].m_scriptAI.setVariable((int)script->getValeur(noInstruction, 1), script->getValeur(noInstruction, 2));
        }
        else if (script->m_instructions[noInstruction].nom=="entity_name" && monstre == -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[(unsigned)script->getValeur(noInstruction, 0)].size() ; ++i)
                {
                    Caracteristique temp = m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].getCaracteristique();
                    temp.nom = configuration->getText((int)script->getValeur(noInstruction, 1), (int)script->getValeur(noInstruction, 2));
                    m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].setCaracteristique(temp);
                }
        }
        else if (script->m_instructions[noInstruction].nom=="goto" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            m_monstre[monstre].setArrivee(
                        coordonnee((int)script->getValeur(noInstruction, 0), (int)script->getValeur(noInstruction, 1)));
        }
        else if (script->m_instructions[noInstruction].nom=="stop" && monstre != -1 && (configuration->hote || !configuration->hote))
        {
            m_monstre[monstre].setArrivee(
                        coordonnee(m_monstre[monstre].getProchaineCase().x, m_monstre[monstre].getProchaineCase().y));
        }
        else if (script->m_instructions[noInstruction].nom=="entity_goto" && monstre == -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[(unsigned)script->getValeur(noInstruction, 0)].size() ; ++i)
                {
                    m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].setArrivee(
                        coordonnee((int)script->getValeur(noInstruction, 1), (int)script->getValeur(noInstruction, 2)));
                }
        }
        else if (script->m_instructions[noInstruction].nom=="entity_setState" && monstre == -1 && (configuration->hote /*|| !configuration->hote*/))
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[(unsigned)script->getValeur(noInstruction, 0)].size() ; ++i)
                {
                    if (m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].getEtat()!=script->getValeur(noInstruction, 1))
                        m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].setJustEtat((int)script->getValeur(noInstruction, 1)), m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].m_etatForce = true;
                    if(script->m_instructions[noInstruction].m_valeurs.size() >= 3)
                        m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].setPose((int)script->getValeur(noInstruction, 2));
                    if(script->m_instructions[noInstruction].m_valeurs.size() >= 4)
                        m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]].setAngle((int)script->getValeur(noInstruction, 3));
                }
        }
        else if (script->m_instructions[noInstruction].nom=="entity_setFriend" && monstre == -1 && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) < m_listID.size())
                for(unsigned i = 0 ; i < m_listID[(unsigned)script->getValeur(noInstruction, 0)].size() ; ++i)
                {
                    if(script->getValeur(noInstruction, 1) == 1)
                    {
                         bool add = true;
                        for(unsigned j = 0 ; j < hero->m_amis.size() ; ++j)
                            if(hero->m_amis[j] == &m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]])
                                add = false;
                        if(add)
                            hero->m_amis.push_back(&m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]]);
                    }
                    else
                    for(unsigned j = 0 ; j < hero->m_amis.size() ; ++j)
                            if(hero->m_amis[j] == &m_monstre[m_listID[(unsigned)script->getValeur(noInstruction, 0)][i]])
                                hero->m_amis.erase(hero->m_amis.begin() + j--);

                }
        }
        else if (script->m_instructions[noInstruction].nom=="setTile" && monstre == -1)
        {
            if(script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < 2)
                if(script->getValeur(noInstruction, 1) >= 0 && script->getValeur(noInstruction, 1) < (int)m_decor[0][0].size())
                    if(script->getValeur(noInstruction, 2) >= 0 && script->getValeur(noInstruction, 2) < (int)m_decor[0].size())
                    {
                        int x = (unsigned)script->getValeur(noInstruction, 1);
                        int y = (unsigned)script->getValeur(noInstruction, 2);
                        int z = (unsigned)script->getValeur(noInstruction, 0);
                        m_decor[z][y][x].setTile((int)script->getValeur(noInstruction, 3));
                        m_decor[z][y][x].m_entite_graphique.m_noAnimation = (int)script->getValeur(noInstruction, 3);
                    }
        }
        else if (script->m_instructions[noInstruction].nom=="setClimate" && (configuration->hote || !configuration->hote))
        {
            if(script->getValeur(noInstruction, 0) >= 0 && script->getValeur(noInstruction, 0) < m_climates.size())
            {
                if((bool)script->getValeur(noInstruction, 1))
                {
                    if(m_climates[(unsigned)script->getValeur(noInstruction, 0)].m_actif)
                    {
                        if(m_climates[(unsigned)script->getValeur(noInstruction, 0)].GetState() == 1.0)
                            m_climates[(unsigned)script->getValeur(noInstruction, 0)].Continue();
                    }
                    else
                        m_climates[(unsigned)script->getValeur(noInstruction, 0)].m_actif = true;
                }
                else
                    m_climates[(unsigned)script->getValeur(noInstruction, 0)].Stop();

            }
        }
        else if (script->m_instructions[noInstruction].nom=="change_map")
        {
            string nomMap = script->m_instructions[noInstruction].valeurString;

            coordonnee coordonneePerso;
            coordonneePerso.x = (int)script->getValeur(noInstruction, 0);
            coordonneePerso.y = (int)script->getValeur(noInstruction, 1);

            sf::Clock Clock;
            Clock.Reset();

            jeu->m_chargement->setC_Chargement(nomMap,coordonneePerso);
            jeu->m_contexte = jeu->m_chargement;
            jeu->SendChangeMap(nomMap, coordonneePerso);
        }
        else if (script->m_instructions[noInstruction].nom=="playMusic")
        {
            if(script->m_instructions[noInstruction].valeurString.empty())
            {
                m_musiqueEnCours = (int)script->getValeur(noInstruction, 0);
                if (m_musiqueEnCours >=0 && m_musiqueEnCours < (int)m_musiques.size())
                    moteurSons->PlayNewMusic(m_musiques[m_musiqueEnCours]);
            }
            else
                moteurSons->PlayNewMusic(script->m_instructions[noInstruction].valeurString);
        }
        else if (script->m_instructions[noInstruction].nom=="setAmbiance")
        {
            if((int)script->getValeur(noInstruction, 0) < (int)m_ambiances.size())
                m_playAmbiances[(int)script->getValeur(noInstruction, 0)] = script->getValeur(noInstruction, 1);
               // moteurSons->JouerSon(m_ambiances[(int)script->getValeur(noInstruction, 0)],coordonnee (), 1, 0, 100*script->getValeur(noInstruction, 1));
           /* m_musiqueEnCours = (int)script->getValeur(noInstruction, 0);
            if (m_musiqueEnCours >=0 && m_musiqueEnCours < (int)m_musiques.size())
                moteurSons->PlayNewMusic(m_musiques[m_musiqueEnCours]);*/
        }
    }
}

void Map::GererConditions(Jeu *jeu,Script *script,int noInstruction,int monstre,Hero *hero,float temps,Menu *menu, bool seDeplacer)
{
    if (noInstruction>=0&&noInstruction<(int)script->m_instructions.size())
    {
        int b=0;
        bool ok=true;
        for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-2;b++)
        {
            if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
            {
                int no = (int) script->m_instructions[noInstruction].m_valeurs[b];

                if (script->m_instructions[no].nom=="hero_dead")
                {
                    if (hero->m_personnage.EnVie())
                        ok=false;
                }
                if (script->m_instructions[no].nom=="alive" && monstre != -1)
                {
                    if (!m_monstre[monstre].EnVie())
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="see" && monstre != -1)
                {
                    if (!m_monstre[monstre].getVu())
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="visible" && monstre != -1)
                {
                    if (!TileVisible(hero->m_personnage.getCoordonnee().x,hero->m_personnage.getCoordonnee().y,m_monstre[monstre].getCoordonnee()))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="shooter" && monstre != -1)
                {
                    if (!m_monstre[monstre].m_shooter)
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="touch" && monstre != -1)
                {
                    if (!m_monstre[monstre].m_touche)
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="stop" && monstre != -1)
                {
                    if (!seDeplacer)
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="getState" && monstre != -1)
                {
                      if (m_monstre[monstre].getEtat() != script->getValeur(no, 0)
                            || (script->m_instructions[no].m_valeurs.size() >= 2
                                 && m_monstre[monstre].m_entite_graphique.m_noAnimation != script->getValeur(no, 1)))

                  //  if (m_monstre[monstre].getEtat() != (int)script->getValeur(no, 0))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="getState" && monstre != -1)
                {

                    if (m_monstre[monstre].getModele()>=0&&m_monstre[monstre].getModele()<(int)m_ModeleMonstre.size())
                        if(m_monstre[monstre].m_entite_graphique.m_tileset->IsPlayingSound((int)script->getValeur(noInstruction, 0)))
                            ok=false;
                }
                else if (script->m_instructions[no].nom=="position" && monstre != -1)
                {
                    if (m_monstre[monstre].getCoordonnee().x != script->getValeur(no, 0)
                    ||  m_monstre[monstre].getCoordonnee().y != script->getValeur(no, 1))
                            ok = false;
                }
                else if (script->m_instructions[no].nom=="numberInvocation" && monstre != -1)
                {
                    if (m_monstre[monstre].m_nombreInvocation != (int)script->getValeur(no, 0))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="miracle" && monstre != -1)
                {
                    bool oldok = ok;

                    ok = false;
                    for(unsigned i = 0 ; i < m_monstre[monstre].m_miracleEnCours.size() ; ++i)
                        if (m_monstre[monstre].m_miracleEnCours[i].m_modele == (int)script->getValeur(no, 0))
                            ok=oldok;
                }
                else if (script->m_instructions[no].nom=="distance" && monstre != -1)
                {
                    bool oldok = ok;

                    ok = false;
                    if (((m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)
                        *(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)
                    +    (m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)
                        *(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)) <= script->getValeur(no, 0)*script->getValeur(no, 0))
                        ok=oldok;

                    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                    if (((m_monstre[monstre].getCoordonnee().x-p->m_personnage.getCoordonnee().x)
                        *(m_monstre[monstre].getCoordonnee().x-p->m_personnage.getCoordonnee().x)
                    +    (m_monstre[monstre].getCoordonnee().y-p->m_personnage.getCoordonnee().y)
                        *(m_monstre[monstre].getCoordonnee().y-p->m_personnage.getCoordonnee().y)) <= script->getValeur(no, 0)*script->getValeur(no, 0))
                        ok=oldok;
                }
                else if (script->m_instructions[no].nom=="exist_item")
                {
                    int nbr = 0;
                    for(unsigned i = 0 ; (int)i < hero->getNombreObjet() ; ++i)
                        if(hero->getNomObjet(i) == script->m_instructions[no].valeurString)
                            nbr++;


                    if(nbr < script->getValeur(no, 0))
                         ok = false;
                    else if(script->getValeur(no, 1))
                        for(unsigned i = 0 ; (int)i < hero->getNombreObjet() ; ++i)
                            if(hero->getNomObjet(i) == script->m_instructions[no].valeurString)
                                hero->delObjet(i), i--;

                }
                else if (script->m_instructions[no].nom=="crafted")
                {
                    bool oldok = ok;
                    ok = false;

                    if(jeu->hero.m_no_result_craft >= 0)
                        if(jeu->hero.m_craft_time <= 0)
                            if(jeu->hero.getNomObjet(jeu->hero.m_no_result_craft) == script->m_instructions[no].valeurString)
                                ok = oldok;
                }
                else if (script->m_instructions[no].nom=="exist_friend")
                {
                    int nbr = 0;

                    for(unsigned j = 0 ; j < hero->m_amis.size() ; ++j)
                        if(m_ModeleMonstre[hero->m_amis[j]->getModele()].m_chemin == script->m_instructions[no].valeurString)
                            nbr++;

                    if(nbr < script->getValeur(no, 0) && script->getValeur(no, 0) >= 0)
                         ok = false;
                    else if(script->getValeur(no, 0) == -1 && nbr > 0)
                         ok = false;
                }
                else if (script->m_instructions[no].nom=="rand")
                {
                    if (rand() % 100 < script->getValeur(no, 0))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="variable")
                {
                    if (script->getVariable((int)script->getValeur(no, 0))!=script->getValeur(no, 1))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="variable_bigger")
                {
                    if (script->getVariable((int)script->getValeur(no, 0))<script->getValeur(no, 1))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="variable_smaller")
                {
                    if (script->getVariable((int)script->getValeur(no, 0))>script->getValeur(no, 1))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="talk" && monstre != -1)
                {
                    if (!(hero->m_personnage.m_cible == &m_monstre[monstre]
                    &&fabs(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)<=1
                    &&fabs(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)<=1))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="interact" && monstre != -1)
                {
                    bool newok = false;

                    if (hero->m_personnage.m_cible == &m_monstre[monstre]
                    &&fabs(m_monstre[monstre].getCoordonnee().x-hero->m_personnage.getCoordonnee().x)<=1
                    &&fabs(m_monstre[monstre].getCoordonnee().y-hero->m_personnage.getCoordonnee().y)<=1)
                        newok = true, hero->m_cibleInt = monstre, jeu->SendInteract();

                    for (std::list<Hero>::iterator p = jeu->m_personnageClients.begin(); p != jeu->m_personnageClients.end(); ++p)
                        if (p->m_cibleInt == monstre
                        &&fabs(m_monstre[monstre].getCoordonnee().x-p->m_personnage.getCoordonnee().x)<=1
                        &&fabs(m_monstre[monstre].getCoordonnee().y-p->m_personnage.getCoordonnee().y)<=1)
                            newok = true;

                    if(!newok)
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="clicOver" && monstre != -1)
                {
                    if (!(hero->m_personnage.m_cible == &m_monstre[monstre]))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="speak_choice" && monstre != -1)
                {
                    if (menu->getSpeakChoice() != script->getValeur(no, 0))
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="no_speak")
                {
                    if (!jeu->menu.m_dialogue.empty())
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="hour")
                {
                    if (configuration->heure < script->getValeur(no, 0)
                     || configuration->heure > script->getValeur(no, 1) )
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="climate")
                {
                    if(m_climates.size() > (unsigned)script->getValeur(no, 0))
                    {

                        if(!m_climates[(unsigned)script->getValeur(no, 0)].m_actif)
                            ok = false;

                        if(script->getValeur(no, 1) != 0)
                        if(m_climates[(unsigned)script->getValeur(no, 0)].m_cur_time > script->getValeur(no, 1))
                            ok = false;

                        if(script->getValeur(no, 2) != 0)
                        if(m_climates[(unsigned)script->getValeur(no, 0)].m_cur_time < script->getValeur(no, 2))
                            ok = false;

                    }
                    else
                        ok = false;
                }
                else if (script->m_instructions[no].nom=="player_class")
                {
                    if (jeu->hero.m_cheminClasse != script->m_instructions[no].valeurString)
                        ok=false;
                }
                else if (script->m_instructions[no].nom=="quest")
                {
                    if (script->getValeur(no, 1) == -1)
                    {
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->getValeur(no, 0))
                                ok = false;
                    }
                    else if(ok)
                    {
                        ok = false;
                        for (int i = 0;i < (int)hero->m_quetes.size(); ++i)
                            if (hero->m_quetes[i].m_id == script->getValeur(no, 0))
                                if (hero->m_quetes[i].m_statut == script->getValeur(no, 1))
                                    ok = true;
                    }
                }
                else if (script->m_instructions[no].nom=="entity_variable" && monstre == -1)
                {
                    if(script->getValeur(no, 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[(int)script->getValeur(no, 0)].size() ; ++i)
                            if (m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].m_scriptAI.getVariable((int)script->getValeur(no, 1))!=script->getValeur(no, 2))
                                ok = false;
                    }
                    else
                        ok = false;
                }
                else if (script->m_instructions[no].nom=="entity_dead" && monstre == -1)
                {
                    if(script->getValeur(no, 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[(int)script->getValeur(no, 0)].size() ; ++i)
                            if (m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].EnVie())
                                if(m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].m_friendly ==
                                   m_ModeleMonstre[m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].getModele()].m_friendly
                                || (int)script->getValeur(no, 1) == 1)
                                    ok = false;
                    }
                  //  else
                    //    ok = false;
                }
                else if (script->m_instructions[no].nom=="entity_position" && monstre == -1)
                {
                    if(script->getValeur(no, 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[(int)script->getValeur(no, 0)].size() ; ++i)
                            if (m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].getCoordonnee().x != script->getValeur(no, 1)
                            ||  m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].getCoordonnee().y != script->getValeur(no, 2))
                                    ok = false;
                    }
                    else
                        ok = false;
                }
                else if (script->m_instructions[no].nom=="entity_state" && monstre == -1)
                {
                    if(script->getValeur(no, 0) < m_listID.size())
                    {
                        for(unsigned i = 0 ; i < m_listID[(int)script->getValeur(no, 0)].size() ; ++i)
                            if (m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].getEtat() != script->getValeur(no, 1)
                            || (script->m_instructions[no].m_valeurs.size() >= 3
                                 && m_monstre[m_listID[(int)script->getValeur(no, 0)][i]].m_entite_graphique.m_noAnimation != script->getValeur(no, 2))
                                )
                                    ok = false;
                    }
                    else
                        ok = false;
                }
            }
        }
        for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-1&&script->m_instructions[noInstruction].m_valeurs[b]!=-3;b++)
            if (ok)
                if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,(int)script->m_instructions[noInstruction].m_valeurs[b],monstre,hero,temps,menu,seDeplacer);
        if (!ok)
            for (;b<(int)script->m_instructions[noInstruction].m_valeurs.size()&&script->m_instructions[noInstruction].m_valeurs[b]!=-1;b++)
                if (script->m_instructions[noInstruction].m_valeurs[b]>=0&&script->m_instructions[noInstruction].m_valeurs[b]<(int)script->m_instructions.size())
                    GererInstructions(jeu,script,(int)script->m_instructions[noInstruction].m_valeurs[b],monstre,hero,temps,menu,seDeplacer);
    }
}
