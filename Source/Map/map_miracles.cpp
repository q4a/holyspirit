#include "map.h"

#include <iostream>
#include <fstream>
#include <dirent.h>


using namespace std;
using namespace sf;

#include "../globale.h"
#include "../jeu.h"

bool Map::Miracle_Aura           (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

    return 1;
}

bool Map::Miracle_Declencheur    (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

                if(effet.m_informations[2] == 0)
                    miracleEnCours.m_coordonneeCible = personnage->m_vientDAttaquer;
                else
                    miracleEnCours.m_infos.back()->m_cible = personnage;
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
            }
        }
    }

    if(effet.m_informations[1] != -100)
    {
        info.m_informations[3] += temps * 100;
        if (info.m_informations[3] > effet.m_informations[1])
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

bool Map::Miracle_Effet  (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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
                effet.m_informations[5]);
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
                effet.m_informations[5]);

            info.m_position = info.m_cible->getCoordonneePixel();
        }
        else
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }


        for (int p=0;p<(int)effet.m_lien.size();p++)
        {
            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
            miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
            miracleEnCours.m_infos.back()->m_position        = info.m_position;
            miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
        }
    }
    else
    {
        Personnage *temp;
        if (effet.m_informations[0])
            temp = personnage;
        else
            temp = info.m_cible;

        if(!temp->m_effets[info.m_IDObjet].m_effet.m_actif)
        {
            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_Charme (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if(info.m_IDObjet == -1)
    {
        if (info.m_cible->m_friendly != personnage->m_friendly
            &&info.m_cible->getCaracteristique().niveau > 0 && info.m_cible->getCaracteristique().vitesse > 0)
        {
            info.m_cible->m_scriptAI     = Script (effet.m_chaine);

            info.m_cible->m_friendly     = true;

            info.m_cible->m_cible        = NULL;
            info.m_cible->frappeEnCours  = false;
            info.m_cible->setEtat(0);

            info.m_IDObjet    = 1;

            if(personnage == &hero->m_personnage)
                hero->m_amis.push_back(info.m_cible);

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
        Lumiere temp;
        temp.rouge = 255;
        temp.vert = 0;
        temp.bleu = 255;
        temp.intensite = 255;
        info.m_cible->setPorteeLumineuse(temp);

        if (!info.m_cible->EnVie())
        {
            if(personnage == &hero->m_personnage)
                for(unsigned o = 0 ; o < hero->m_amis.size() ;  ++o)
                    if(hero->m_amis[o] == info.m_cible)
                        hero->m_amis.erase(hero->m_amis.begin() + o);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }

        if (fabs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10 || fabs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
        {
            coordonnee temp;
            bool ok = false;
            int id = -1;
            for(int i = 0 ; i < m_monstre.size() ; ++i)
                if(&m_monstre[i] == info.m_cible)
                    id = i, m_decor[1][m_monstre[i].getCoordonnee().y][m_monstre[i].getCoordonnee().x].delMonstre(i);

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

        if (!info.m_cible->m_friendly)
        {
            info.m_cible->DetruireEffets();
            info.m_cible->m_scriptAI = m_ModeleMonstre[info.m_cible->getModele()].m_scriptAI;
            info.m_cible->setDepart();

            Lumiere temp;
            temp = m_ModeleMonstre[info.m_cible->getModele()].getPorteeLumineuse();
            info.m_cible->setPorteeLumineuse(temp);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

            return 0;
        }
    }

    return 1;
}

bool Map::Miracle_CorpsACorps (Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_cible != NULL)
    {
        if (fabs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > effet.m_informations[0]
         || fabs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > effet.m_informations[0] )
            personnage->setArrivee(info.m_cible->getCoordonnee());
        else
        {
            personnage->setArrivee(personnage->getProchaineCase());
            info.m_position.x = (float)personnage->getProchaineCase().x * COTE_TILE + 1;
            info.m_position.y = (float)personnage->getProchaineCase().y * COTE_TILE + 1;

            if( personnage->getArrivee().x == personnage->getCoordonnee().x
             && personnage->getArrivee().y == personnage->getCoordonnee().y)
            {
                personnage->Frappe(personnage->getCoordonneePixel(),info.m_cible->getCoordonneePixel());

                if(effet.m_informations[1])
                    personnage->m_miracleFrappeEnCours = true;
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
        }
    }
    else
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);

    return 1;
}

bool Map::Miracle_Repetition(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

bool Map::Miracle_Projectile(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet >= 0 && info.m_IDObjet < (int)m_projectile.size())
    {
        if (effet.m_informations[5] > 0)
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
            }
        }

        if (!m_projectile[info.m_IDObjet].m_effet.m_actif)
            m_projectile[info.m_IDObjet].m_actif = false;

        if (m_projectile[info.m_IDObjet].m_actif)
            m_projectile[info.m_IDObjet].m_effet.Animer(temps);

        if (!m_projectile[info.m_IDObjet].m_effet.m_actif)
            m_projectile[info.m_IDObjet].m_actif = false;

        if (!m_projectile[info.m_IDObjet].m_actif)
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

        coordonnee cible;

        if (info.m_cible != NULL)
            cible = info.m_cible->getProchaineCase();
        else
            cible = miracleEnCours.m_coordonneeCible;


        info.m_IDObjet = AjouterProjectile( info.m_position,
                                            cible,personnage->getCoordonnee(),10,effet.m_informations[0],
                                            (float)effet.m_informations[1]*M_PI/180,!personnage->m_friendly,
                                            moteurGraphique->getTileset(modele.m_tileset[effet.m_sequence]));

        if (effet.m_informations[2])
            m_projectile.back().m_cible = cible;
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

bool Map::Miracle_EffetGraphique(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (info.m_IDObjet >= 0 && info.m_IDObjet < (int)m_effets.size())
    {
        m_effets[info.m_IDObjet].Animer(temps);
        if (!m_effets[info.m_IDObjet].m_actif)
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
        m_effets.back().Initialiser(coordonnee((int)(((float)info.m_position.x - (float)info.m_position.y) * 64 / COTE_TILE),
                                               (int)(((float)info.m_position.x + (float)info.m_position.y) * 64 / COTE_TILE)));
        m_effets.back().Animer(0);

        info.m_IDObjet               = m_effets.size()-1;

        if (m_effets.back().m_position.y / COTE_TILE>=0&&m_effets.back().m_position.y / COTE_TILE<m_dimensions.y)
            if (m_effets.back().m_position.x / COTE_TILE>=0&&m_effets.back().m_position.x / COTE_TILE<m_dimensions.x)
                m_decor[1][(int)(m_effets.back().m_position.y / COTE_TILE)][(int)(m_effets.back().m_position.x / COTE_TILE)].setEffetGraphique(m_effets.size()-1);
    }
    return 1;
}

bool Map::Miracle_Invocation(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

        coordonnee positionCase;
        positionCase.x = cible.x-1;
        positionCase.y = cible.y-1;

        if (positionCase.x<0)
            positionCase.x=0;
        if (positionCase.y<0)
            positionCase.y=0;

        while ((getCollision(positionCase.x,positionCase.y) || ( positionCase.x==hero->m_personnage.getCoordonnee().x && positionCase.y==hero->m_personnage.getCoordonnee().y )) && invoquer )
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
            for (unsigned p=0;p < effet.m_lien.size();p++)
            {
                miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                miracleEnCours.m_infos.back()->m_effetEnCours    = effet.m_lien[p];
                miracleEnCours.m_infos.back()->m_position        = info.m_position;
                miracleEnCours.m_infos.back()->m_cible           = info.m_cible;
            }

            info.m_position.x = positionCase.x*COTE_TILE;
            info.m_position.y = positionCase.y*COTE_TILE;



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
                console->Ajouter("Chargement de : "+effet.m_chaine+" terminé",0);
                numero=m_ModeleMonstre.size()-1;
            }

            m_monstre.push_back(Monstre ());

            m_monstre.back().Charger(numero,&m_ModeleMonstre[numero]);
            m_monstre.back().setCoordonnee(positionCase),m_monstre.back().setDepart();

            coordonnee pos;
            pos.x=(int)(((m_monstre.back().getCoordonneePixel().x-m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE*64));
            pos.y=(int)(((m_monstre.back().getCoordonneePixel().x+m_monstre.back().getCoordonneePixel().y)*64/COTE_TILE)/2+32)*2;

            m_monstre.back().m_entite_graphique.Initialiser(pos);

            m_monstre.back().m_friendly = personnage->m_friendly;

            if(personnage == &hero->m_personnage)
                hero->m_amis.push_back(&m_monstre.back());

            info.m_cible = &m_monstre.back();

            m_decor[1][positionCase.y][positionCase.x].setMonstre(m_monstre.size()-1);

            info.m_IDObjet=m_monstre.size()-1;

            if(modele.m_golem)
            {
                Caracteristique temp = m_monstre.back().getCaracteristique();
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

                m_monstre.back().setCaracteristique(temp);

                bool mir = false;

                for(unsigned j = 0 ; j < miracleEnCours.m_source->m_miracles_benedictions.size() ; ++j)
                {
                    if(!mir)
                    {
                        m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.push_back(Miracle (
                                    miracleEnCours.m_source->m_miracles_benedictions[j].m_chemin,
                                    temp,0));

                        m_monstre.back().m_miracleEnCours.push_back(EntiteMiracle ());
                        m_monstre.back().m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                        m_monstre.back().m_miracleEnCours.back().m_modele = m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.size() - 1;

                        m_monstre.back().m_miracleEnCours.back().m_infos.back()->m_position.x = m_monstre.back().getCoordonneePixel().x;
                        m_monstre.back().m_miracleEnCours.back().m_infos.back()->m_position.y = m_monstre.back().getCoordonneePixel().y;

                        mir = true;
                    }
                    else
                    {
                        m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.back().Concatenencer(
                                    miracleEnCours.m_source->m_miracles_benedictions[j].m_chemin,
                                    temp,0);
                    }
                }

                for(unsigned i = 0 ; i < miracleEnCours.m_source->m_benedictions.size() ; ++i)
                {
                    if(!mir)
                    {
                        m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.push_back(Miracle ());

                        m_monstre.back().m_miracleEnCours.push_back(EntiteMiracle ());
                        m_monstre.back().m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                        m_monstre.back().m_miracleEnCours.back().m_modele = m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.size() - 1;

                        m_monstre.back().m_miracleEnCours.back().m_infos.back()->m_position.x = m_monstre.back().getCoordonneePixel().x;
                        m_monstre.back().m_miracleEnCours.back().m_infos.back()->m_position.y = m_monstre.back().getCoordonneePixel().y;

                    }

                    ChargerMiracleBenediction(miracleEnCours.m_source->m_benedictions[i],m_ModeleMonstre[m_monstre.back().getModele()].m_miracles.back(),mir);
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

            if(personnage == &hero->m_personnage)
                for(unsigned o = 0 ; o < hero->m_amis.size() ;  ++o)
                    if(hero->m_amis[o] == info.m_cible)
                        hero->m_amis.erase(hero->m_amis.begin() + o);

            miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
            return 0;
        }

        if (fabs(info.m_cible->getCoordonnee().x - personnage->getCoordonnee().x) > 10 || fabs(info.m_cible->getCoordonnee().y - personnage->getCoordonnee().y) > 10)
        {
            coordonnee temp;
            bool ok = false;
            int id = -1;
            for(int i = 0 ; i < m_monstre.size() ; ++i)
                if(&m_monstre[i] == info.m_cible)
                    id = i, m_decor[1][m_monstre[i].getCoordonnee().y][m_monstre[i].getCoordonnee().x].delMonstre(i);

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
    }
    else
    {
        miracleEnCours.m_infos.erase(miracleEnCours.m_infos.begin()+o);
        return 0;
    }
    return 1;
}

bool Map::Miracle_Pose(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if (effet.m_informations[0] != -1)
        personnage->setJustEtat(effet.m_informations[0]);

    if (effet.m_informations[1] != -1)
        personnage->setPose(effet.m_informations[1]);

    personnage->addAngle(effet.m_informations[2]*45);

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

bool Map::Miracle_Charge(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

        if (((  miracleEnCours.m_coordonneeDepart.x  < miracleEnCours.m_coordonneeCible.x&&personnage->getCoordonneePixel().x>miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||( miracleEnCours.m_coordonneeDepart.x  > miracleEnCours.m_coordonneeCible.x&&personnage->getCoordonneePixel().x<miracleEnCours.m_coordonneeCible.x*COTE_TILE)
            ||  miracleEnCours.m_coordonneeDepart.x  == miracleEnCours.m_coordonneeCible.x)

           &&((  miracleEnCours.m_coordonneeDepart.y   > miracleEnCours.m_coordonneeCible.y&&personnage->getCoordonneePixel().y<miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||(   miracleEnCours.m_coordonneeDepart.y   < miracleEnCours.m_coordonneeCible.y&&personnage->getCoordonneePixel().y>miracleEnCours.m_coordonneeCible.y*COTE_TILE)
           ||    miracleEnCours.m_coordonneeDepart.y   == miracleEnCours.m_coordonneeCible.y)

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

    if ( !(personnage->getCoordonnee().x == miracleEnCours.m_coordonneeCible.x && personnage->getCoordonnee().y == miracleEnCours.m_coordonneeCible.y) )
    {
        float m = atan2((float)miracleEnCours.m_coordonneeCible.x * COTE_TILE - (float)personnage->getCoordonneePixel().x,
                        (float)miracleEnCours.m_coordonneeCible.y * COTE_TILE - (float)personnage->getCoordonneePixel().y);

        personnage->setPousse(coordonneeDecimal  (sin(m) * effet.m_informations[0],
                                                  cos(m) * effet.m_informations[0]));
    }

    info.m_IDObjet= 1;

    return 1;
}

bool Map::Miracle_EffetEcran(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

bool Map::Miracle_Degats(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    int deg = rand() % (int)(effet.m_informations[1] - effet.m_informations[0] + 1) + effet.m_informations[0];

    if (info.m_cible != NULL)
    {
        if (info.m_cible->getCoordonnee().y >=0 && info.m_cible->getCoordonnee().y < (int)m_decor[0].size())
            if (info.m_cible->getCoordonnee().x >=0 && info.m_cible->getCoordonnee().x < (int)m_decor[0][info.m_cible->getCoordonnee().y].size())
            {
                info.m_position = info.m_cible->getCoordonneePixel();
                if(deg != 0)
                    InfligerDegats(info.m_cible, deg, effet.m_informations[2], hero, 0, effet.m_informations[3]);
            }
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

bool Map::Miracle_Souffle(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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

bool Map::Miracle_Zone(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    coordonnee buf( (int)((int)(info.m_position.x+COTE_TILE*0.5f)/COTE_TILE),
                    (int)((int)(info.m_position.y+COTE_TILE*0.5f)/COTE_TILE));

    for(int x = buf.x - effet.m_informations[3];
            x <= buf.x + effet.m_informations[3]; ++x)
    for(int y = buf.y - effet.m_informations[3];
            y <= buf.y + effet.m_informations[3]; ++y)
        if (y>=0&&x>=0&&y<m_dimensions.y&&x<m_dimensions.x)
            {

                for (unsigned z = 0 ; z < m_decor[1][y][x].getMonstre().size() ; ++z)
                    if (m_decor[1][y][x].getMonstre()[z]>=0&&m_decor[1][y][x].getMonstre()[z]<(int)m_monstre.size())
                        if (m_monstre[m_decor[1][y][x].getMonstre()[z]].EnVie()
                        && ((m_monstre[m_decor[1][y][x].getMonstre()[z]].m_friendly != personnage->m_friendly)
                         || (m_monstre[m_decor[1][y][x].getMonstre()[z]].m_friendly == personnage->m_friendly && effet.m_informations[5]/*&& y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x*/ /*&& ((!personnage->m_friendly) || effet.m_informations[5]))*/))
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].getCaracteristique().rang>=0
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].m_actif
                        &&m_monstre[m_decor[1][y][x].getMonstre()[z]].getCaracteristique().niveau>=0)
                        {
                            miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                            miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
                            miracleEnCours.m_infos.back()->m_position        =  info.m_position;
                            miracleEnCours.m_infos.back()->m_cible           = &m_monstre[m_decor[1][y][x].getMonstre()[z]];
                        }

                if (y==hero->m_personnage.getCoordonnee().y&&x==hero->m_personnage.getCoordonnee().x && ((!personnage->m_friendly) || effet.m_informations[5]))
                {
                    miracleEnCours.m_infos.push_back(new InfosEntiteMiracle ());
                    miracleEnCours.m_infos.back()->m_effetEnCours    =  effet.m_informations[0];
                    miracleEnCours.m_infos.back()->m_position        =  info.m_position;
                    miracleEnCours.m_infos.back()->m_cible           = &hero->m_personnage;
                }
            }


    if (effet.m_informations[4])
        VerifierDeclencheursDegats(buf.y,buf.x);


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

bool Map::Miracle_Conditions(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    bool oui = false;
    if(effet.m_informations[1] == C_ANGLE)
        if((int)(personnage->getAngle()/45) == effet.m_informations[2])
            oui = true;

    if(effet.m_informations[1] == C_VARIABLE)
        if(personnage->m_variables_miracles.size() > effet.m_informations[2])
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
            && info.m_cible->m_friendly != personnage->m_friendly)
                oui = true;

    if(effet.m_informations[1] == C_RANDOM)
        if(rand()%100 < effet.m_informations[2])
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

bool Map::Miracle_Bloquer(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
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


bool Map::Miracle_Variable(Hero *hero, Personnage *personnage, Miracle &modele, Effet &effet, EntiteMiracle &miracleEnCours, InfosEntiteMiracle &info, float temps, int o)
{
    if(personnage->m_variables_miracles.size() <=  effet.m_informations[0])
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

void Map::GererMiracle(Personnage *personnage,std::vector<Miracle> &miracles ,float temps,coordonnee positionHero,Hero *hero)
{
    bool continuerb = true;
    for (int i=0;i<(int)personnage->m_miracleEnCours.size() && continuerb;++i)
    {
        bool continuer=true;
        for (int o=0;o<(int)personnage->m_miracleEnCours[i].m_infos.size() && continuerb;o++)
        {
            if (personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours >= 0)
            {

                int effetEnCours = personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours;
                int type = miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[personnage->m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type;

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

                if (type == AURA)
                    continuer = Miracle_Aura(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == DECLENCHEUR)
                    continuer = Miracle_Declencheur(hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET)
                    continuer = Miracle_Effet(      hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CHARME)
                    continuer = Miracle_Charme(     hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==CORPS_A_CORPS)
                    continuer = Miracle_CorpsACorps(hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==REPETITION)
                    continuer = Miracle_Repetition( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==PROJECTILE)
                    continuer = Miracle_Projectile( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET_GRAPHIQUE)
                    continuer = Miracle_EffetGraphique( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type==INVOCATION)
                {
                    int no = 0;
                    for(unsigned k = 0 ; k < m_monstre.size() ; ++k)
                        if(&m_monstre[k] == personnage)
                            no = k;
                    continuer = Miracle_Invocation( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                    if(personnage != &hero->m_personnage)
                        personnage = &m_monstre[no];
                }

                else if (type == CHANGEMENT_POSE)
                    continuer = Miracle_Pose(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CHARGE)
                    continuer = Miracle_Charge(     hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == EFFET_ECRAN)
                    continuer = Miracle_EffetEcran( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == DEGATS)
                    continuer = Miracle_Degats( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == SOUFFLE)
                    continuer = Miracle_Souffle( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == ZONE)
                    continuer = Miracle_Zone(       hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == CONDITION)
                    continuer = Miracle_Conditions( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == BLOQUER)
                    continuer = Miracle_Bloquer   ( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

                else if (type == VARIABLE_MIRACLE)
                    continuer = Miracle_Variable  ( hero,personnage, miracles[personnage->m_miracleEnCours[i].m_modele],
                                                    miracles[personnage->m_miracleEnCours[i].m_modele].m_effets[effetEnCours],
                                                    personnage->m_miracleEnCours[i], *personnage->m_miracleEnCours[i].m_infos[o],
                                                    temps, o);

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
                personnage->m_miracleEnCours[i].m_infos.erase(personnage->m_miracleEnCours[i].m_infos.begin()+o);
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
