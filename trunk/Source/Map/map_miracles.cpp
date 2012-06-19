#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include "map.h"

#include <iostream>
#include <fstream>
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"

bool Map::Miracle_Aura           (Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_IDObjet == -1)
    {
        info.m_IDObjet = 1;

        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[0];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }
    else
        info.m_position = personnage->getCoordonneePixel();

    if(detruire)
    {
        for (int r = o; r < (int) miracleEnCours.m_infos.size() ; ++r)
            if (miracleEnCours.m_infos[r]->m_effetEnCours == effet.m_informations[0])
            {
                if(modele.m_effets[miracleEnCours.m_infos[r]->m_effetEnCours].m_type == EFFET)
                    miracleEnCours.m_infos[r]->m_cible->m_effets[miracleEnCours.m_infos[r]->m_IDObjet].m_effet.m_actif = false;

                miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin() + r);
                r--;
            }

        for (int p=0;p<(int)effet.m_lien.size();p++)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours=effet.m_lien[p];
            miracleEnCours.m_infos.back()->m_position=miracleEnCours.m_infos[o]->m_position;
            miracleEnCours.m_infos.back()->m_cible=miracleEnCours.m_infos[o]->m_cible;
        }

        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

        return 0;
    }

    return 1;
}

bool Map::Miracle_Declencheur    (Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if(effet.m_informations[0] == D_PERMANENT)
    {
        info.m_informations[4] += temps * 100;
        if (info.m_informations[4] > effet.m_informations[4])
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = info.m_cible;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;

                if(effet.m_informations[5])
                    detruire = true;
            }
            info.m_informations[4] = 0;
        }
    }
    if(effet.m_informations[0] == D_FRAPPE)
    {
        if(personnage->m_vientDeFrapper != NULL)
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];

                miracleEnCours.m_infos.back()->m_position.x      = personnage->getCoordonneePixel().x/*+cos(-(personnage->getAngle()+22.5)*M_PI/180)*96*/;
                miracleEnCours.m_infos.back()->m_position.y      = personnage->getCoordonneePixel().y/*+sin(-(personnage->getAngle()+22.5)*M_PI/180)*96*/;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = personnage->m_vientDeFrapper;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;

                if(effet.m_informations[5])
                    detruire = true;
            }
        }
    }
    if(effet.m_informations[0] == D_ATTAQUE)
    {
        if(personnage->m_vientDAttaquer.x != -1)
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];

                miracleEnCours.m_infos.back()->m_position.x      = personnage->getCoordonneePixel().x;
                miracleEnCours.m_infos.back()->m_position.y      = personnage->getCoordonneePixel().y;

                miracleEnCours.m_infos.back()->m_cible = info.m_cible;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_coordonneeCible = personnage->m_vientDAttaquer;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;

                if(effet.m_informations[5])
                    detruire = true;
            }
        }
    }
    if(effet.m_informations[0] == D_TOUCHE)
    {
        if(personnage->m_vientDetreTouche != NULL)
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_infos.back()->m_cible = personnage->m_vientDetreTouche;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;

                if(effet.m_informations[5])
                    detruire = true;
            }
        }
    }

    if(effet.m_informations[0] == D_CIBLE_MORT)
    {
        if(miracleEnCours.m_infos.back()->m_cible)
        if(!miracleEnCours.m_infos.back()->m_cible->EnVie())
        {
            if (effet.m_informations[3] >= 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[3];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;

                if(effet.m_informations[2] != 0)
                    miracleEnCours.m_infos.back()->m_cible = personnage;


                for (int p=0;p<(int)effet.m_lien.size();p++)
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                    miracleEnCours.m_infos.back()->m_position        = info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
                }

                miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin() + o);

                if(effet.m_informations[5])
                    detruire = true;

                return 0;
            }
        }
    }

    if(effet.m_informations[1] != -100 || detruire)
    {
        info.m_informations[3] += temps * 100;
        if (info.m_informations[3] > effet.m_informations[1] || detruire)
        {
            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin() + o);

            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_Effet  (Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if(info.m_IDObjet == -1)
    {
        if (effet.m_informations[0])
        {
            info.m_IDObjet = personnage->AjouterEffet(
                moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]),
                effet.m_informations[1], // TYPE
                effet.m_informations[2], // TEMPS
                effet.m_informations[3], // INFOS
                effet.m_informations[4],
                effet.m_informations[5],
                effet.m_informations[6], // COLOR
                effet.m_informations[7], // COLOR
                effet.m_informations[8], // COLOR
                effet.m_informations[9]); // COLOR
            info.m_cible = personnage;
        }
        else if(info.m_cible != NULL)
        {
            info.m_IDObjet = info.m_cible->AjouterEffet(
                moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]),
                effet.m_informations[1], // TYPE
                effet.m_informations[2], // TEMPS
                effet.m_informations[3], // INFOS
                effet.m_informations[4],
                effet.m_informations[5],
                effet.m_informations[6], // COLOR
                effet.m_informations[7], // COLOR
                effet.m_informations[8], // COLOR
                effet.m_informations[9]); // COLOR

            info.m_position = info.m_cible->getCoordonneePixel();
        }
        else
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }

    }
    else
    {
        Personnage *temp;
        if (effet.m_informations[0])
            temp = personnage;
        else
            temp = info.m_cible;

        info.m_position = temp->getCoordonneePixel();

        bool fin = false;

        if((int)temp->m_effets.size() > info.m_IDObjet)
        {
            if(detruire)
                temp->m_effets[info.m_IDObjet].m_effet.m_actif = false;

            if(!temp->m_effets[info.m_IDObjet].m_effet.m_actif)
                fin = true;
        }
        else
            fin = true;

        if(fin)
        {
            moteurGraphique->LightManager->Delete_Light(temp->m_effets[info.m_IDObjet].m_effet.m_light);
            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_Charme (Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if(info.m_IDObjet == -1)
    {
        if (info.m_cible->m_friendly != personnage->m_friendly
            &&info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0
            &&info.m_cible->getCaracteristique().rang <= 1)
        {
            info.m_cible->m_scriptAI = effet.m_chaine;
            info.m_cible->m_scriptAI.Add(m_ModeleMonstre[info.m_cible->getModele()].m_scriptAI);

            info.m_cible->m_friendly     = true;

            info.m_cible->m_cible        = NULL;
            info.m_cible->frappeEnCours  = false;
            info.m_cible->setEtat(0);

            info.m_IDObjet    = 1;

            for (std::list<Hero*>::iterator p = jeu->m_listHeroes.begin(); p != jeu->m_listHeroes.end(); ++p)
                if(personnage == &(*p)->m_personnage)
                    jeu->hero.m_amis.push_back(info.m_cible);

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours=effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position=info.m_position;
                miracleEnCours.m_infos.back()->m_cible=info.m_cible;
            }

            return 0;
        }
        else
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }
    else
    {
        info.m_cible->m_entite_graphique.m_color = sf::Color(255,32,128);

        if (!info.m_cible->EnVie())
        {
            for (std::list<Hero*>::iterator h = jeu->m_listHeroes.begin(); h != jeu->m_listHeroes.end(); ++h)
                if(personnage == &(*h)->m_personnage)
                    for(unsigned p = 0 ; p < jeu->hero.m_amis.size() ;  ++p)
                        if(jeu->hero.m_amis[p] == info.m_cible)
                            jeu->hero.m_amis.erase(jeu->hero.m_amis.begin() + p);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }

        if (abs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10
         || abs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
        {
            coordonnee temp;
            bool ok = false;
            int id = -1;
            for(unsigned i = 0 ; i < m_monstre.size() ; ++i)
                if(m_monstre[i] == info.m_cible)
                    id = i, m_decor[1][m_monstre[i]->getCoordonnee().y][m_monstre[i]->getCoordonnee().x].delMonstre(i);

            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x - 1;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x + 1;
            temp.y = personnage->getCoordonnee().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;
            temp.x = personnage->getCoordonnee().x + 1;
            temp.y = personnage->getCoordonnee().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;


            if(ok)
            {
                m_decor[1][info.m_cible->getCoordonnee().y][info.m_cible->getCoordonnee().x].setMonstre(id);

                info.m_cible->setDepart();
                info.m_cible->frappeEnCours = false;
            }
        }

        if (!info.m_cible->m_friendly || detruire)
        {
            info.m_cible->DetruireEffets();
            info.m_cible->m_scriptAI = m_ModeleMonstre[info.m_cible->getModele()].m_scriptAI;
            info.m_cible->setDepart();

            info.m_cible->m_entite_graphique.m_color = sf::Color(255,255,255);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_CorpsACorps (Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_cible || miracleEnCours.m_forced_maj)
    {
        bool ok = false;

        if(info.m_cible)
        {
            if( (abs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > effet.m_informations[0]
                 && abs(info.m_cible->getProchaineCase().x - personnage->getCoordonnee().x) > effet.m_informations[0])
             || (abs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > effet.m_informations[0]
                 && abs(info.m_cible->getProchaineCase().y - personnage->getCoordonnee().y) > effet.m_informations[0]))
                personnage->setArrivee(info.m_cible->getProchaineCase());
            else ok = true;
        }
        if(miracleEnCours.m_forced_maj)
            ok = true;

        if(ok)
        {
            personnage->setArrivee(personnage->getCoordonnee());

            //info.m_position.x = (float)personnage->getProchaineCase().x * COTE_TILE + 1;
            //info.m_position.y = (float)personnage->getProchaineCase().y * COTE_TILE + 1;

            info.m_position =  personnage->getCoordonneePixel();

            if( personnage->getArrivee().x == personnage->getCoordonnee().x
             && personnage->getArrivee().y == personnage->getCoordonnee().y)
            {
                coordonnee pos;
                if(info.m_cible)
                    pos = info.m_cible->getCoordonneePixel();
                else
                {
                    pos.x = (int)(miracleEnCours.m_coordonneeCible.x * COTE_TILE);
                    pos.y = (int)(miracleEnCours.m_coordonneeCible.y * COTE_TILE);
                }

                personnage->Frappe(personnage->getCoordonneePixel(),pos);

                if(effet.m_informations[1])
                {
                    personnage->m_miracleFrappeEnCours = true;
                    personnage->m_cible = info.m_cible;
                }

                for (int p=0;p<(int)effet.m_lien.size();p++)
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    =   effet.m_lien[p];
                    miracleEnCours.m_infos.back()->m_position        =   info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           =   info.m_cible;
                }

                miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

                return 0;
            }
            personnage->setArrivee(personnage->getProchaineCase());
        }
    }
    else
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 1;
}

bool Map::Miracle_Repetition(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    info.m_informations[3] += temps * 100;
    if (info.m_informations[3] > effet.m_informations[2])
    {
        if (effet.m_informations[0] >= 0)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[0];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        info.m_informations[1]--;
        info.m_informations[3] = 0;
    }

    if (-info.m_informations[1] >= effet.m_informations[1])
    {
        for (int p=0;p<(int)effet.m_lien.size();p++)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

        return 0;
    }

    return 1;
}

bool Map::Miracle_Projectile(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_IDObjet >= 0 && info.m_IDObjet < (int)m_projectile.size())
    {
        if (effet.m_informations[5] > 0 || effet.m_informations[8])
        {
            info.m_informations[6] += temps * 100;
            if (info.m_informations[6] > effet.m_informations[6])
            {
                if (effet.m_informations[5] >= 0)
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[5];
                    miracleEnCours.m_infos.back()->m_position        = m_projectile[info.m_IDObjet].m_position;
                    miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
                }
                info.m_informations[6] = 0;

                if(effet.m_informations[8])
                {
                    m_projectile[info.m_IDObjet].m_effet.m_actif = false;
                    m_projectile[info.m_IDObjet].m_actif = false;
                }
            }
        }

        if (!m_projectile[info.m_IDObjet].m_effet.m_actif)
            m_projectile[info.m_IDObjet].m_actif = false;

        if (m_projectile[info.m_IDObjet].m_actif)
            m_projectile[info.m_IDObjet].m_effet.Animer(temps);

        if (!m_projectile[info.m_IDObjet].m_effet.m_actif)
            m_projectile[info.m_IDObjet].m_actif = false;

        if (!m_projectile[info.m_IDObjet].m_actif && !m_projectile[info.m_IDObjet].m_supprime)
        {
            m_projectile[info.m_IDObjet].m_supprime = true;
            info.m_cible = m_projectile[info.m_IDObjet].m_entite_cible;

            if(modele.m_miracleArme)
                personnage->m_vientDeToucher = info.m_cible;

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = m_projectile[info.m_IDObjet].m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        coordonnee pos( (int)((float)info.m_position.x/COTE_TILE),
                        (int)((float)info.m_position.y/COTE_TILE));

        coordonneeDecimal cible;

        if (info.m_cible != NULL)
        {
            float ratio = sqrt((info.m_cible->getCoordonneePixel().x - info.m_position.x) *
                               (info.m_cible->getCoordonneePixel().x - info.m_position.x) +
                               (info.m_cible->getCoordonneePixel().y - info.m_position.y) *
                               (info.m_cible->getCoordonneePixel().y - info.m_position.y)) * 0.1 / effet.m_informations[0] * 10 / 500;

            cible.x = info.m_cible->getCoordonneePixel().x + info.m_cible->getEstimationMouvement().x * ratio;
            cible.y = info.m_cible->getCoordonneePixel().y + info.m_cible->getEstimationMouvement().y * ratio;
        }
        else
        {
            cible.x = miracleEnCours.m_coordonneeCible.x * COTE_TILE;
            cible.y = miracleEnCours.m_coordonneeCible.y * COTE_TILE;
        }

        Tileset *tileset = NULL;
        if(effet.m_sequence >= 0 && effet.m_sequence < (int)modele.m_tileset.size())
            tileset = moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]);


        info.m_IDObjet = AjouterProjectile( info.m_position,
                                            cible,personnage->getCoordonnee(),10,effet.m_informations[0],
                                            (float)effet.m_informations[1]*M_PI/180,!personnage->m_friendly,
                                            tileset);

        if (effet.m_informations[2])
        {
            m_projectile.back().m_cible.x = (int)(cible.x / COTE_TILE);
            m_projectile.back().m_cible.y = (int)(cible.y / COTE_TILE);
        }
        else
            m_projectile.back().m_cible = coordonnee (-1, -1);

        if (effet.m_informations[3])
            m_projectile.back().m_rotation = 0;

        m_projectile.back().m_transperce = effet.m_informations[4];

        if (effet.m_informations[7] > 0)
            m_projectile.back().m_effet.m_compteur = effet.m_informations[7];


    }

    return 1;
}

bool Map::Miracle_EffetGraphique(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_IDObjet >= 0)
    {
        bool suppr = false;

        if(info.m_IDObjet < (int)m_effets.size())
        {
            //m_effets[info.m_IDObjet].Animer(temps);

            if (!m_effets[info.m_IDObjet].m_actif)
                suppr = true;
        }
        else
            suppr = true;

        if(suppr)
        {
            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        m_effets.push_back(EffetGraphique ());

        m_effets.back().m_position.x = (int)info.m_position.x;
        m_effets.back().m_position.y = (int)info.m_position.y;

        m_effets.back().m_compteur   = effet.m_informations[0];

        m_effets.back().m_actif      = true;

        m_effets.back().m_tileset    = moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]);
        m_effets.back().m_couche     = 10;
        m_effets.back().Animer(0);
        m_effets.back().Initialiser(coordonnee((int)(((float)info.m_position.x - (float)info.m_position.y) * 64 / COTE_TILE),
                                               (int)(((float)info.m_position.x + (float)info.m_position.y) * 64 / COTE_TILE)));

        info.m_IDObjet               = m_effets.size()-1;

        m_effets.back().m_position_case.y = (int)((m_effets.back().m_position.y + COTE_TILE * 0.25) / COTE_TILE);
        m_effets.back().m_position_case.x = (int)((m_effets.back().m_position.x + COTE_TILE * 0.25) / COTE_TILE);

        if (m_effets.back().m_position_case.y >= 0
         && m_effets.back().m_position_case.y < m_dimensions.y
         && m_effets.back().m_position_case.x >= 0
         && m_effets.back().m_position_case.x < m_dimensions.x)
            m_decor[1][m_effets.back().m_position_case.y]
                      [m_effets.back().m_position_case.x].setEffetGraphique(m_effets.size()-1);
    }
    return 1;
}

bool Map::Miracle_Invocation(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_IDObjet==-1)
    {
        bool invoquer=true;
        int x=0,y=0;

        coordonnee cible;

        if(effet.m_informations[0] == 1)
        {
            cible.x = (int)(info.m_position.x/COTE_TILE);
            cible.y = (int)(info.m_position.y/COTE_TILE);
        }
        else
        {
            if (info.m_cible != NULL)
                cible = info.m_cible->getProchaineCase();
            else
                cible = miracleEnCours.m_coordonneeCible;
        }

        coordonnee positionCase = cible;
        positionCase.x = cible.x-1;
        positionCase.y = cible.y-1;

        if (positionCase.x<0)
            positionCase.x=0;
        if (positionCase.y<0)
            positionCase.y=0;
        if (positionCase.x>=m_dimensions.x)
            positionCase.x=m_dimensions.x - 1;
        if (positionCase.y>=m_dimensions.y)
            positionCase.y=m_dimensions.y - 1;

        while ((getCollision(positionCase.x,positionCase.y) || ( positionCase.x==jeu->hero.m_personnage.getCoordonnee().x && positionCase.y==jeu->hero.m_personnage.getCoordonnee().y )) && invoquer )
        {
            positionCase.x++;
            x++;
            if (x>=3)
                x=0,positionCase.x-=3,y++,positionCase.y++;
            if (y>=3)
                invoquer = false;
        }

        if(effet.m_informations[0] == 1)
        {
            positionCase.x = (int)(info.m_position.x/COTE_TILE);
            positionCase.y = (int)(info.m_position.y/COTE_TILE);
        }

        if (invoquer)
        {
            info.m_position.x = positionCase.x*COTE_TILE;
            info.m_position.y = positionCase.y*COTE_TILE;

            for (unsigned p=0;p < effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }



            int numero=-1;
            for (int j=0;j<(int)m_ModeleMonstre.size();j++)
                if (effet.m_chaine==m_ModeleMonstre[j].m_chemin)
                {
                    numero=j;
                    break;
                }

            if (numero==-1)
            {
                m_ModeleMonstre.push_back(Modele_Monstre ());
                m_ModeleMonstre.back().Charger(effet.m_chaine);
                console->Ajouter("Loading : "+effet.m_chaine+" ended",0);
                numero=m_ModeleMonstre.size()-1;
            }

            m_monstre.push_back(new Monstre ());

            if(effet.m_informations[1] == 1)
            {
                m_monstre.back()->setCoordonneePixel2(personnage->getCoordonneePixel());

                positionCase.x = (int)(m_monstre.back()->getCoordonneePixel().x/COTE_TILE + 0.5);
                positionCase.y = (int)(m_monstre.back()->getCoordonneePixel().y/COTE_TILE + 0.5);
            }

            m_monstre.back()->Charger(numero,&m_ModeleMonstre[numero]);
            m_monstre.back()->setCoordonnee(positionCase),m_monstre.back()->setDepart();

            if(effet.m_informations[1] == 1)
                m_monstre.back()->setCoordonneePixel2(personnage->getCoordonneePixel());

            if(effet.m_informations[1] == 1)
            m_monstre.back()->setForcedAngle(personnage->getAngle());

            coordonnee pos;
            pos.x=(int)(((m_monstre.back()->getCoordonneePixel().x-m_monstre.back()->getCoordonneePixel().y)*64/COTE_TILE*64));
            pos.y=(int)(((m_monstre.back()->getCoordonneePixel().x+m_monstre.back()->getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            m_monstre.back()->Animer(&m_ModeleMonstre[numero],0);
            m_monstre.back()->m_entite_graphique.Initialiser(pos);

           // m_monstre.back()->m_friendly = personnage->m_friendly;


            for (std::list<Hero*>::iterator p = jeu->m_listHeroes.begin(); p != jeu->m_listHeroes.end(); ++p)
                if(personnage == &(*p)->m_personnage)
                    jeu->hero.m_amis.push_back(m_monstre.back());

            info.m_cible = m_monstre.back();


            m_decor[1][positionCase.y][positionCase.x].setMonstre(m_monstre.size()-1);

            info.m_IDObjet=m_monstre.size()-1;

            if(modele.m_golem)
            {
                Caracteristique temp = m_monstre.back()->getCaracteristique();
                temp.maxVie = miracleEnCours.m_source->m_gol_caract.maxVie;
                temp.vie    = miracleEnCours.m_source->m_gol_caract.vie;

                temp.degatsMin[0]      = miracleEnCours.m_source->m_gol_caract.degatsMin[0];
                temp.degatsMax[0]      = miracleEnCours.m_source->m_gol_caract.degatsMax[0];

                temp.armure[0]      = miracleEnCours.m_source->m_gol_caract.armure[0];
                temp.armure[1]      = miracleEnCours.m_source->m_gol_caract.armure[1];
                temp.armure[2]      = miracleEnCours.m_source->m_gol_caract.armure[2];
                temp.armure[3]      = miracleEnCours.m_source->m_gol_caract.armure[3];

                temp.force          += miracleEnCours.m_source->m_gol_caract.force;
                temp.dexterite      += miracleEnCours.m_source->m_gol_caract.dexterite;
                temp.vitalite       += miracleEnCours.m_source->m_gol_caract.vitalite;
                temp.piete          += miracleEnCours.m_source->m_gol_caract.piete;
                temp.charisme       += miracleEnCours.m_source->m_gol_caract.charisme;

                m_monstre.back()->setCaracteristique(temp);

                bool mir = false;

                for(unsigned j = 0 ; j < miracleEnCours.m_source->m_miracles_benedictions.size() ; ++j)
                {
                    if(!mir)
                    {
                        m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.push_back(Miracle (
                                    miracleEnCours.m_source->m_miracles_benedictions[j].m_chemin,
                                    temp,0));

                        m_monstre.back()->m_miracleEnCours.push_back(EntiteMiracle ());
                        m_monstre.back()->m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                        m_monstre.back()->m_miracleEnCours.back().m_modele = m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.size() - 1;

                        m_monstre.back()->m_miracleEnCours.back().m_infos.back()->m_position.x = m_monstre.back()->getCoordonneePixel().x;
                        m_monstre.back()->m_miracleEnCours.back().m_infos.back()->m_position.y = m_monstre.back()->getCoordonneePixel().y;

                        mir = true;
                    }
                    else
                    {
                        m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.back().Concatenencer(
                                    miracleEnCours.m_source->m_miracles_benedictions[j].m_chemin,
                                    temp,0);
                    }
                }

                for(unsigned i = 0 ; i < miracleEnCours.m_source->m_benedictions.size() ; ++i)
                {
                    if(!mir)
                    {
                        m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.push_back(Miracle ());

                        m_monstre.back()->m_miracleEnCours.push_back(EntiteMiracle ());
                        m_monstre.back()->m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                        m_monstre.back()->m_miracleEnCours.back().m_modele = m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.size() - 1;

                        m_monstre.back()->m_miracleEnCours.back().m_infos.back()->m_position.x = m_monstre.back()->getCoordonneePixel().x;
                        m_monstre.back()->m_miracleEnCours.back().m_infos.back()->m_position.y = m_monstre.back()->getCoordonneePixel().y;

                    }

                    ChargerMiracleBenediction(miracleEnCours.m_source->m_benedictions[i],m_ModeleMonstre[m_monstre.back()->getModele()].m_miracles.back(),mir);
                }


            }
        }
    }
    else if (info.m_IDObjet>=0&&info.m_IDObjet<(int)m_monstre.size())
    {
        personnage->m_nombreInvocation++;
        if (!info.m_cible->EnVie())
        {
            if(effet.m_informations[1] > 0)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[1];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            for (std::list<Hero*>::iterator h = jeu->m_listHeroes.begin(); h != jeu->m_listHeroes.end(); ++h)
                if(personnage == &(*h)->m_personnage)
                    for(unsigned p = 0 ; p < jeu->hero.m_amis.size() ;  ++p)
                        if(jeu->hero.m_amis[p] == info.m_cible)
                            jeu->hero.m_amis.erase(jeu->hero.m_amis.begin() + p);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }

        if (abs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10
         || abs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
        {
            coordonnee temp;
            bool ok = false;
            int id = -1;
            for(unsigned i = 0 ; i < m_monstre.size() ; ++i)
                if(m_monstre[i] == info.m_cible)
                    id = i, m_decor[1][m_monstre[i]->getCoordonnee().y][m_monstre[i]->getCoordonnee().x].delMonstre(i);

            temp.x = personnage->getProchaineCase().x - 1;
            temp.y = personnage->getProchaineCase().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x - 1;
            temp.y = personnage->getProchaineCase().y;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x - 1;
            temp.y = personnage->getProchaineCase().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x;
            temp.y = personnage->getProchaineCase().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x;
            temp.y = personnage->getProchaineCase().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x + 1;
            temp.y = personnage->getProchaineCase().y - 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;

            temp.x = personnage->getProchaineCase().x + 1;
            temp.y = personnage->getProchaineCase().y + 1;
            if(!getCollision(temp.x, temp.y))
                info.m_cible->setCoordonnee(temp), ok = true;


            if(ok)
            {
                m_decor[1][info.m_cible->getCoordonnee().y][info.m_cible->getCoordonnee().x].setMonstre(id);

                info.m_cible->setDepart();
                info.m_cible->frappeEnCours = false;
            }
        }
    }
    else
    {
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
        return 0;
    }
    return 1;
}

bool Map::Miracle_Pose(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (effet.m_informations[0] != -1)
        personnage->setJustEtat(effet.m_informations[0]);

    if (effet.m_informations[1] != -1)
        personnage->setPose(effet.m_informations[1]);

    personnage->addAngle(effet.m_informations[2]*45);

    if(effet.m_informations[3])
        personnage->m_lancementMiracleEnCours = true;

      //  personnage->m_etatForce = true;

    if(effet.m_informations[4])
    {
        coordonnee pos;
        if(info.m_cible)
            pos = info.m_cible->getCoordonneePixel();
        else
        {
            pos.x = (int)(miracleEnCours.m_coordonneeCible.x * COTE_TILE);
            pos.y = (int)(miracleEnCours.m_coordonneeCible.y * COTE_TILE);
        }

        personnage->Frappe(personnage->getCoordonneePixel(),pos);
    }

    for (int p=0;p<(int)effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Charge(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    info.m_informations[3] += temps * 1000;
    if (info.m_informations[3] > effet.m_informations[2])
    {
        if (effet.m_informations[1] >= 0)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[1];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }

        info.m_informations[3] -= effet.m_informations[2];
    }

    info.m_position.x = (float)personnage->getCoordonneePixel().x;
    info.m_position.y = (float)personnage->getCoordonneePixel().y;

    if (info.m_IDObjet == 1)
    {

        if(miracleEnCours.m_coordonneeDepart.x != miracleEnCours.m_coordonneeCible.x && personnage->getPousse().x == 0)
        {
            miracleEnCours.m_coordonneeDepart.x = personnage->getCoordonnee().x;
            miracleEnCours.m_coordonneeCible.x  = personnage->getCoordonnee().x;
        }
        if(miracleEnCours.m_coordonneeDepart.y != miracleEnCours.m_coordonneeCible.y && personnage->getPousse().y == 0)
        {
            miracleEnCours.m_coordonneeDepart.y = personnage->getCoordonnee().y;
            miracleEnCours.m_coordonneeCible.y  = personnage->getCoordonnee().y;
        }

        if((((  miracleEnCours.m_coordonneeDepart.x  < miracleEnCours.m_coordonneeCible.x&&personnage->getCoordonneePixel().x+1>miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||( miracleEnCours.m_coordonneeDepart.x  > miracleEnCours.m_coordonneeCible.x&&(int)personnage->getCoordonneePixel().x<miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||  miracleEnCours.m_coordonneeDepart.x  == miracleEnCours.m_coordonneeCible.x)

           &&((  miracleEnCours.m_coordonneeDepart.y   > miracleEnCours.m_coordonneeCible.y&&(int)personnage->getCoordonneePixel().y<miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||(   miracleEnCours.m_coordonneeDepart.y   < miracleEnCours.m_coordonneeCible.y&&personnage->getCoordonneePixel().y+1>miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||    miracleEnCours.m_coordonneeDepart.y   == miracleEnCours.m_coordonneeCible.y))

            ||(personnage->getPousse().x == 0 && personnage->getPousse().y == 0))
        {

          /*  if(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x
            && personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y )
                personnage->setCoordonnee(coordonnee (miracleEnCours.m_coordonneeCible.x,
                                                      miracleEnCours.m_coordonneeCible.y));*/

            personnage->setPousse(coordonneeDecimal  (0,0));

            info.m_position.x = (float)personnage->getCoordonneePixel().x;
            info.m_position.y = (float)personnage->getCoordonneePixel().y;

            for (int p=0;p<(int)effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours=effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position = info.m_position;
                miracleEnCours.m_infos.back()->m_cible=info.m_cible;
            }

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }
    else
    {
        float m=atan2((double)(personnage->getCoordonnee().x-miracleEnCours.m_coordonneeCible.x),(double)(personnage->getCoordonnee().y-miracleEnCours.m_coordonneeCible.y));
        m+=M_PI/3;

        m=(int)(m*180/M_PI);
        if (m>=360)
            m=0;
        if (m<0)
            m=360+m;

        m = (int)(m/45)*45;
        personnage->setForcedAngle((int)m);
    }

    if ( !(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x
        && personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y) )
    {
        float m = atan2((float)miracleEnCours.m_coordonneeCible.x * COTE_TILE - (float)personnage->getCoordonneePixel().x,
                        (float)miracleEnCours.m_coordonneeCible.y * COTE_TILE - (float)personnage->getCoordonneePixel().y);

        coordonneeDecimal temp = personnage->getPousse();

        personnage->setPousse(coordonneeDecimal  (sin(m) * effet.m_informations[0],
                                                  cos(m) * effet.m_informations[0]));

        if(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x && temp.x == 0)
            personnage->setPousse(coordonneeDecimal(0, personnage->getPousse().y));
        if(personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y && temp.y == 0)
            personnage->setPousse(coordonneeDecimal(personnage->getPousse().x, 0));
    }

    info.m_IDObjet= 1;

    return 1;
}


bool Map::Miracle_Teleport(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    coordonnee enCours = miracleEnCours.m_coordonneeCible;

    float angle = 0;
    if(info.m_cible != NULL)
        angle = info.m_cible->getAngle() * M_PI / 180;

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle+M_PI)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle+M_PI)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle+3*M_PI_4)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle+3*M_PI_4)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle-3*M_PI_4)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle-3*M_PI_4)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle+M_PI_2)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle+M_PI_2)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle-M_PI_2)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle-M_PI_2)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle+M_PI_4)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle+M_PI_4)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle-M_PI_4)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle-M_PI_4)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours.x = miracleEnCours.m_coordonneeCible.x + (int)(cos(angle)*2);
    enCours.y = miracleEnCours.m_coordonneeCible.y + (int)(sin(angle)*2);
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    enCours = miracleEnCours.m_coordonneeCible;
    if(!getCollision(enCours.x,enCours.y))
        personnage->setCoordonnee(enCours);

    personnage->setDepart();
    personnage->setArrivee(personnage->getCoordonnee());

    for (int p=0;p<(int)effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }
    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
    return 0;
}

bool Map::Miracle_EffetEcran(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    moteurGraphique->m_effetsEcran.push_back(Effet_ecran ());
    moteurGraphique->m_effetsEcran.back().type      = effet.m_informations[0];
    moteurGraphique->m_effetsEcran.back().info1     = effet.m_informations[1];
    moteurGraphique->m_effetsEcran.back().info2     = effet.m_informations[2];
    moteurGraphique->m_effetsEcran.back().pos.x     = info.m_position.x;
    moteurGraphique->m_effetsEcran.back().pos.y     = info.m_position.y;


    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Degats(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    int deg = 0;

    if(effet.m_informations[1] != 0)
        deg = rand() % (int)(effet.m_informations[1] - effet.m_informations[0] + 1) + effet.m_informations[0];

    if (info.m_cible != NULL)
    {
        info.m_position = info.m_cible->getCoordonneePixel();
        info.m_position.x += COTE_TILE/2 + 1;//info.m_cible->m_entite_graphique.m_decalage.y*2 + info.m_cible->m_entite_graphique.m_decalage.x;
        info.m_position.y += COTE_TILE/2 + 1;//info.m_cible->m_entite_graphique.m_decalage.y*2 - info.m_cible->m_entite_graphique.m_decalage.x;
       // if (info.m_cible->getCoordonnee().y >=0 && info.m_cible->getCoordonnee().y < (int)m_decor[0].size())
         //   if (info.m_cible->getCoordonnee().x >=0 && info.m_cible->getCoordonnee().x < (int)m_decor[0][info.m_cible->getCoordonnee().y].size())
           // {
                if(deg != 0)
                {
                    if(!miracleEnCours.m_miracle_client)
                        InfligerDegats(info.m_cible, personnage, deg, effet.m_informations[2], jeu, effet.m_informations[3]);
                    if(personnage == &jeu->hero.m_personnage && effet.m_informations[3] == 0 && effet.m_informations[2] == 0)
                        jeu->hero.JouerSonAttaque(info.m_cible->m_materiau);
                }
           // }
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Souffle(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if (info.m_cible != NULL)
    {
        coordonneeDecimal vecteur;

        if      ((int)(info.m_position.x*0.1f)-(int)(info.m_cible->getCoordonneePixel().x*0.1f)<0)
            vecteur.x =  effet.m_informations[0];
        else if ((int)(info.m_position.x*0.1f)-(int)(info.m_cible->getCoordonneePixel().x*0.1f)>0)
            vecteur.x = -effet.m_informations[0];
        else
            vecteur.x = 0;

        if      ((int)(info.m_position.y*0.1f)-(int)(info.m_cible->getCoordonneePixel().y*0.1f)<0)
            vecteur.y =  effet.m_informations[0];
        else if ((int)(info.m_position.y*0.1f)-(int)(info.m_cible->getCoordonneePixel().y*0.1f)>0)
            vecteur.y = -effet.m_informations[0];
        else
            vecteur.y = 0;

        if(vecteur.x == 0 && vecteur.y == 0)
            vecteur.x = effet.m_informations[0],
            vecteur.y = effet.m_informations[0];

        info.m_cible->Pousser(vecteur);
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Zone(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    coordonnee buf( (int)((int)(info.m_position.x+COTE_TILE*0.25f)/COTE_TILE),
                    (int)((int)(info.m_position.y+COTE_TILE*0.25f)/COTE_TILE));

    for(int x = buf.x - effet.m_informations[3];
            x <= buf.x + effet.m_informations[3]; ++x)
    for(int y = buf.y - effet.m_informations[3];
            y <= buf.y + effet.m_informations[3]; ++y)
        if (y>=0&&x>=0&&y<m_dimensions.y&&x<m_dimensions.x)
            {

                for (unsigned z = 0 ; z < m_decor[1][y][x].getMonstre().size() ; ++z)
                    if (m_decor[1][y][x].getMonstre()[z]>=0&&m_decor[1][y][x].getMonstre()[z]<(int)m_monstre.size())
                        if (m_monstre[m_decor[1][y][x].getMonstre()[z]]->EnVie()
                        && ((m_monstre[m_decor[1][y][x].getMonstre()[z]]->m_friendly != personnage->m_friendly)
                         || (m_monstre[m_decor[1][y][x].getMonstre()[z]]->m_friendly == personnage->m_friendly && effet.m_informations[5]/*&& y==jeu->hero.m_personnage.getCoordonnee().y&&x==jeu->hero.m_personnage.getCoordonnee().x*/ /*&& ((!personnage->m_friendly) || effet.m_informations[5]))*/))
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]]->getCaracteristique().rang>=0
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]]->m_actif
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]]->getCaracteristique().niveau>=0)
                        {
                            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_informations[0];
                            miracleEnCours.m_infos.back()->m_position        = info.m_position;
                            miracleEnCours.m_infos.back()->m_cible           = m_monstre[m_decor[1][y][x].getMonstre()[z]];
                        }

                if (y==jeu->hero.m_personnage.getCoordonnee().y&&x==jeu->hero.m_personnage.getCoordonnee().x && ((!personnage->m_friendly) || effet.m_informations[5]))
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
                    miracleEnCours.m_infos.back()->m_position        =  info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           = &jeu->hero.m_personnage;
                }
            }


    //if (effet.m_informations[4])
      //  VerifierDeclencheursDegats(buf.y,buf.x);


    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Conditions(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    bool oui = false;
    if(effet.m_informations[1] == C_ANGLE)
        if((int)(personnage->getAngle()/45) == effet.m_informations[2])
            oui = true;

    if(effet.m_informations[1] == C_EQUIPEMENT)
        if((int)(jeu->hero.m_cas) == effet.m_informations[2])
            oui = true;

    if(effet.m_informations[1] == C_VARIABLE)
        if((int)personnage->m_variables_miracles.size() > effet.m_informations[2])
            if(personnage->m_variables_miracles[effet.m_informations[2]] == effet.m_informations[3])
                oui = true;

    if(effet.m_informations[1] == C_NO_VARIABLE)
    {
        if((int)personnage->m_variables_miracles.size() > effet.m_informations[2])
        {
            if(personnage->m_variables_miracles[effet.m_informations[2]] != effet.m_informations[3])
                oui = true;
        }
        else
            oui = true;
    }


    if(effet.m_informations[1] == C_CIBLELIFE)
        if(info.m_cible != NULL)
            if(info.m_cible->getCaracteristique().maxVie <= effet.m_informations[2]
            && info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0
            && info.m_cible->getCaracteristique().rang <= 1
            && info.m_cible->m_friendly != personnage->m_friendly)
                oui = true;

    if(effet.m_informations[1] == C_RANDOM)
        if(rand()%100 < effet.m_informations[2])
            oui = true;

    if(effet.m_informations[1] == C_CIBLE)
        if(info.m_cible)
        if(info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0
        && info.m_cible->m_friendly != personnage->m_friendly)
            oui = true;

    if(oui)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
        miracleEnCours.m_infos.back()->m_position        =  info.m_position;
        miracleEnCours.m_infos.back()->m_cible           =  info.m_cible;
    }

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

bool Map::Miracle_Bloquer(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    personnage->m_miracleBloquant = effet.m_informations[0];

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}


bool Map::Miracle_Variable(Jeu *jeu, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o, bool detruire)
{
    if((int)personnage->m_variables_miracles.size() <=  effet.m_informations[0])
        personnage->m_variables_miracles.resize(effet.m_informations[0] + 1,0);

    personnage->m_variables_miracles[effet.m_informations[0]] = effet.m_informations[1];

    for (unsigned p=0;p<effet.m_lien.size();p++)
    {
        miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
        miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
        miracleEnCours.m_infos.back()->m_position        = info.m_position;
        miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
    }

    miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 0;
}

void Map::GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles ,float temps,Jeu *jeu)
{
    bool continuerb = true;
    for (int i=0;i<(int)personnage->m_miracleEnCours.size() && continuerb;++i)
    if(personnage->m_miracleEnCours[i].m_modele >= 0 && personnage->m_miracleEnCours[i].m_modele < (int)miracles.size())
    {
        bool detruire = false;

        if(personnage->getCaracteristique().reserveFoi > personnage->getCaracteristique().maxFoi
        && miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi > 0)
            detruire = true;

        bool continuer=true;
        for (int o=0;o<(int)personnage->m_miracleEnCours[i].m_infos.size() && continuerb;o++)
        {
            if (personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours >= 0
             && personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours <  (int)miracles[personnage->m_miracleEnCours[i].m_modele].m_effets.size())
            {

                int effetEnCours = personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours;
                int type = -2;
                if(personnage->m_miracleEnCours[i].m_modele >= 0 && personnage->m_miracleEnCours[i].m_modele < (int)miracles.size())
                {
                    type = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type;

                    if(miracles[personnage->m_miracleEnCours[i].m_modele].m_consommer == effetEnCours
                     &&!personnage->m_miracleEnCours[i].m_dejaConsommeFoi)
                    {
                        Caracteristique temp = personnage->getCaracteristique();
                        temp.foi        -= miracles[personnage->m_miracleEnCours[i].m_modele].m_coutFoi
                                            + miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                        temp.vie        -= miracles[personnage->m_miracleEnCours[i].m_modele].m_coutVie
                                            + miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                        temp.reserveFoi += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                        temp.reserveVie += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                        personnage->setCaracteristique(temp);
                        miracles[personnage->m_miracleEnCours[i].m_modele].m_cur_time = 0;

                        personnage->m_miracleEnCours[i].m_dejaConsommeFoi = true;
                    }

                }

                if (type == AURA)
                    continuer = Miracle_Aura(       jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == DECLENCHEUR)
                    continuer = Miracle_Declencheur(jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == EFFET)
                    continuer = Miracle_Effet(      jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == CHARME)
                    continuer = Miracle_Charme(     jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type==CORPS_A_CORPS)
                    continuer = Miracle_CorpsACorps(jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type==REPETITION)
                    continuer = Miracle_Repetition( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type==PROJECTILE)
                    continuer = Miracle_Projectile( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == EFFET_GRAPHIQUE)
                    continuer = Miracle_EffetGraphique( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type==INVOCATION)
                {
                    int no = 0;
                    for(unsigned k = 0 ; k < m_monstre.size() ; ++k)
                        if(m_monstre[k] == personnage)
                            no = k;
                    continuer = Miracle_Invocation( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                    if(personnage != &jeu->hero.m_personnage)
                        personnage = m_monstre[no];
                }

                else if (type == CHANGEMENT_POSE)
                    continuer = Miracle_Pose(       jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == CHARGE)
                    continuer = Miracle_Charge(     jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == EFFET_ECRAN)
                    continuer = Miracle_EffetEcran( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == DEGATS)
                    continuer = Miracle_Degats( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == SOUFFLE)
                    continuer = Miracle_Souffle( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == ZONE)
                    continuer = Miracle_Zone(       jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == CONDITION)
                    continuer = Miracle_Conditions( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == BLOQUER)
                    continuer = Miracle_Bloquer   ( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == VARIABLE_MIRACLE)
                    continuer = Miracle_Variable  ( jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);


                else if (type == TELEPORT)
                    continuer = Miracle_Teleport(   jeu,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o, detruire);

                else if (type == -1)
                {
                    for (unsigned p=0;p<miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                    {
                        personnage->m_miracleEnCours[i].m_infos.push_back(new InfosEntiteMiracle ());
                        personnage->m_miracleEnCours[i].m_infos.back()->m_effetEnCours   = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p];
                        personnage->m_miracleEnCours[i].m_infos.back()->m_position       = personnage->m_miracleEnCours[i].m_infos[o]->m_position;
                        personnage->m_miracleEnCours[i].m_infos.back()->m_cible          = personnage->m_miracleEnCours[i].m_infos[o]->m_cible;
                    }

                    personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
                    continuer=false;
                }

                if (!continuer)
                    o --, continuer = true;
            }
            else
                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o),o--;
        }


        if (personnage->m_miracleEnCours[i].m_infos.empty())
        {
            if (personnage->m_miracleEnCours[i].m_dejaConsommeFoi)
            {
                Caracteristique temp = personnage->getCaracteristique();
                temp.foi        += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                temp.vie        += miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                temp.reserveFoi -= miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveFoi;
                temp.reserveVie -= miracles[personnage->m_miracleEnCours[i].m_modele].m_reserveVie;
                personnage->setCaracteristique(temp);
            }

            personnage->m_miracleEnCours.erase (personnage->m_miracleEnCours.begin()+i);
            continuerb = false;
        }
    }

    personnage->m_vientDetreTouche  = NULL;
}
