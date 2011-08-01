

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


#ifndef HEROCPP
#define HEROCPP

#include "../constantes.h"
#include "hero.h"
#include "../globale.h"
#include "../jeu.h"


#include <iostream>
#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <dirent.h>
#include <sstream>

using namespace std;
using namespace sf;

int crypter(int);
int decrypter(int);

//int TrierInventaire(std::vector<Objet>*, int, bool = 0);

std::string DecouperTexte(std::string texte, int tailleCadran, int tailleTexte);

inline sf::Vector2f AutoScreenAdjust(float x, float y, float decalage = 0)
{
    sf::Vector2f temp;
    temp.x = x + (configuration->Resolution.x - 800) * 0.5;
    temp.y = y + (configuration->Resolution.y - 600) - decalage * configuration->Resolution.h/600;
    return temp;
}


bool AjouterObjetInventaire(Objet newObj, std::vector<Objet>* inventaire, coordonnee taille, bool infini)
{
    bool continuer = true;
    bool retour = false;

    if (inventaire)
    {
        inventaire->push_back(newObj);

        for (int y=0;continuer;y++)
            for (int x=0;x<taille.w&&continuer;x++)
            {
                if (y < taille.h || infini)
                {
                    bool ajouter=true;
                    for (int h=0;h<inventaire->back().getTaille().y && ajouter;h++)
                        for (int w=0;w<inventaire->back().getTaille().x && ajouter;w++)
                        {
                            if (x+w < taille.w && (infini || y+h < taille.h))
                            {
                                for (int j=0;j<(int)inventaire->size()-1 && ajouter;j++)
                                    if ((*inventaire)[j].m_equipe<0)// || ((*inventaire)[j].m_equipe_set != m_weaponsSet && (*inventaire)[j].m_equipe_set >= 0))
                                        for (int Y=0;Y<(*inventaire)[j].getTaille().y && ajouter;Y++)
                                            for (int X=0;X<(*inventaire)[j].getTaille().x && ajouter;X++)
                                                if ((*inventaire)[j].getPosition().x+X==x+w && (*inventaire)[j].getPosition().y+Y==y+h)
                                                    ajouter = false;
                            }
                            else
                                ajouter = false;
                        }

                    if (ajouter)
                    {
                        continuer = false;
                        retour = true;
                        inventaire->back().setPosition(x,y);
                    }
                }
                else
                    continuer = false, inventaire->erase(inventaire->begin() + inventaire->size() - 1);
            }
    }

    return (retour);
}

Hero::Hero()
{
    Reset();
}

Hero::~Hero()
{
    if (configuration->debug)
        console->Ajouter("Destruction du héro ...");
}

void Hero::Reset()
{
    m_classe = Classe ();

    m_contenuSave.clear();
    m_docs.clear();
    m_potales.clear();
    m_lvl_miracles.clear();
    m_lvl_miracles_new.clear();
    m_amis.clear();
    m_coffre.clear();
    m_items_cooldown.clear();
    m_inventaire.clear();
    m_weaponMiracle.clear();
    m_quetes.clear();

    m_personnage = Personnage ();

    m_weaponsSet = 0;

    m_personnage.setEtat(ARRET);

    m_chercherSac.x=-1;
    m_chercherSac.y=-1;

    coordonnee position;
    position.x=-10000;
    position.y=-10000;

    m_personnage.setCoordonnee(position);


    m_personnage.m_friendly = 1;

    Caracteristique temp;

    temp.vitesse            = 0.9;
    temp.niveau             = 1;
    temp.nom                = "Héros";

    m_personnage.setCaracteristique(temp);
    m_personnage.m_heroic = true;

    m_personnage.m_friendly = true;

    m_objetEnMain=-1;
    m_achat=false;

    RecalculerCaracteristiques();

    miracleEnCours=0;

    m_miracleEnMain = -1;

    m_cas=0;

    m_argent=0;
    m_holywater=0;

    m_angleFleche = 0;


    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_cheminModeleNouveau[i]="",m_cheminModele[i]="";

    m_defilement_trader=0;

    for(int i = 0 ; i < 8 ; ++i)
        m_raccourcis[i].no = -1, m_raccourcis[i].miracle = false;

    m_queteSelectionnee = -1;
    m_docSelectionne = -1;

    m_defil_quetes = 0;
    m_defil_cdoc = 0;
    m_defil_ldoc = 0;
    m_max_defil_cdoc = 0;
    m_max_defil_ldoc = 0;

    m_personnage.m_miracleALancer = -1;

    m_miracle_droite[0] = -1;
    m_miracle_droite[1] = -1;
    m_miracle_gauche[0] = -1;
    m_miracle_gauche[1] = -1;

    m_last_potale = 0;

    m_no_schema_bless = -1;
    m_no_schema_craft = -1;
    m_no_result_bless = -1;
    m_no_result_craft = -1;

    m_craft_time = 0;
    m_bless_time = 0;

    m_case_visee.x = -1;
    m_case_visee.y = -1;

    m_trier_en_hauteur = false;

    m_select_friend = -1;

    newQuest = false;
    newDoc = false;

    m_lastRaccourcis = -1;
}

void Hero::ResetQuests()
{
    m_quetes.clear();

    for(unsigned i = 0 ; i < m_inventaire.size(); ++i)
        if(m_inventaire[i].m_type == AUCUN && m_inventaire[i].getRarete() == QUETE)
           m_inventaire.erase(m_inventaire.begin() + i--);
}

void Hero::Sauvegarder()
{
    console->Ajouter("");
    console->Ajouter("Sauvegarde du héro...");

    ofstream fichier((configuration->chemin_temps+"Save.sav.hs").c_str(), ios::out | ios::trunc | ios::binary);

    m_personnage.DesactiverEffets();

    if (fichier)
    {
        if (configuration->debug)
            console->Ajouter("Ouverture du fichier.");

        fichier<<VERSION_SAVE<<endl;

        //fichier<<m_caracteristiques.nom.c_str()<<" "<<endl;
        fichier<<m_cheminClasse<<" "<<endl;

        fichier<<m_personnage.getCaracteristique().pointAme<<endl;
        fichier<<m_personnage.getCaracteristique().niveau<<endl;
        fichier<<m_personnage.getCaracteristique().force<<endl;
        fichier<<m_personnage.getCaracteristique().dexterite<<endl;
        fichier<<m_personnage.getCaracteristique().vitalite<<endl;
        fichier<<m_personnage.getCaracteristique().piete<<endl;
        fichier<<m_personnage.getCaracteristique().charisme<<endl;
        fichier<<m_personnage.getCaracteristique().pts_restant<<endl;
        fichier<<m_personnage.getCaracteristique().miracles_restant<<endl;

        fichier<<m_argent<<endl;
        fichier<<m_holywater<<endl;

        fichier<<configuration->minute<<endl;
        fichier<<configuration->heure<<endl;
        fichier<<configuration->jour<<endl;

        fichier<<m_last_potale<<endl;

        fichier<<m_craft_time<<endl;
        fichier<<m_bless_time<<endl;
        fichier<<m_craft_time_max<<endl;
        fichier<<m_bless_time_max<<endl;
        fichier<<m_no_schema_craft<<endl;
        fichier<<m_no_schema_bless<<endl;
        fichier<<m_no_result_craft<<endl;
        fichier<<m_no_result_bless<<endl;

        for(int i = 0 ; i < 8 ; ++i)
            fichier<<m_raccourcis[i].miracle<<" "<<m_raccourcis[i].no<<endl;

        fichier<<m_miracle_droite[0]<<endl;
        fichier<<m_miracle_droite[1]<<endl;
        fichier<<m_miracle_gauche[0]<<endl;
        fichier<<m_miracle_gauche[1]<<endl;

        if (configuration->debug)
            console->Ajouter("/Ecriture des caracterstiques.");

        for (int i=0;i<(int)m_inventaire.size();++i)
            m_inventaire[i].SauvegarderTexte(&fichier);

        if (configuration->debug)
            console->Ajouter("/Ecriture des objets.");

        fichier<<'$'<<endl;

        for (int i=0;i<(int)m_coffre.size();++i)
            m_coffre[i].SauvegarderTexte(&fichier);

        if (configuration->debug)
            console->Ajouter("/Ecriture des objets du coffre.");

        fichier<<'$'<<endl;


        for (int i=0;i<(int)m_lvl_miracles.size();++i)
            fichier<<"* l"<<m_lvl_miracles[i]<<" $ ";

        if (configuration->debug)
            console->Ajouter("/Ecriture des miracles.");

        fichier<<'$'<<endl;

        for (int i=0;i<(int)m_docs.size();++i)
            fichier<<" d"<<m_docs[i].m_chemin<<" "<<endl;

        if (configuration->debug)
            console->Ajouter("/Ecriture des docs.");

        fichier<<'$'<<endl;

        for (int i=0;i<(int)m_quetes.size();++i)
            m_quetes[i].SauvegarderTexte(&fichier);

        if (configuration->debug)
            console->Ajouter("/Ecriture des quetes.");

        fichier<<'$'<<endl;

        for (int i=0;i<(int)m_potales.size();++i)
        {
            fichier<<" p ";

            fichier<<" x"<<m_potales[i].position.x;
            fichier<<" y"<<m_potales[i].position.y;
            fichier<<" t"<<m_potales[i].nom;
            fichier<<" m"<<m_potales[i].chemin;

            fichier<<" $ ";
        }


        if (configuration->debug)
            console->Ajouter("/Ecriture des potales.");

        fichier<<'$'<<endl;

        fichier.close();

        console->Ajouter("Sauvegarde du héro terminée !");

    }
    else
        console->Ajouter("Impossible de sauvegarder le héro !",1);

    cDAT fichierSave;

    fichierSave.Create(m_contenuSave, configuration->chemin_saves+m_chemin_save);

    m_personnage.ActiverEffets();
}

void Hero::SauvegarderApercu()
{
    sf::RenderImage render;
    render.Create(256,256);
    render.Clear(sf::Color(0,0,0,0));

    if (configuration->debug)
            console->Ajouter("/Génération de l'image...");


    int etat = m_personnage.getEtat();
    int pose = m_personnage.getPose();
    int angle = m_personnage.getAngle();
    int next_angle = m_personnage.getNextAngle();

    m_personnage.setJustEtat(0);
    m_personnage.setPose(0);
    m_personnage.setForcedAngle(315);

    CalculerOrdreAffichage();

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        if (m_ordreAffichage[i]!=-1)
        {
            m_personnage.m_entite_graphique.m_tileset = &m_modelePersonnage[m_ordreAffichage[i]].m_tileset[0][5];
            m_personnage.Animer(&m_modelePersonnage[m_ordreAffichage[i]], 0, true);
            m_personnage.m_entite_graphique.Generer();

            m_personnage.m_entite_graphique.SetX(128);
            m_personnage.m_entite_graphique.SetY(128);

            render.Draw(m_personnage.m_entite_graphique);
        }

    m_personnage.setJustEtat(etat);
    m_personnage.setPose(pose);
    m_personnage.setAngle(next_angle);
    m_personnage.setForcedAngle(angle);

    if (configuration->debug)
            console->Ajouter("/Image affichée.");

    render.Display();

    render.GetImage().SaveToFile(configuration->chemin_temps + "Image.png");

    if (configuration->debug)
            console->Ajouter("/Image générée.");
}
void Hero::Charger(const std::string &chemin_save)
{
    console->Ajouter("Chargement du hero.");
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_cheminModele[i]="";

    bool nouveau=true;
    string chemin;

    m_chemin_save = chemin_save;

    cDAT reader;
    if (reader.Read(configuration->chemin_saves+m_chemin_save))
    {
        ifstream* fichier=reader.GetInfos(configuration->chemin_temps+"Save.sav.hs");
        //fichier.open((configuration->chemin_saves+"hero.sav.hs").c_str(), ios::in | ios::binary);
        if (fichier)
        {
            char caractere;
            Caracteristique charTemp;
            charTemp=m_personnage.getCaracteristique();

            int temp = 0;
            *fichier>>temp;


            if(temp < 7)
                *fichier>>m_caracteristiques.nom;

            charTemp.nom = m_caracteristiques.nom;

            *fichier>>m_cheminClasse;

            if(temp < 4)
                *fichier>>charTemp.vitesse;

            *fichier>>charTemp.pointAme;
            *fichier>>charTemp.niveau;

            charTemp.vitesse = 0.8;

            *fichier>>charTemp.force;
            *fichier>>charTemp.dexterite;
            *fichier>>charTemp.vitalite;
            *fichier>>charTemp.piete;
            *fichier>>charTemp.charisme;
            *fichier>>charTemp.pts_restant;
            *fichier>>charTemp.miracles_restant;

            *fichier>>m_argent;
            *fichier>>m_holywater;

            *fichier>>configuration->minute;
            *fichier>>configuration->heure;
            *fichier>>configuration->jour;

            *fichier>>m_last_potale;

             if(temp >= 5)
             {
                *fichier>>m_craft_time;
                *fichier>>m_bless_time;
                *fichier>>m_craft_time_max;
                *fichier>>m_bless_time_max;
                *fichier>>m_no_schema_craft;
                *fichier>>m_no_schema_bless;
                *fichier>>m_no_result_craft;
                *fichier>>m_no_result_bless;
             }


            for(int i = 0 ; i < 8 ; ++i)
                *fichier>>m_raccourcis[i].miracle>>m_raccourcis[i].no;

            if(temp >= 10)
            {
                *fichier>>m_miracle_droite[0];
                *fichier>>m_miracle_droite[1];
                *fichier>>m_miracle_gauche[0];
                *fichier>>m_miracle_gauche[1];
            }
            else
            {
                *fichier>>m_miracle_droite[0];
                if(temp >= 3)
                    *fichier>>m_miracle_gauche[0];
            }


            charTemp.ancienPointAme=charTemp.pointAme,charTemp.positionAncienAme=charTemp.pointAme;

            if (configuration->debug)
                console->Ajouter("/Lectures des caracteristiques.");

            do
            {
                fichier->get(caractere);
                if (caractere=='o')
                {
                    int pos=fichier->tellg();
                    m_inventaire.push_back(Objet ());
                    m_inventaire.back().ChargerTexte(fichier,m_caracteristiques,true);
                    m_inventaire.back().Charger(m_inventaire.back().getChemin(),m_caracteristiques,true);
                    fichier->seekg(pos, ios::beg);
                    m_inventaire.back().m_benedictions.clear();
                    m_inventaire.back().ChargerTexte(fichier,m_caracteristiques);

                    if(m_inventaire.back().m_equipe>=0
                    &&(m_inventaire.back().m_equipe_set == m_weaponsSet || m_inventaire.back().m_equipe_set == -1)
                    &&m_inventaire.back().m_type==ARME)
                    {
                        if (m_inventaire.back().m_shoot_weapon)
                            m_personnage.m_shooter=true;
                        else
                            m_personnage.m_shooter=false;
                    }

                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des objets.");

            do
            {
                fichier->get(caractere);
                if (caractere=='o')
                {
                    int pos=fichier->tellg();
                    m_coffre.push_back(Objet ());
                    m_coffre.back().ChargerTexte(fichier,m_caracteristiques,true);
                    m_coffre.back().Charger(m_coffre.back().getChemin(),m_caracteristiques,true);
                    fichier->seekg(pos, ios::beg);
                    m_coffre.back().m_benedictions.clear();
                    m_coffre.back().ChargerTexte(fichier,m_caracteristiques);

                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des objets du coffre.");


            if(temp < 6)
                m_lvl_miracles.push_back(1);

            do
            {
                fichier->get(caractere);
                if (caractere=='*')
                {
                    m_lvl_miracles.push_back(0);
                    do
                    {
                        fichier->get(caractere);
                        if (caractere=='l')
                            *fichier>>m_lvl_miracles.back();

                        if (fichier->eof())
                            throw "Impossible de charger la sauvegarde";
                    }
                    while (caractere!='$');
                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des miracles.");

            if(temp == 8)
            do
            {
                fichier->get(caractere);
                if (caractere=='d')
                {
                    Document doc;
                    doc.ChargerTexte(fichier);
                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if(temp >= 9)
            do
            {
                fichier->get(caractere);
                if (caractere=='d')
                {
                    std::string chemin2;
                    *fichier>>chemin2;
                    Objet doc;
                    doc.Charger(chemin2,m_caracteristiques,true);

                    m_docs.push_back(Document ());
                    m_docs.back().m_chemin = doc.getChemin();
                    m_docs.back().m_nom = doc.getNom();
                    m_docs.back().m_description = configuration->getText(7,doc.m_doc);

                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            } while(caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des docs.");
            do
            {
                fichier->get(caractere);
                if (caractere=='q')
                {
                    m_quetes.push_back(Quete ());
                    m_quetes.back().ChargerTexte(fichier);
                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des quêtes.");

            do
            {
                fichier->get(caractere);
                if (caractere=='p')
                {
                    m_potales.push_back(Potale ());
                    do
                    {
                        fichier->get(caractere);
                        if (caractere=='m')
                            *fichier>>m_potales.back().chemin;

                        else if (caractere=='t')
                            *fichier>>m_potales.back().nom;

                        else if (caractere=='x')
                            *fichier>>m_potales.back().position.x;
                        else if (caractere=='y')
                            *fichier>>m_potales.back().position.y;


                        if (fichier->eof())
                        {
                            throw "Impossible de charger la potale";
                        }
                    }
                    while (caractere!='$');
                    fichier->get(caractere);
                }
                if (fichier->eof())
                    throw "Impossible de charger la sauvegarde";
            }
            while (caractere!='$');

            if (configuration->debug)
                console->Ajouter("/Lectures des potales.");

            UpdateRaccourcis();

            m_personnage.setCaracteristique(charTemp);
            nouveau=false;
        }
        fichier->close();
        delete fichier;
    }
    //}
    //}
    // closedir(repertoire);

    m_classe.Charger(m_cheminClasse, m_lvl_miracles, m_caracteristiques);
    m_lvl_miracles.resize(m_classe.miracles.size(),0);

    m_lvl_miracles.front() = 1;

    m_lvl_miracles_new = m_lvl_miracles;

    ChargerModele();

    RecalculerCaracteristiques();
    RecalculerCaracteristiques();

    for(unsigned i = 0 ; i < m_docs.size() ; ++i)
    {
        m_docs[i].m_description = DecouperTexte(m_docs[i].m_description,m_classe.position_contenu_docs.w,16);
        m_docs[i].GenerateDescription();
    }

    if (nouveau)
    {
        sf::Color color;
        color.r=255;
        color.g=255;
        color.b=255;

        Caracteristique charTemp;
        charTemp=m_personnage.getCaracteristique();

        charTemp.niveau = 1;
        charTemp.pointAme = 0;
        charTemp.force=m_classe.caracteristique.force;
        charTemp.vitalite=m_classe.caracteristique.vitalite;
        charTemp.dexterite=m_classe.caracteristique.dexterite;
        charTemp.piete=m_classe.caracteristique.piete;
        charTemp.charisme=m_classe.caracteristique.charisme;
        charTemp.nom = m_caracteristiques.nom;

        m_personnage.setCaracteristique(charTemp);

        /*int k = 0;
        for(unsigned i = 0 ; i < m_lvl_miracles.size() ; ++i)
        {
            m_lvl_miracles[i] = 1;
            if(i > 0 && k < 8 && m_classe.miracles[i].m_buf == -1)
                m_raccourcis[k].no = i, m_raccourcis[k++].miracle = true;
        }*/

        RecalculerCaracteristiques();

        for (int i=0;i<(int)m_classe.equipementParDefaut.size();++i)
        {
            Objet temp;

            temp.Charger(m_classe.equipementParDefaut[i],m_caracteristiques);
            temp.Generer(1);
            AjouterObjet(temp,false,false);
        }
        ChargerModele();

        RecalculerCaracteristiques();
    }

    m_caracteristiques.vie=m_caracteristiques.maxVie;
    m_caracteristiques.foi=m_caracteristiques.maxFoi;

    Caracteristique temp = m_personnage.getCaracteristique();
    temp.maxVie = m_caracteristiques.maxVie;
    temp.vie = m_caracteristiques.maxVie;

    temp.maxFoi = m_caracteristiques.maxFoi;
    temp.foi = m_caracteristiques.maxFoi;
    m_personnage.setCaracteristique(temp);

    if (configuration->debug)
        console->Ajouter("/Chargement du héro terminé");
}

bool Hero::ChargerPresentation(const std::string &chemin_save)
{
    bool erreur = false;
    cDAT reader;
    if (reader.Read(configuration->chemin_saves+chemin_save))
    {
        ifstream* fichier=reader.GetInfos(configuration->chemin_temps+"Save.sav.hs");
        if (fichier)
        {
            int temp = 0;
            *fichier>>temp;

            if(temp < 7)
                *fichier>>m_caracteristiques.nom;

            *fichier>>m_cheminClasse;

            if(temp < 4)
                *fichier>>m_caracteristiques.vitesse;

            *fichier>>m_caracteristiques.pointAme;
            *fichier>>m_caracteristiques.niveau;
        }
        fichier->close();
        delete fichier;
    }

    return erreur;
}

void Hero::ChargerModele(bool tout)
{
    m_personnage.setPose(0);
    m_cas=0;

    int nombreArme=0;

    for(unsigned i =  0 ; i < m_weaponMiracle.size() ; ++i)
        for (unsigned j = 0 ; j < m_personnage.m_miracleEnCours.size() ; ++j)
            if(m_weaponMiracle[i] == m_personnage.m_miracleEnCours[j].m_modele)
                m_personnage.m_miracleEnCours.erase(m_personnage.m_miracleEnCours.begin() + j --);

    m_weaponMiracle.clear();

    std::vector <Miracle> miraclesLances;
    for (unsigned i = 0 ; i < m_personnage.m_miracleEnCours.size() ; ++i)
        miraclesLances.push_back(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele]);

    m_classe.miracles.clear();
    m_classe.Charger(m_cheminClasse, m_lvl_miracles_new, m_caracteristiques);

    for (int i=0;i<(int)m_inventaire.size();++i)
    {
        if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size()
         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
        if (m_inventaire[i].Utilisable(m_caracteristiques,m_cheminClasse))
        {
            m_inventaire[i].ChargerMiracle(m_personnage.getCaracteristique());

            if ((m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)||(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER&&m_inventaire[i].m_type==ARME))
                nombreArme++;

            if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
                for (int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if (m_inventaire[i].m_emplacementImpossible[j]==BOUCLIER)
                        m_cas=2;

            if (m_inventaire[i].m_useMiracle)
            {
                m_classe.miracles.push_back(m_inventaire[i].m_miracle);
                m_weaponMiracle.push_back(m_classe.miracles.size()-1);
            }
        }
    }


    if (nombreArme==2)
        m_cas=1;

    int ordre[NOMBRE_MORCEAU_PERSONNAGE];

    int priorite[NOMBRE_MORCEAU_PERSONNAGE];

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        priorite[i] = -1;
        m_pasEquipe[i]=true,m_cheminModeleNouveau[i]="";
        if (tout)
            m_cheminModele[i]="";
    }


    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0
         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
        if (m_inventaire[i].Utilisable(m_caracteristiques,m_cheminClasse))
        {
            if (m_inventaire[i].m_emplacementImageHero.size()>0)
            {
                int temp=m_cas;

                if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME&&m_cas>0)
                    temp=0;

                if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER&&m_inventaire[i].m_type==ARME&&m_cas>0)
                    temp=1;

                if (temp>=(int)m_inventaire[i].m_emplacementImageHero.size())
                    temp=0;


                if (m_inventaire[i].m_emplacementImageHero[temp]>=0&&m_inventaire[i].m_emplacementImageHero[temp]<NOMBRE_MORCEAU_PERSONNAGE)
                {
                    if(m_cheminModeleNouveau[m_inventaire[i].m_emplacementImageHero[temp]].empty() || m_inventaire[i].m_equipe > priorite[m_inventaire[i].m_emplacementImageHero[temp]])
                    {
                        m_cheminModeleNouveau[m_inventaire[i].m_emplacementImageHero[temp]]=m_inventaire[i].m_cheminImageHero[temp];
                        ordre[m_inventaire[i].m_emplacementImageHero[temp]] = temp;
                        priorite[m_inventaire[i].m_emplacementImageHero[temp]] = m_inventaire[i].m_equipe;

                        if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
                            m_weaponModel = m_inventaire[i].m_emplacementImageHero[temp];
                    }
                    m_pasEquipe[m_inventaire[i].m_emplacementImageHero[temp]]=false;
                }
            }
        }

    ChargerGraphics();

    for (unsigned i = 0 ; i < m_personnage.m_miracleEnCours.size() ; ++i)
    {
        m_personnage.m_miracleEnCours[i].m_modele = m_classe.miracles.size();
        m_classe.miracles.push_back(miraclesLances[i]);
        m_classe.miracles.back().RechargerTileset();
    }

    for (unsigned i = 0 ; i < m_weaponMiracle.size() ; ++i)
    {
        m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());
        m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

        m_personnage.m_miracleEnCours.back().m_modele = m_weaponMiracle[i];

        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x;
        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y;
    }

    CalculerOrdreAffichage();
}

void Hero::ChargerGraphics()
{
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        if (m_cheminModeleNouveau[i]!="" && m_cheminModeleNouveau[i]!=m_cheminModele[i])
            m_modelePersonnage[i].Charger(m_classe.chemin_modele+m_cheminModeleNouveau[i]);
        else if (m_cheminModeleNouveau[i]=="")
            m_modelePersonnage[i].Reinitialiser();

        m_cheminModele[i]=m_cheminModeleNouveau[i];
    }

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE ;++i)
    {
        if (m_pasEquipe[i] && !m_classe.modeleNu[i][m_cas].empty())
        {
            if (m_cheminModele[i]!=m_classe.modeleNu[i][m_cas])
            {
                m_modelePersonnage[i].Charger(m_classe.chemin_modele+m_classe.modeleNu[i][m_cas]);
                m_cheminModele[i]=m_classe.modeleNu[i][m_cas];
            }
        }
    }

    CalculerOrdreAffichage();
}

void Hero::CalculerOrdreAffichage()
{
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_ordreAffichage[i]=-1;

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        if (m_cheminModele[i] != "")
        {
            int ordre = m_personnage.getOrdre(&m_modelePersonnage[i]);
            if (ordre!=-10)
                m_ordreAffichage[(int)(NOMBRE_MORCEAU_PERSONNAGE/2+ordre)]=i;
        }
    }
}

void Hero::Afficher()
{
    if(m_personnage.getEtat() == 3)
        m_personnage.setEtat(0,0);

    int plusHaut = -1;
    int plusBas = -1;
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        if(m_ordreAffichage[i] >= 0)
            plusHaut = i;
        if(m_ordreAffichage[i] >= 0 && plusBas == -1)
            plusBas = i;
    }

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        if (m_ordreAffichage[i]!=-1)
        {
            m_personnage.Animer(&m_modelePersonnage[m_ordreAffichage[i]], 0, true);
            m_personnage.m_entite_graphique.Generer();
            m_personnage.Afficher(&m_modelePersonnage[m_ordreAffichage[i]], m_classe.border,
                                  false, i!=plusHaut,i!=plusBas);
        }
}

void Hero::AfficherCaracteristiques(float decalage, bool trader)
{
    sf::Text string;
    string.SetFont(moteurGraphique->m_font);
    string.SetCharacterSize(12);

    string.SetColor(sf::Color(255,255,255));


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.niveau;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(171,322,decalage));
        string.Move(14 - string.GetRect().Width * 0.5, 0);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);

        if(!trader
         &&eventManager->getPositionSouris().x > string.GetRect().Left
         &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
         &&eventManager->getPositionSouris().y > string.GetRect().Top
         &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
            moteurGraphique->AjouterTexte(configuration->getText(0,49),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,
                                          19,0,12,sf::Color(224,224,224));
    }

    {
        string.SetCharacterSize(14);
        string.SetString(m_caracteristiques.nom.c_str());
        string.SetPosition(AutoScreenAdjust(44,320,decalage));
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);

        if(!trader
         &&eventManager->getPositionSouris().x > string.GetRect().Left
         &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
         &&eventManager->getPositionSouris().y > string.GetRect().Top
         &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
            moteurGraphique->AjouterTexte(configuration->getText(0,50),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,
                                          19,0,12,sf::Color(224,224,224));
    }

     {

        string.SetCharacterSize(14);
        string.SetString(configuration->getText(3,m_classe.nom).c_str());
        string.SetPosition(AutoScreenAdjust(44,348,decalage));
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);

        if(!trader
         &&eventManager->getPositionSouris().x > string.GetRect().Left
         &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
         &&eventManager->getPositionSouris().y > string.GetRect().Top
         &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
            moteurGraphique->AjouterTexte(configuration->getText(0,62),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,
                                          19,0,12,sf::Color(224,224,224));
    }

    string.SetString(configuration->getText(0,3));
    string.SetCharacterSize(14);
    string.SetPosition(AutoScreenAdjust(224,319,decalage));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf/*<<configuration->getText(0,3)<<" : "*/<<(int)m_caracteristiques.vie<<" / "<<(int)m_caracteristiques.maxVie;
        string.SetString(buf.str());
    }

    string.SetCharacterSize(14);

    if ((string.GetRect().Width)>94)
    {
        string.SetCharacterSize(13);
        if (string.GetRect().Width>94)
            string.SetCharacterSize(12);
    }

    string.SetPosition(AutoScreenAdjust(267,319,decalage));
    string.Move(47 - string.GetRect().Width * 0.5, 0);

    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
         &&eventManager->getPositionSouris().x > string.GetRect().Left
         &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
         &&eventManager->getPositionSouris().y > string.GetRect().Top
         &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
        moteurGraphique->AjouterTexte(configuration->getText(0,3),coordonnee(   eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,
                                      19,0,12,sf::Color(224,224,224));

    string.SetString(configuration->getText(0,4));
    string.SetCharacterSize(14);
    string.SetPosition(AutoScreenAdjust(224,347,decalage));
    moteurGraphique->AjouterTexte(&string,15);

    string.SetCharacterSize(14);


    {
        std::ostringstream  buf;
        buf/*<<configuration->getText(0,4)<<" : "*/<<(int)m_caracteristiques.foi<<" / "<<(int)m_caracteristiques.maxFoi;
        string.SetString(buf.str());
    }

    if (string.GetRect().Width > 94)
    {
        string.SetCharacterSize(13);
        if (string.GetRect().Width > 94)
        string.SetCharacterSize(12);
    }

    string.SetPosition(AutoScreenAdjust(267,347,decalage));
    string.Move(47 - string.GetRect().Width * 0.5, 0);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
         &&eventManager->getPositionSouris().x > string.GetRect().Left
         &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
         &&eventManager->getPositionSouris().y > string.GetRect().Top
         &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
        moteurGraphique->AjouterTexte(configuration->getText(0,4),coordonnee(   eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224));

    string.SetCharacterSize(14);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.force;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(132,376,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_personnage.getCaracteristique().pts_restant>0)
    {
        m_classe.plus_button[0].position.y = (int)(373-decalage);
        sf::Sprite sprite = m_classe.plus_button[0].Afficher(0);
        moteurGraphique->AjouterCommande(&sprite,15,0);

        if(m_classe.plus_button[0].m_action)
        {
            m_classe.plus_button[0].m_action = false;

            Caracteristique temp = m_personnage.getCaracteristique();

            if(eventManager->getEvenement(sf::Keyboard::LControl, EventKey))
            {
                temp.pts_restant-=5;
                temp.force+=5;
                if(temp.pts_restant < 0)
                    temp.force+=temp.pts_restant, temp.pts_restant = 0;
            }
            else
            {
                temp.pts_restant--;
                temp.force++;
            }

            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }

    string.SetString(configuration->getText(0,5).c_str());

    string.SetX(46 + (configuration->Resolution.x - 800) * 0.5);
    string.SetY(378 + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    string.SetPosition(AutoScreenAdjust(46,376,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
     &&eventManager->getPositionSouris().x > string.GetRect().Left
     &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
     &&eventManager->getPositionSouris().y > string.GetRect().Top
     &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height) {
        moteurGraphique->AjouterTexte(configuration->getText(0,90),coordonnee(  eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224)); }


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.dexterite;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(132,404,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
        /*if (m_caracteristiques.dexterite!=m_personnage.getCaracteristique().dexterite)
            string.SetColor(sf::Color(0,128,255));
        else*/
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_personnage.getCaracteristique().pts_restant>0)
    {
        m_classe.plus_button[1].position.y = (int)(401-decalage);
        sf::Sprite sprite = m_classe.plus_button[1].Afficher(0);
        moteurGraphique->AjouterCommande(&sprite,15,0);

        if(m_classe.plus_button[1].m_action)
        {
            m_classe.plus_button[1].m_action = false;

            Caracteristique temp = m_personnage.getCaracteristique();

            if(eventManager->getEvenement(sf::Keyboard::LControl, EventKey))
            {
                temp.pts_restant-=5;
                temp.dexterite+=5;
                if(temp.pts_restant < 0)
                    temp.dexterite+=temp.pts_restant, temp.pts_restant = 0;
            }
            else
            {
                temp.pts_restant--;
                temp.dexterite++;
            }

            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }

    string.SetString(configuration->getText(0,6).c_str());
   // string.SetX(46 + (configuration->Resolution.x - 800) * 0.5);
    //string.SetY(404 + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    string.SetPosition(AutoScreenAdjust(46,404,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
     &&eventManager->getPositionSouris().x > string.GetRect().Left
     &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
     &&eventManager->getPositionSouris().y > string.GetRect().Top
     &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height) {
        moteurGraphique->AjouterTexte(configuration->getText(0,91),coordonnee(  eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224)); }


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.vitalite;
        string.SetString(buf.str());
        //string.SetX((132 + (configuration->Resolution.x - 800) * 0.5)+17-string.GetRect().Width/2);
       // string.SetY(429 + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
        string.SetPosition(AutoScreenAdjust(132,432,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
        /*if (m_caracteristiques.vitalite!=m_personnage.getCaracteristique().vitalite)
            string.SetColor(sf::Color(0,128,255));
        else*/
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_personnage.getCaracteristique().pts_restant>0)
    {
        m_classe.plus_button[2].position.y = (int)(429-decalage);
        sf::Sprite sprite = m_classe.plus_button[2].Afficher(0);
        moteurGraphique->AjouterCommande(&sprite,15,0);

        if(m_classe.plus_button[2].m_action)
        {
            m_classe.plus_button[2].m_action = false;

            Caracteristique temp = m_personnage.getCaracteristique();

            if(eventManager->getEvenement(sf::Keyboard::LControl, EventKey))
            {
                temp.pts_restant-=5;
                temp.vitalite+=5;
                if(temp.pts_restant < 0)
                    temp.vitalite+=temp.pts_restant, temp.pts_restant = 0;
            }
            else
            {
                temp.pts_restant--;
                temp.vitalite++;
            }

            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }

    string.SetString(configuration->getText(0,7).c_str());
  //  string.SetX(46 + (configuration->Resolution.x - 800) * 0.5);
  //  string.SetY(434 + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    string.SetPosition(AutoScreenAdjust(46,432,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
     &&eventManager->getPositionSouris().x > string.GetRect().Left
     &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
     &&eventManager->getPositionSouris().y > string.GetRect().Top
     &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height) {
        moteurGraphique->AjouterTexte(configuration->getText(0,92),coordonnee(  eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224)); }


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.piete;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(132,460,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
        /*if (m_caracteristiques.piete!=m_personnage.getCaracteristique().piete)
            string.SetColor(sf::Color(0,128,255));
        else*/
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_personnage.getCaracteristique().pts_restant>0)
    {
        m_classe.plus_button[3].position.y = (int)(457-decalage);
        sf::Sprite sprite = m_classe.plus_button[3].Afficher(0);
        moteurGraphique->AjouterCommande(&sprite,15,0);

        if(m_classe.plus_button[3].m_action)
        {
            m_classe.plus_button[3].m_action = false;

            Caracteristique temp = m_personnage.getCaracteristique();

            if(eventManager->getEvenement(sf::Keyboard::LControl, EventKey))
            {
                temp.pts_restant-=5;
                temp.piete+=5;
                if(temp.pts_restant < 0)
                    temp.piete+=temp.pts_restant, temp.pts_restant = 0;
            }
            else
            {
                temp.pts_restant--;
                temp.piete++;
            }

            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }

    string.SetString(configuration->getText(0,8).c_str());
    string.SetPosition(AutoScreenAdjust(46,460,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
     &&eventManager->getPositionSouris().x > string.GetRect().Left
     &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
     &&eventManager->getPositionSouris().y > string.GetRect().Top
     &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height) {
        moteurGraphique->AjouterTexte(configuration->getText(0,93),coordonnee(  eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224)); }


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.charisme;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(132,488,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
        /*if (m_caracteristiques.charisme!=m_personnage.getCaracteristique().charisme)
            string.SetColor(sf::Color(0,128,255));
        else*/
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_personnage.getCaracteristique().pts_restant>0)
    {
        m_classe.plus_button[4].position.y = (int)(485-decalage);
        sf::Sprite sprite = m_classe.plus_button[4].Afficher(0);
        moteurGraphique->AjouterCommande(&sprite,15,0);

        if(m_classe.plus_button[4].m_action)
        {
            m_classe.plus_button[4].m_action = false;

            Caracteristique temp = m_personnage.getCaracteristique();

            if(eventManager->getEvenement(sf::Keyboard::LControl, EventKey))
            {
                temp.pts_restant-=5;
                temp.charisme+=5;
                if(temp.pts_restant < 0)
                    temp.charisme+=temp.pts_restant, temp.pts_restant = 0;
            }
            else
            {
                temp.pts_restant--;
                temp.charisme++;
            }

            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }

    string.SetString(configuration->getText(0,9).c_str());
    string.SetX(46 + (configuration->Resolution.x - 800) * 0.5);
    string.SetY(490 + (configuration->Resolution.y - 600) -decalage*configuration->Resolution.h/600);
    string.SetPosition(AutoScreenAdjust(46,488,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    if(!trader
     &&eventManager->getPositionSouris().x > string.GetRect().Left
     &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
     &&eventManager->getPositionSouris().y > string.GetRect().Top
     &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height) {
        moteurGraphique->AjouterTexte(configuration->getText(0,94),coordonnee(  eventManager->getPositionSouris().x,
                                                                                eventManager->getPositionSouris().y - 20),
                                      m_classe.border,19,0,12,sf::Color(224,224,224)); }

    string.SetString(configuration->getText(0,12).c_str());
    string.SetPosition(AutoScreenAdjust(46,516,decalage));
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.pts_restant;
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(132,516,decalage));
        string.Move(17 - string.GetRect().Width * 0.5, 0);
       // string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }


    string.SetCharacterSize(12);
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.degatsMin[PHYSIQUE]<<" - "<<m_caracteristiques.degatsMax[PHYSIQUE];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(208,405,decalage));
        string.Move(32 - string.GetRect().Width * 0.5, 0);
        //string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.degatsMin[FEU]<<" - "<<m_caracteristiques.degatsMax[FEU];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(208,433,decalage));
        string.Move(32 - string.GetRect().Width * 0.5, 0);
       // string.SetColor(sf::Color(255,128,128));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.degatsMin[FOI]<<" - "<<m_caracteristiques.degatsMax[FOI];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(208,461,decalage));
        string.Move(32 - string.GetRect().Width * 0.5, 0);
       // string.SetColor(sf::Color(128,128,255));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.degatsMin[CORROSION]<<" - "<<m_caracteristiques.degatsMax[CORROSION];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(208,489,decalage));
        string.Move(32 - string.GetRect().Width * 0.5, 0);
        //string.SetColor(sf::Color(128,255,128));
        moteurGraphique->AjouterTexte(&string,15);
    }

    string.SetString(configuration->getText(0,10).c_str());
    string.SetPosition(AutoScreenAdjust(208,376,decalage));
    string.Move(32 - string.GetRect().Width * 0.5, 0);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.armure[PHYSIQUE];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(307,405,decalage));
        string.Move(28 - string.GetRect().Width * 0.5, 0);
      //  string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.armure[FEU];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(307,433,decalage));
        string.Move(28 - string.GetRect().Width * 0.5, 0);
        //string.SetColor(sf::Color(255,128,128));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.armure[FOI];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(307,461,decalage));
        string.Move(28 - string.GetRect().Width * 0.5, 0);
       // string.SetColor(sf::Color(128,128,255));
        moteurGraphique->AjouterTexte(&string,15);
    }
    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.armure[CORROSION];
        string.SetString(buf.str());
        string.SetPosition(AutoScreenAdjust(307,489,decalage));
        string.Move(28 - string.GetRect().Width * 0.5, 0);
        //string.SetColor(sf::Color(128,255,128));
        moteurGraphique->AjouterTexte(&string,15);
    }

    string.SetString(configuration->getText(0,11).c_str());
    string.SetPosition(AutoScreenAdjust(307,376,decalage));
    string.Move(28 - string.GetRect().Width * 0.5, 0);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);


    if(!trader
     &&eventManager->getPositionSouris().x > AutoScreenAdjust(207,0,decalage).x
     &&eventManager->getPositionSouris().x < AutoScreenAdjust(361,0,decalage).x )
    {
        if(eventManager->getPositionSouris().y > AutoScreenAdjust(0,404,decalage).y
         &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,422,decalage).y)
            moteurGraphique->AjouterTexte(configuration->getText(0,44),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,14,sf::Color(224,224,224));
        if(eventManager->getPositionSouris().y > AutoScreenAdjust(0,432,decalage).y
         &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,450,decalage).y)
            moteurGraphique->AjouterTexte(configuration->getText(0,45),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,14,sf::Color(224,224,224));
        if(eventManager->getPositionSouris().y > AutoScreenAdjust(0,460,decalage).y
         &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,478,decalage).y)
            moteurGraphique->AjouterTexte(configuration->getText(0,46),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,14,sf::Color(224,224,224));
        if(eventManager->getPositionSouris().y > AutoScreenAdjust(0,488,decalage).y
         &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,506,decalage).y)
            moteurGraphique->AjouterTexte(configuration->getText(0,47),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,14,sf::Color(224,224,224));
    }

    {
        std::ostringstream  buf;
        buf<<m_argent;
        string.SetString(buf.str());
        string.SetCharacterSize(12);
        string.SetPosition(AutoScreenAdjust(732,508,decalage));
        string.Move(-string.GetRect().Width, 0);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);

        if(    eventManager->getPositionSouris().x > string.GetRect().Left
             &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
             &&eventManager->getPositionSouris().y > string.GetRect().Top
             &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
                moteurGraphique->AjouterTexte(configuration->getText(0,48),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,14,sf::Color(224,224,224));
    }

    {
        std::ostringstream  buf;
        buf<<m_holywater;
        string.SetString(buf.str());
        string.SetCharacterSize(12);
        string.SetPosition(AutoScreenAdjust(452,508,decalage));
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);

        if(    eventManager->getPositionSouris().x > string.GetRect().Left
             &&eventManager->getPositionSouris().x < string.GetRect().Left + string.GetRect().Width
             &&eventManager->getPositionSouris().y > string.GetRect().Top
             &&eventManager->getPositionSouris().y < string.GetRect().Top + string.GetRect().Height)
                moteurGraphique->AjouterTexte(configuration->getText(0,34),coordonnee(  eventManager->getPositionSouris().x,
                                                                                    eventManager->getPositionSouris().y - 20),
                                          m_classe.border,19,0,12,sf::Color(224,224,224));
    }




}

void Hero::AfficherAmisEtCraft()
{

    AfficherRaccourcis();

    bool hover_friend = false;

    for(unsigned i = 0 ; i < m_amis.size() ; ++i)
    {
        std::ostringstream buf;
        if(m_amis[i]->m_heroic)
            buf<<"("<<m_amis[i]->getCaracteristique().niveau<<") ";
        buf<<m_amis[i]->getCaracteristique().nom;
        moteurGraphique->AjouterTexte(buf.str(), coordonnee(16, 64 + i * 26), 14, 0, 14);

        sf::Sprite temp;
        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre_vide.image));
        //temp.SetColor(sf::Color(32,32,32));
        temp.SetPosition(12,82 + i * 26);
        temp.Resize(144,16);
        moteurGraphique->AjouterCommande(&temp, 14, 0);

        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre.image));

        temp.SetSubRect(sf::IntRect(0,0,(int)(m_amis[i]->getCaracteristique().vie*m_classe.barre_vie_monstre.position.w/m_amis[i]->getCaracteristique().maxVie),
                                         m_classe.barre_vie_monstre.position.h));

        temp.Resize(m_amis[i]->getCaracteristique().vie/m_amis[i]->getCaracteristique().maxVie * 144,16);
        moteurGraphique->AjouterCommande(&temp, 14, 0);

      //  temp.SetColor(sf::Color(164,32,32));

        if(!m_amis[i]->EnVie())
        {
            m_amis.erase(m_amis.begin() + i);
            i--;
        }
        else if(eventManager->getPositionSouris().x > temp.GetPosition().x
            && eventManager->getPositionSouris().x < temp.GetPosition().x + 144
            && eventManager->getPositionSouris().y > temp.GetPosition().y
            && eventManager->getPositionSouris().y < temp.GetPosition().y + 16)
            {
                hover_friend = true;
                temp.SetColor(sf::Color(64,32,32));

                if(!eventManager->getEvenement(Mouse::Left,EventClicA))
                    m_select_friend = i;

                if(eventManager->getEvenement(Mouse::Left,EventClicA) && m_select_friend == (int)i)
                if(!m_amis[i]->m_heroic)
                {
                    bool charme = false;
                    eventManager->StopEvenement(Mouse::Left,EventClic);
                    eventManager->StopEvenement(Mouse::Left,EventClicA);
                    for(unsigned j = 0 ; j < m_personnage.m_miracleEnCours.size() ; ++j)
                        for(unsigned k = 0 ; k < m_personnage.m_miracleEnCours[j].m_infos.size() ; ++k)
                        {
                            if(m_classe.miracles[m_personnage.m_miracleEnCours[j].m_modele].m_effets[m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == CHARME)
                                if(m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible == m_amis[i])
                                   m_amis[i]->m_friendly = false, charme = true;
                            if(m_classe.miracles[m_personnage.m_miracleEnCours[j].m_modele].m_effets[m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == INVOCATION)
                                if(m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible == m_amis[i])
                                   m_amis[i]->m_friendly = false, charme = false;
                        }

                    if(!charme)
                        m_amis[i]->Kill();

                    m_amis.erase(m_amis.begin() + i);
                    i--;
                }
            }

        moteurGraphique->AjouterCommande(&temp, 14, 0);
    }

    if(eventManager->getEvenement(Mouse::Left,EventClicA) && !hover_friend)
        m_select_friend = -1;


    if(m_no_result_craft >= 0)
    {
        moteurGraphique->AjouterTexte(m_inventaire[m_no_result_craft].getNom(),
                                      coordonnee(configuration->Resolution.x - 144, 64), 14, 0, 14);

        sf::Sprite temp;
        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre_vide.image));
        temp.SetPosition(configuration->Resolution.x - 12 - 144,82);
        temp.Resize(144,16);
        moteurGraphique->AjouterCommande(&temp, 14, 0);

        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre.image));
        temp.SetSubRect(sf::IntRect(0,0,(int)((m_craft_time_max - m_craft_time)* m_classe.barre_vie_monstre.position.w / m_craft_time_max) ,
                                         m_classe.barre_vie_monstre.position.h));
        temp.Resize((m_craft_time_max - m_craft_time) / m_craft_time_max * 144,16);

        if(m_craft_time > 0)
            temp.SetColor(sf::Color(255,255,255,128));

        moteurGraphique->AjouterCommande(&temp, 14, 0);

        sf::Text text;
        std::ostringstream buf;

        buf<<(float)((int)((m_craft_time_max - m_craft_time) * 100 / m_craft_time_max))<<" %";
        text.SetCharacterSize(10);
        text.SetString(buf.str());
        text.SetPosition(configuration->Resolution.x - 150 - text.GetRect().Width,80);
        moteurGraphique->AjouterTexte(&text,14);
    }

    if(m_no_result_bless >= 0)
    {
        moteurGraphique->AjouterTexte(m_inventaire[m_no_result_bless].getNom(),
                                      coordonnee(configuration->Resolution.x - 144, 64 + 32), 14, 0, 14);

        sf::Sprite temp;
        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre_vide.image));
        //temp.SetColor(sf::Color(32,32,32));
        temp.SetPosition(configuration->Resolution.x - 12 - 144,82 + 32);
        temp.Resize(144,16);
        moteurGraphique->AjouterCommande(&temp, 14, 0);

        temp.SetImage(*moteurGraphique->getImage(m_classe.barre_vie_monstre.image));
        temp.SetSubRect(sf::IntRect(0,0,(int)((m_bless_time_max - m_bless_time) * m_classe.barre_vie_monstre.position.w / m_bless_time_max),
                                         m_classe.barre_vie_monstre.position.h));
        temp.Resize((m_bless_time_max - m_bless_time) / m_bless_time_max * 144,16);

        if(m_bless_time > 0)
            temp.SetColor(sf::Color(255,255,255,128));

        moteurGraphique->AjouterCommande(&temp, 14, 0);

        sf::Text text;
        std::ostringstream buf;

        buf<<(float)((int)((m_bless_time_max - m_bless_time) * 100 / m_bless_time_max))<<" %";
        text.SetCharacterSize(10);
        text.SetString(buf.str());
        text.SetPosition(configuration->Resolution.x - 150 - text.GetRect().Width,80 + 32);
        moteurGraphique->AjouterTexte(&text,14);
    }
}

void Hero::AfficherQuetes(float decalage)
{
    newQuest = false;
    m_quetePointee = -1;
    coordonnee position = m_classe.position_contenu_quetes;

    bool actif = true;

    for (int i = (int)m_quetes.size()-1;i >= 0;--i)
    {
        if(m_quetes[i].m_nom != " " && m_quetes[i].m_actif == actif)
        {
            sf::Text texte;
            texte.SetFont(moteurGraphique->m_font);
            texte.SetCharacterSize(14);
            texte.SetPosition(AutoScreenAdjust(position.x + 2,position.y,decalage));

            if(!actif)
                texte.SetColor(sf::Color(128,128,128));

            texte.SetString(m_quetes[i].m_nom);

            moteurGraphique->AjouterTexte(&texte,15);

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_contenu_quetes.x,0).x
             &&eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_contenu_quetes.x + m_classe.position_contenu_quetes.w,0).x
             &&eventManager->getPositionSouris().y > texte.GetRect().Top
             &&eventManager->getPositionSouris().y < texte.GetRect().Top + 19)
            {
                sf::Sprite sprite;
                sprite.SetImage(*moteurGraphique->getImage(0));
                sprite.Resize(m_classe.position_contenu_quetes.w, 19);
                sprite.SetPosition(AutoScreenAdjust(position.x,position.y,decalage));
                sprite.SetColor(sf::Color(255, 255, 255, 128));

                m_quetePointee = i;

                moteurGraphique->AjouterCommande(&sprite,15,0);
            }

            position.y += 19;
        }

        if(i == 0 && actif)
            i = m_quetes.size(), actif = false;
    }

    int queteAffichee = 0;
    if(m_queteSelectionnee >= 0)
        queteAffichee = m_queteSelectionnee;

    if (queteAffichee >= 0 && queteAffichee < (int)m_quetes.size())
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetCharacterSize(16);
        texte.SetPosition(AutoScreenAdjust(m_classe.position_contenu_description_quete.x + 2,
                                           m_classe.position_contenu_description_quete.y,decalage));
        texte.SetString(m_quetes[queteAffichee].m_nom);

        moteurGraphique->AjouterTexte(&texte,15,0);

        texte.SetCharacterSize(14);
        texte.SetPosition(AutoScreenAdjust(m_classe.position_contenu_description_quete.x + 2,
                                           m_classe.position_contenu_description_quete.y + 32,decalage));
        texte.SetString(m_quetes[queteAffichee].m_description);

        moteurGraphique->AjouterTexte(&texte,15,0);
    }
}


void Hero::AfficherDocs(float decalage)
{
    newDoc = false;
    m_docPointe = -1;
    coordonnee position = m_classe.position_list_docs;
    for (int i = (int)m_docs.size()-1;i >= 0;--i)
    if(m_docs[i].m_nom != " ")
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetCharacterSize(14);
        texte.SetPosition(AutoScreenAdjust(position.x + 2,position.y,decalage));

        texte.SetString(m_docs[i].m_nom);

        moteurGraphique->AjouterTexte(&texte,15);

        if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_list_docs.x,0).x
         &&eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_list_docs.x + m_classe.position_list_docs.w,0).x
         &&eventManager->getPositionSouris().y > texte.GetRect().Top
         &&eventManager->getPositionSouris().y < texte.GetRect().Top + 19)
        {
            sf::Sprite sprite;
            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.Resize(m_classe.position_list_docs.w, 19);
            sprite.SetPosition(AutoScreenAdjust(position.x,position.y,decalage));
            sprite.SetColor(sf::Color(255, 255, 255, 128));

            m_docPointe = i;

            moteurGraphique->AjouterCommande(&sprite,15,0);
        }

        position.y += 19;
    }

    int docAffiche = 0;
    if(m_docSelectionne >= 0)
        docAffiche = m_docSelectionne;

    if (docAffiche >= 0 && docAffiche < (int)m_docs.size())
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetCharacterSize(16);
        texte.SetPosition(AutoScreenAdjust(m_classe.position_contenu_docs.x + 2,
                                           m_classe.position_contenu_docs.y,decalage));

        texte.SetString(m_docs[docAffiche].m_nom);

        moteurGraphique->AjouterTexte(&texte,15,0);

        texte.SetCharacterSize(14);
        texte.SetPosition(AutoScreenAdjust(m_classe.position_contenu_docs.x + 2,
                                           m_classe.position_contenu_docs.y + 24,decalage));

        m_max_defil_cdoc = m_docs[docAffiche].m_ldescription.size();

        for(unsigned i = m_defil_cdoc ;
            i < m_docs[docAffiche].m_ldescription.size() &&
            i < (unsigned)(m_defil_cdoc + m_classe.position_contenu_docs.h);
            ++i)
        {
            texte.SetString(m_docs[docAffiche].m_ldescription[i]);
            texte.Move(0,19);

            moteurGraphique->AjouterTexte(&texte,15,0);
        }
    }

    if(m_max_defil_cdoc-m_classe.position_contenu_docs.h > 0)
    {
        sf::Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(m_classe.scroll_button_cdocs.image));
        sprite.SetPosition(AutoScreenAdjust(m_classe.scroll_button_cdocs.position.x,
                                           (m_classe.scroll_button_cdocs.position.y+
                                            ((19*m_classe.position_contenu_docs.h + m_classe.scroll_button_cdocs.position.h)
                                            *m_defil_cdoc/(m_max_defil_cdoc-m_classe.position_contenu_docs.h))),decalage));
        sprite.Resize(m_classe.scroll_button_cdocs.position.w, m_classe.scroll_button_cdocs.position.h);

        moteurGraphique->AjouterCommande(&sprite,18,0);
    }
}

void Hero::AfficherPotales(float decalage, std::string chemin)
{
    coordonnee position = m_classe.position_contenu_description_quete;
    m_potale_selectionnee = -1;

    for (int i = 0;i < (int)m_potales.size();++i)
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetCharacterSize(14);
        texte.SetPosition(AutoScreenAdjust(position.x + 2,
                                           position.y,decalage));
        texte.SetString(configuration->getText(4,m_potales[i].nom));


        if(fabs(m_potales[i].position.x - m_personnage.getCoordonnee().x) < 5
        && fabs(m_potales[i].position.y - m_personnage.getCoordonnee().y) < 5
        && m_potales[i].chemin == chemin)
            texte.SetColor(sf::Color(128,128,128));
        else
        if(eventManager->getPositionSouris().x > m_classe.position_contenu_description_quete.x + (configuration->Resolution.x - 800) * 0.5
        && eventManager->getPositionSouris().x < m_classe.position_contenu_description_quete.x + m_classe.position_contenu_description_quete.w  + (configuration->Resolution.x - 800) * 0.5
        && eventManager->getPositionSouris().y > texte.GetRect().Top
        && eventManager->getPositionSouris().y < texte.GetRect().Top + texte.GetRect().Height)
        {
            sf::Sprite sprite;
            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.Resize(m_classe.position_contenu_description_quete.w, texte.GetRect().Height+2);
            sprite.SetPosition(AutoScreenAdjust(position.x,
                                                position.y,decalage));
            sprite.SetColor(sf::Color(255, 255, 255, 128));

            if(eventManager->getEvenement(sf::Mouse::Left, EventClic))
            {
                eventManager->StopEvenement(sf::Mouse::Left, EventClic);
                m_potale_selectionnee = i;
            }

            moteurGraphique->AjouterCommande(&sprite,15,0);
        }

        moteurGraphique->AjouterTexte(&texte,15);

        position.y += 19;
    }
}

bool Hero::AfficherMiracles(float decalage, int fenetreEnCours)
{
    bool retour = false;
    sf::Text texte;
    texte.SetCharacterSize(14);

    std::ostringstream buf;
    buf<<m_personnage.getCaracteristique().miracles_restant<<endl;

    texte.SetString(buf.str());
    texte.SetPosition(AutoScreenAdjust(m_classe.position_points_miracles.x,
                                       m_classe.position_points_miracles.y,decalage));
    texte.Move((m_classe.position_points_miracles.w - texte.GetRect().Width) * 0.5,0);

    moteurGraphique->AjouterTexte(&texte,15,0);

    if (eventManager->getPositionSouris().x > m_classe.position_points_miracles.x + (configuration->Resolution.x - 800) * 0.5
     && eventManager->getPositionSouris().x < m_classe.position_points_miracles.x + m_classe.position_points_miracles.w + (configuration->Resolution.x - 800) * 0.5
     && eventManager->getPositionSouris().y > m_classe.position_points_miracles.y + (configuration->Resolution.y - 600)
     && eventManager->getPositionSouris().y < m_classe.position_points_miracles.y + m_classe.position_points_miracles.h + (configuration->Resolution.y - 600))
        moteurGraphique->AjouterTexte(configuration->getText(0,12), coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),m_classe.border,20,false,12,sf::Color(224,224,224));

    texte.SetCharacterSize(12);

    for (int i = 0;i < (int)m_classe.position_miracles.size(); ++i)
        if (m_classe.page_miracles[i] == fenetreEnCours)
        {
            std::ostringstream buf2;
            buf2<<m_lvl_miracles_new[i]<<endl;
            texte.SetString(buf2.str());
            texte.SetPosition(AutoScreenAdjust(m_classe.position_miracles[i].x,
                                               m_classe.position_miracles[i].y + 39,decalage));
            texte.Move((m_classe.position_miracles[i].w - texte.GetRect().Width) * 0.5,0);

            moteurGraphique->AjouterTexte(&texte,15,0);
        }

    for (int i = 0;i < (int)m_classe.boutons_miracles.size(); ++i)
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_classe.boutons_miracles[i].image.image));
        sprite.SetSubRect(sf::IntRect(m_classe.boutons_miracles[i].image.position.x,
                                      m_classe.boutons_miracles[i].image.position.y,
                                      m_classe.boutons_miracles[i].image.position.w,
                                      m_classe.boutons_miracles[i].image.position.h));

        sprite.SetPosition(AutoScreenAdjust(m_classe.boutons_miracles[i].position.x,
                                            m_classe.boutons_miracles[i].position.y,decalage));

        sprite.Resize(m_classe.boutons_miracles[i].position.w, m_classe.boutons_miracles[i].position.h);

        if (i != fenetreEnCours)
        {
            sprite.SetColor(sf::Color(128,128,128));
            if((int)m_classe.onglets_miracles.size() > i)
            {
                sf::Sprite sprite2;

                sprite2.SetImage(*moteurGraphique->getImage(m_classe.onglets_miracles[i].image.image));
                sprite2.SetSubRect(sf::IntRect(m_classe.onglets_miracles[i].image.position.x,
                                              m_classe.onglets_miracles[i].image.position.y,
                                              m_classe.onglets_miracles[i].image.position.w,
                                              m_classe.onglets_miracles[i].image.position.h));

                sprite2.SetPosition(AutoScreenAdjust(m_classe.onglets_miracles[i].position.x,
                                                    m_classe.onglets_miracles[i].position.y,decalage));

                sprite2.Resize(m_classe.onglets_miracles[i].position.w, m_classe.onglets_miracles[i].position.h);
                moteurGraphique->AjouterCommande(&sprite2, 16,0);
            }
        }

        moteurGraphique->AjouterCommande(&sprite, 16,0);

        sprite.Move(3,3);
        sprite.SetColor(sf::Color(0,0,0));
        moteurGraphique->AjouterCommande(&sprite, 15,0);
    }

    for (int i = 0;i < (int)m_lvl_miracles_new.size(); ++i)
        if (m_classe.page_miracles[i] == fenetreEnCours)
        {
            sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(m_classe.miracles_cadre.image));
            sprite.SetPosition(AutoScreenAdjust(m_classe.position_miracles[i].x + m_classe.miracles_cadre.position.x,
                                               m_classe.position_miracles[i].y + m_classe.miracles_cadre.position.y,decalage));
            moteurGraphique->AjouterCommande(&sprite, 15,0);

            sprite.SetImage(*moteurGraphique->getImage(m_classe.icone_miracles[i].image));
            sprite.SetSubRect(sf::IntRect(m_classe.icone_miracles[i].position.x,
                                          m_classe.icone_miracles[i].position.y,
                                          m_classe.icone_miracles[i].position.w,
                                          m_classe.icone_miracles[i].position.h));

            sprite.SetPosition(AutoScreenAdjust(m_classe.position_miracles[i].x,
                                                m_classe.position_miracles[i].y - 2,decalage));

            sprite.Resize(m_classe.position_miracles[i].w, m_classe.position_miracles[i].h);
            moteurGraphique->AjouterCommande(&sprite, 16,0);

            sprite.Move(3,3);
            sprite.SetColor(sf::Color(0,0,0));
            moteurGraphique->AjouterCommande(&sprite, 15,0);

           // bool plus_button_survol = false;
            bool add_point = false;

            if (m_personnage.getCaracteristique().miracles_restant > 0
            && (int)((m_personnage.getCaracteristique().niveau-1)/3) >= m_lvl_miracles[i])
            {
                if (m_classe.miracles[i].m_buf == -1 || m_lvl_miracles[m_classe.miracles[i].m_buf] > 0)
                {
                    sf::Sprite sprite2 = m_classe.miracles_plus_button[i].Afficher(decalage);

                    moteurGraphique->AjouterCommande(&sprite2, 17,0);
                }
                else
                    m_classe.miracles_plus_button[i].m_hover = false;
            }
            else
                m_classe.miracles_plus_button[i].m_hover = false;


            if (!m_classe.miracles_plus_button[i].m_hover
                &&eventManager->getPositionSouris().x > m_classe.position_miracles[i].x + m_classe.miracles_cadre.position.x + (configuration->Resolution.x - 800) * 0.5
                &&eventManager->getPositionSouris().x < m_classe.position_miracles[i].x + m_classe.miracles_cadre.position.w + m_classe.miracles_cadre.position.x + (configuration->Resolution.x - 800) * 0.5
                &&eventManager->getPositionSouris().y > m_classe.position_miracles[i].y + m_classe.miracles_cadre.position.y + (configuration->Resolution.y - 600)
                &&eventManager->getPositionSouris().y < m_classe.position_miracles[i].y + m_classe.miracles_cadre.position.h + m_classe.miracles_cadre.position.y + (configuration->Resolution.y - 600))
            {
                m_classe.miracles[i].AfficherDescription(coordonnee((m_classe.position_miracles[i].x + m_classe.position_miracles[i].w)*configuration->Resolution.w/800,
                                                                     m_classe.position_miracles[i].y*configuration->Resolution.h/600),
                                                         m_classe.border);

                retour = true;

                if (eventManager->getEvenement(Mouse::Left,EventClic))
                    if((m_lvl_miracles_new[i] > 0 && m_classe.miracles[i].m_buf == -1) ||
                        m_lvl_miracles[i] > 0)
                    {
                        if (m_classe.miracles[i].m_buf != -1)
                            m_miracleEnMain = m_classe.miracles[i].m_buf;
                        else
                            m_miracleEnMain = i;
                        m_lastRaccourcis = -1;
                        eventManager->StopEvenement(Mouse::Left,EventClic);
                    }

                if (eventManager->getEvenement(Mouse::Right,EventClic) && m_miracleEnMain < 0)
                {
                    add_point = true;

                    eventManager->StopEvenement(Mouse::Right,EventClic);

                    RecalculerCaracteristiques(true);
                    ChargerModele();
                }
            }

            if(m_classe.miracles_plus_button[i].m_action)
                add_point = true, m_classe.miracles_plus_button[i].m_action = false;

            if(add_point)
            {
                //moteurSons->JouerSon(configuration->sound_menu,coordonnee (0,0),0);
                moteurSons->JouerSon(configuration->sound_select_miracle,coordonnee (0,0),0);

                if (m_personnage.getCaracteristique().miracles_restant > 0)
                    if (m_classe.miracles[i].m_buf == -1 || (m_classe.miracles[i].m_buf != -1 && m_lvl_miracles[m_classe.miracles[i].m_buf] > 0))
                    {
                        Caracteristique temp = m_personnage.getCaracteristique();
                        temp.miracles_restant--;
                        m_lvl_miracles[i]++;
                        m_personnage.setCaracteristique(temp);

                        if(m_lvl_miracles[i] == 1 && m_classe.miracles[i].m_buf == -1)
                        {
                            for(int k = 0 ; k < 8 ; ++k)
                                if(m_raccourcis[k].no == -1)
                                    m_raccourcis[k].no = i, m_raccourcis[k].miracle = true, k=8;

                            if(m_miracle_droite[m_weaponsSet] == -1 && !m_classe.miracles[i].m_direct)
                                m_miracle_droite[m_weaponsSet] = i;
                        }

                        RecalculerCaracteristiques(true);
                        ChargerModele();
                    }
            }
        }

    if (eventManager->getEvenement(Mouse::Right,EventClic))
        m_miracleEnMain = -1;

    return (retour);
}

void Hero::GererRaccourcisMiracles()
{
    if(eventManager->getEvenement(Mouse::Left,EventClic))
    {
        for(int i = 0 ; i < 8 ; ++i)
        {
            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x + m_classe.position_raccourcis[i].w
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y + m_classe.position_raccourcis[i].h)
            {
                if (m_miracleEnMain >= 0)
                {
                    moteurSons->JouerSon(configuration->sound_select_miracle,coordonnee (0,0),0);
                    int buf = m_raccourcis[i].no;
                    m_raccourcis[i].no = m_miracleEnMain;
                    if(m_raccourcis[i].miracle )
                        m_miracleEnMain = buf;
                    else
                        m_objetEnMain = buf, m_miracleEnMain = -1;

                    m_lastRaccourcis = i;

                    m_raccourcis[i].miracle = true;
                    eventManager->StopEvenement(Mouse::Left,EventClic);
                }
                else if(m_raccourcis[i].miracle && m_raccourcis[i].no >= 0 && m_objetEnMain == -1)
                    m_miracleEnMain = m_raccourcis[i].no, m_raccourcis[i].no = -1,m_lastRaccourcis = i,
                    eventManager->StopEvenement(Mouse::Left,EventClic);
            }
        }
        if (  eventManager->getPositionSouris().x > m_classe.p_miracleG.position.x + (configuration->Resolution.x - 800) * 0.5
            &&eventManager->getPositionSouris().x < m_classe.p_miracleG.position.x + (configuration->Resolution.x - 800) * 0.5 + m_classe.p_miracleG.position.w
            &&eventManager->getPositionSouris().y > m_classe.p_miracleG.position.y + (configuration->Resolution.y - 600)
            &&eventManager->getPositionSouris().y < m_classe.p_miracleG.position.y + (configuration->Resolution.y - 600) + m_classe.p_miracleG.position.h)
        {
            if (m_miracleEnMain >= 0)
            {
                moteurSons->JouerSon(configuration->sound_select_miracle,coordonnee (0,0),0);
                m_miracle_gauche[m_weaponsSet] = m_miracleEnMain;
                if(m_lastRaccourcis != -1)
                    m_raccourcis[m_lastRaccourcis].no = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracle_gauche[m_weaponsSet], m_miracle_gauche[m_weaponsSet] = -1,m_lastRaccourcis=-1;

            eventManager->StopEvenement(Mouse::Left,EventClic);
        }

        if (  eventManager->getPositionSouris().x > m_classe.p_miracleD.position.x + (configuration->Resolution.x - 800) * 0.5
            &&eventManager->getPositionSouris().x < m_classe.p_miracleD.position.x + (configuration->Resolution.x - 800) * 0.5 + m_classe.p_miracleD.position.w
            &&eventManager->getPositionSouris().y > m_classe.p_miracleD.position.y + (configuration->Resolution.y - 600)
            &&eventManager->getPositionSouris().y < m_classe.p_miracleD.position.y + (configuration->Resolution.y - 600) + m_classe.p_miracleD.position.h)
        {
            if (m_miracleEnMain >= 0)
            {
                moteurSons->JouerSon(configuration->sound_select_miracle,coordonnee (0,0),0);
                m_miracle_droite[m_weaponsSet] = m_miracleEnMain;
                if(m_lastRaccourcis != -1)
                    m_raccourcis[m_lastRaccourcis].no = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracle_droite[m_weaponsSet], m_miracle_droite[m_weaponsSet] = -1,m_lastRaccourcis=-1;

            eventManager->StopEvenement(Mouse::Left,EventClic);
        }
    }

    if (m_miracleEnMain >= 0 && m_miracleEnMain < (int)m_classe.position_miracles.size())
    {
        sf::Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(m_classe.icone_miracles[m_miracleEnMain].image));

        sprite.SetSubRect(IntRect(  m_classe.icone_miracles[m_miracleEnMain].position.x,
                                    m_classe.icone_miracles[m_miracleEnMain].position.y,
                                    m_classe.icone_miracles[m_miracleEnMain].position.w,
                                    m_classe.icone_miracles[m_miracleEnMain].position.h));
        sprite.SetX(eventManager->getPositionSouris().x - 16 );
        sprite.SetY(eventManager->getPositionSouris().y - 16 );

        sprite.Resize(32,32);

        moteurGraphique->AjouterCommande(&sprite,19,0);
    }
}


void Hero::GererRaccourcisObjets(bool inventory, bool hideLeft)
{
    if(eventManager->getEvenement(Mouse::Left,EventClic))
        for(int i = 0 ; i < 8 ; ++i)
        {
            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x + m_classe.position_raccourcis[i].w
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y + m_classe.position_raccourcis[i].h)
            {
                if (m_objetEnMain >= 0)
                {
                    moteurSons->JouerSon(configuration->sound_select_miracle,coordonnee (0,0),0);
                    int buf = m_raccourcis[i].no;
                    m_raccourcis[i].no = m_objetEnMain;

                    if(m_raccourcis[i].miracle)
                        m_miracleEnMain = buf, m_objetEnMain = -1;
                    else
                        m_objetEnMain = buf;

                    m_lastRaccourcis = -1;

                    m_raccourcis[i].miracle = false;
                    eventManager->StopEvenement(Mouse::Left,EventClic);
                }
                else if(!m_raccourcis[i].miracle && m_raccourcis[i].no >= 0 && m_miracleEnMain == -1)
                    m_objetEnMain = m_raccourcis[i].no, m_raccourcis[i].no = -1,m_lastRaccourcis = -1,
                    eventManager->StopEvenement(Mouse::Left,EventClic);

                UpdateRaccourcis();
            }
        }


    if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(0));

        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32,m_inventaire[m_objetEnMain].getTaille().y*32);

        coordonnee caseVisee;
        caseVisee.x=(int)(((eventManager->getPositionSouris().x+8)-AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x)/32 - m_inventaire[m_objetEnMain].getTaille().x/2);
        caseVisee.y=(int)(((eventManager->getPositionSouris().y+8)-AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32,0).y)/32 - m_inventaire[m_objetEnMain].getTaille().y/2);

        sprite.SetX(eventManager->getPositionSouris().x - m_inventaire[m_objetEnMain].getTaille().x*16);
        sprite.SetY(eventManager->getPositionSouris().y - m_inventaire[m_objetEnMain].getTaille().y*16);

        sprite.SetColor(GetItemColor(m_inventaire[m_objetEnMain].getRarete()));
        sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.25),(int)(sprite.GetColor().g*0.25),(int)(sprite.GetColor().b*0.25),128));

        if(inventory)
        {
            int emplacement=-1;
            for (int k=0;k<(int)m_classe.emplacements.size();k++)
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.emplacements[k].position.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.emplacements[k].position.x + m_classe.emplacements[k].position.w,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.emplacements[k].position.y, 0).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.emplacements[k].position.y + m_classe.emplacements[k].position.h, 0).y)
                    emplacement=k;

            if (PossibleEquiper(m_objetEnMain, emplacement) && !hideLeft)
            {
                sf::Sprite emp_background;
                emp_background.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_background));

                emp_background.Resize(m_classe.emplacements[emplacement].position.w,
                                      m_classe.emplacements[emplacement].position.h);

                emp_background.SetX(m_classe.emplacements[emplacement].position.x);
                emp_background.SetY(m_classe.emplacements[emplacement].position.y);

                emp_background.SetColor(GetItemColor(m_inventaire[m_objetEnMain].getRarete()));
                emp_background.SetColor(sf::Color((int)(emp_background.GetColor().r),
                                                  (int)(emp_background.GetColor().g),
                                                  (int)(emp_background.GetColor().b),128));

                moteurGraphique->AjouterCommande(&emp_background,17,false);
            }

            if(eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x
             ||eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x + 32*m_classe.position_contenu_inventaire.w
             ||eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32,0).y
             ||eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32,0).y + 32*m_classe.position_contenu_inventaire.h)
            {
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_sac_inventaire.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_sac_inventaire.x + m_classe.position_sac_inventaire.w,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_sac_inventaire.y, 0).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_sac_inventaire.y + m_classe.position_sac_inventaire.h * 20, 0).y)
                    sprite.SetColor(sf::Color(128,0,0,128));
            }
            else
            {
                sprite.SetX(caseVisee.x*32 + AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x);
                sprite.SetY(caseVisee.y*32 + AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32,0).y);
            }
        }

        moteurGraphique->AjouterCommande(&sprite,18,0);

        sprite.SetColor(sf::Color(255,255,255,255));

        sprite.SetImage(*moteurGraphique->getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x,
                                  m_inventaire[m_objetEnMain].getPositionImage().y,
                                  m_inventaire[m_objetEnMain].getPositionImage().w,
                                  m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32,
                      m_inventaire[m_objetEnMain].getTaille().y*32);

        sprite.SetX(eventManager->getPositionSouris().x - m_inventaire[m_objetEnMain].getTaille().x*16);
        sprite.SetY(eventManager->getPositionSouris().y - m_inventaire[m_objetEnMain].getTaille().y*16);

        moteurGraphique->AjouterCommande(&sprite,19,0);
    }
}

bool Hero::AfficherInventaire(float decalage, std::vector<Objet> *trader, bool hideLeft, bool bless, bool craft)
{
    bool retour = false;
    m_objetVise = -1;

    for(unsigned i = 0 ; i < m_classe.emplacements.size() && !hideLeft ; ++i)
        m_classe.emplacements[i].empty = true;

    Sprite sprite_sort_inv = m_classe.sort_inventory.Afficher(decalage);

    if(m_classe.sort_inventory.m_action)
    {
        m_classe.sort_inventory.m_action = false;
        AutoTrierInventaire();
    }

    for (int i=0;i<(int)m_inventaire.size();++i)
        if (i!=m_objetEnMain && (m_inventaire[i].m_equipe==-1
      || (m_inventaire[i].m_equipe>=0 && !hideLeft &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))))
        if ((i == m_no_result_bless && bless)
         || (i == m_no_result_craft && craft)
         || (i == m_no_schema_bless && bless)
         || (i == m_no_schema_craft && craft)
         || (i != m_no_schema_craft && i != m_no_result_craft
          && i != m_no_schema_bless && i != m_no_result_bless))
        {
            coordonneeDecimal position(0,0,0,0);
            sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_background));

            if(configuration->item_background)
            {
                sprite.SetColor(GetItemColor(m_inventaire[i].getRarete()));
                sprite.SetColor(sf::Color(sprite.GetColor().r,sprite.GetColor().g,sprite.GetColor().b,96));
            }
            else
                sprite.SetColor(sf::Color(128,128,128,128));

            sprite.Resize(m_inventaire[i].getTaille().x*32-1,m_inventaire[i].getTaille().y*32-1);

            if (m_inventaire[i].m_equipe==-1 && i != m_no_schema_craft && i != m_no_result_craft
                                             && i != m_no_schema_bless && i != m_no_result_bless)
            {
                position.x = AutoScreenAdjust(m_inventaire[i].getPosition().x*32 + m_classe.position_contenu_inventaire.x, 0).x;
                position.y = AutoScreenAdjust(0, (m_inventaire[i].getPosition().y-1)*32 + m_classe.position_contenu_inventaire.y, decalage).y;

                sprite.SetPosition(position.x, position.y);

                position.h=m_inventaire[i].getTaille().y*32;
                position.w=m_inventaire[i].getTaille().x*32;

                if(m_objetEnMain==-1
                &&!m_classe.sort_inventory.m_hover
                && eventManager->getPositionSouris().x > sprite.GetPosition().x
                && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
                && eventManager->getPositionSouris().y > sprite.GetPosition().y
                && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
                {
                    m_objetVise = i;
                    coordonnee temp=eventManager->getPositionSouris();
                    temp.y+=32;
                    temp.x+=96;

                    for (int j=0;j<(int)m_inventaire.size();j++)
                        if (m_inventaire[j].m_equipe>=0
                         &&(m_inventaire[j].m_equipe_set == m_weaponsSet || m_inventaire[j].m_equipe_set == -1))
                            for (int k=0;k<(int)m_inventaire[i].m_emplacement.size();k++)
                                for (int l=0;l<(int)m_inventaire[j].m_emplacement.size();l++)
                                    if (m_inventaire[i].m_emplacement[k]==m_inventaire[j].m_emplacement[l])
                                        temp.x+=64;

                    int decalage2 = m_inventaire[i].AfficherCaracteristiques(temp,m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse,1,1,0);
                    retour = true;

                    for(int k = m_classe.emplacements.size() - 1 ; k >= 0 ; k--)
                    {
                        bool ok = false;
                        for (int l=0;l<(int)m_inventaire[i].m_emplacement.size();l++)
                            if(m_inventaire[i].m_emplacement[l] == m_classe.emplacements[k].emplacement)
                                ok = true;

                        if(ok)
                            for (int j=0;j<(int)m_inventaire.size();j++)
                                if(m_inventaire[j].m_equipe == k
                                &&(m_inventaire[j].m_equipe_set == m_weaponsSet || m_inventaire[j].m_equipe_set == -1))
                                    temp.x=decalage2-4,decalage2=m_inventaire[j].AfficherCaracteristiques(temp,m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse,1,1,0,0);
                    }
                }
            }
            else if(i != m_no_schema_craft && i != m_no_result_craft
                 && i != m_no_schema_bless && i != m_no_result_bless)
            {
                if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size()
                 &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
                {
                    m_classe.emplacements[m_inventaire[i].m_equipe].empty = false;

                    sprite.Resize(m_classe.emplacements[m_inventaire[i].m_equipe].position.w,
                                  m_classe.emplacements[m_inventaire[i].m_equipe].position.h);

                    position.x = AutoScreenAdjust(m_classe.emplacements[m_inventaire[i].m_equipe].position.x +
                                                   (m_classe.emplacements[m_inventaire[i].m_equipe].position.w - m_inventaire[i].getPositionImage().w) * 0.5, 0).x;
                    position.y = AutoScreenAdjust(0, m_classe.emplacements[m_inventaire[i].m_equipe].position.y +
                                                   (m_classe.emplacements[m_inventaire[i].m_equipe].position.h - m_inventaire[i].getPositionImage().h)/2, decalage).y;

                    sprite.SetPosition(AutoScreenAdjust(m_classe.emplacements[m_inventaire[i].m_equipe].position.x,
                                                        m_classe.emplacements[m_inventaire[i].m_equipe].position.y, decalage));

                    position.h=m_classe.emplacements[m_inventaire[i].m_equipe].position.h;
                    position.w=m_classe.emplacements[m_inventaire[i].m_equipe].position.w;


                    if(m_objetEnMain==-1
                    && eventManager->getPositionSouris().x > sprite.GetPosition().x
                    && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
                    && eventManager->getPositionSouris().y > sprite.GetPosition().y
                    && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
                        m_inventaire[i].AfficherCaracteristiques(coordonnee ((int)(position.x+ m_classe.emplacements[m_inventaire[i].m_equipe].position.w),
                                                                             (int)(position.y+position.h),0,0),m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse), retour = true;
                }
            }
            else if(i != m_no_result_craft && i != m_no_result_bless
                &&((i == m_no_schema_bless && bless) || (i == m_no_schema_craft && craft)))
            {
                position.x = AutoScreenAdjust(m_classe.schema_craft.position.x, 0).x;
                position.y = AutoScreenAdjust(0, m_classe.schema_craft.position.y, decalage).y;

                position.h = m_inventaire[i].getTaille().y*32;
                position.w = m_inventaire[i].getTaille().x*32;

                sprite.SetPosition(position.x, position.y);

                if(m_objetEnMain==-1
                && eventManager->getPositionSouris().x > sprite.GetPosition().x
                && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
                && eventManager->getPositionSouris().y > sprite.GetPosition().y
                && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
                    m_inventaire[i].AfficherCaracteristiques(coordonnee ((int)(position.x+64),(int)(position.y+position.h),0,0),m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse), retour = true;
            }
            else
            {
                if(bless)
                    sprite.Resize(m_classe.result_craft.position.w,
                                  (float)m_classe.result_craft.position.h * (m_bless_time_max - m_bless_time)/m_bless_time_max+1);
                else
                    sprite.Resize(m_classe.result_craft.position.w,
                                  (float)m_classe.result_craft.position.h * (m_craft_time_max - m_craft_time)/m_craft_time_max+1);

                position.x = AutoScreenAdjust(m_classe.result_craft.position.x + (m_classe.result_craft.position.w - m_inventaire[i].getPositionImage().w) * 0.5, 0).x;
                position.y = AutoScreenAdjust(0, m_classe.result_craft.position.y + (m_classe.result_craft.position.h - m_inventaire[i].getPositionImage().h) * 0.5, decalage).y;

                position.h=m_classe.result_craft.position.h;
                position.w=m_classe.result_craft.position.w;

                sprite.SetX((251)*configuration->Resolution.w/800);
                sprite.SetY((43)*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
                sprite.SetPosition(AutoScreenAdjust(m_classe.result_craft.position.x ,
                                                    m_classe.result_craft.position.y , decalage));

                if(m_objetEnMain==-1
                && eventManager->getPositionSouris().x > sprite.GetPosition().x
                && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
                && eventManager->getPositionSouris().y > sprite.GetPosition().y
                && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
                    m_inventaire[i].AfficherCaracteristiques(coordonnee ((int)(position.x+160),(int)(position.y+position.h),0,0),m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse), retour = true;
            }



        //    m_classe.border.Afficher(coordonnee (sprite.GetPosition().x+4,sprite.GetPosition().y+4),
            //                         coordonnee (sprite.GetSize().x-8,sprite.GetSize().y-8), 16, GetItemColor(m_inventaire[i].getRarete()));




           /* sf::Sprite sprite2;
            if (!m_inventaire[i].Utilisable(m_caracteristiques,m_cheminClasse))
            {
                sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_unusable));
                sprite2.SetColor(sf::Color(255,255,255,255));

                int taille = m_inventaire[i].getTaille().x*32;
                if(m_inventaire[i].getTaille().y < m_inventaire[i].getTaille().x)
                    taille = m_inventaire[i].getTaille().y*32;

                if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
                {
                    if(m_classe.emplacements[m_inventaire[i].m_equipe].position.w > m_classe.emplacements[m_inventaire[i].m_equipe].position.h)
                        taille = m_classe.emplacements[m_inventaire[i].m_equipe].position.h;
                    else
                        taille = m_classe.emplacements[m_inventaire[i].m_equipe].position.w;
                }

                sprite2.Resize(taille, taille);
                sprite2.SetPosition(sprite.GetPosition());
                if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
                {
                    if(m_classe.emplacements[m_inventaire[i].m_equipe].position.w > m_classe.emplacements[m_inventaire[i].m_equipe].position.h)
                        sprite2.Move(m_classe.emplacements[m_inventaire[i].m_equipe].position.w * 0.5 - m_classe.emplacements[m_inventaire[i].m_equipe].position.h * 0.5,0);
                    else
                        sprite2.Move(0,m_classe.emplacements[m_inventaire[i].m_equipe].position.h * 0.5 - m_classe.emplacements[m_inventaire[i].m_equipe].position.w * 0.5);
                }
                else
                {
                    if(m_inventaire[i].getTaille().y < m_inventaire[i].getTaille().x)
                        sprite2.Move(m_inventaire[i].getTaille().x*16 - m_inventaire[i].getTaille().y*16,0);
                    else
                        sprite2.Move(0,m_inventaire[i].getTaille().y*16 - m_inventaire[i].getTaille().x*16);
                }

                for(int X = 0 ; X < position.w ; X+=32)
                for(int Y = 0 ; Y < position.h ; Y+=32)
                {
                    sprite2.Resize(32,32);
                    sprite2.SetPosition(position.x + X,position.y + Y);
                    moteurGraphique->AjouterCommande(&sprite2,16,0);
                }
                sprite2.SetColor(sf::Color(255,255,255,64));
                for(int X = 0 ; X < position.w ; X+=32)
                for(int Y = 0 ; Y < position.h ; Y+=32)
                {
                    sprite2.Resize(32,32);
                    sprite2.SetPosition(position.x + X,position.y + Y);
                    moteurGraphique->AjouterCommande(&sprite2,18,0);
                }
           //     sprite.SetColor(sf::Color(sprite.GetColor().r,
             //                             sprite.GetColor().g,
              //                            sprite.GetColor().b,255));
            }*/
            if (!m_inventaire[i].Utilisable(m_caracteristiques,m_cheminClasse))
            {
                sf::Sprite sprite2 = sprite;
                sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_unusable));
                sprite2.SetColor(sf::Color(96,0,0,128));
                moteurGraphique->AjouterCommande(&sprite2,16,0);
            }

            moteurGraphique->AjouterCommande(&sprite,16,0);




            sprite.SetBlendMode(sf::Blend::Alpha);
            sprite.SetColor(sf::Color(255,255,255,255));


            sprite.SetOrigin(m_inventaire[i].getPositionImage().w/2,m_inventaire[i].getPositionImage().h/2);

            sprite.SetX(position.x+m_inventaire[i].getPositionImage().w/2);
            sprite.SetY(position.y+m_inventaire[i].getPositionImage().h/2);

            sprite.SetImage(*moteurGraphique->getImage(m_inventaire[i].getImage()));
            sprite.SetSubRect(IntRect(  m_inventaire[i].getPositionImage().x,
                                        m_inventaire[i].getPositionImage().y,
                                        m_inventaire[i].getPositionImage().w,
                                        m_inventaire[i].getPositionImage().h));
            sprite.Resize(m_inventaire[i].getTaille().x*32,m_inventaire[i].getTaille().y*32);

            if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER && m_inventaire[i].m_type==ARME)
                sprite.SetRotation(-45);
            else
                sprite.SetRotation(0);

            moteurGraphique->AjouterCommande(&sprite,17,0);


            /*if (!m_inventaire[i].Utilisable(m_caracteristiques,m_cheminClasse))
            {
                sprite.SetColor(sf::Color(255,0,0,255));

                moteurGraphique->AjouterCommande(&sprite,17,0);
            }*/
            //moteurGraphique->AjouterCommande(&sprite2,17,0);

            sprite.SetRotation(0);
        }

    m_max_defilement_trader=0;

  //  cout<<trader<<endl<<&m_coffre<<endl<<endl;

    for (int i=0;i<(int)(*trader).size();++i)
    {
        if ((*trader)[i].getPosition().y+(*trader)[i].getTaille().y>m_max_defilement_trader)
            m_max_defilement_trader=(*trader)[i].getPosition().y+(*trader)[i].getTaille().y;

        if ((*trader)[i].getPosition().y+(*trader)[i].getTaille().y-m_defilement_trader>0
          &&(*trader)[i].getPosition().y-m_defilement_trader<m_classe.position_contenu_marchand.h)
        {
            coordonneeDecimal position;
            sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_background));

            if(configuration->item_background)
            {
                sprite.SetColor(GetItemColor((*trader)[i].getRarete()));
                sprite.SetColor(sf::Color(sprite.GetColor().r,sprite.GetColor().g,sprite.GetColor().b,96));
            }
            else
                sprite.SetColor(sf::Color(128,128,128,128));

            sprite.Resize((*trader)[i].getTaille().x*32-1,(*trader)[i].getTaille().y*32-1);

            position.x = AutoScreenAdjust((*trader)[i].getPosition().x*32 + m_classe.position_contenu_marchand.x, 0).x;
            position.y = AutoScreenAdjust(0, ((*trader)[i].getPosition().y-m_defilement_trader-1)*32 + m_classe.position_contenu_marchand.y, decalage).y;

            sprite.SetPosition(position.x, position.y);

            position.h=(*trader)[i].getTaille().y*32;
            position.w=(*trader)[i].getTaille().x*32;

            if ((*trader)[i].getPosition().y+(*trader)[i].getTaille().y-m_defilement_trader>m_classe.position_contenu_marchand.h)
            {
                sprite.SetSubRect(sf::IntRect(0,0,160,(m_defilement_trader+m_classe.position_contenu_marchand.h-(*trader)[i].getPosition().y)*32));
                sprite.Resize((*trader)[i].getTaille().x*32,(m_defilement_trader+m_classe.position_contenu_marchand.h-(*trader)[i].getPosition().y)*32);
            }
            if ((*trader)[i].getPosition().y-m_defilement_trader<0)
            {
                sprite.SetSubRect(sf::IntRect(0,(-(*trader)[i].getPosition().y+m_defilement_trader)*32,160,160-(-(*trader)[i].getPosition().y+m_defilement_trader)*32));
                sprite.Resize((*trader)[i].getTaille().x*32,(*trader)[i].getTaille().y*32-(-(*trader)[i].getPosition().y+m_defilement_trader)*32);
                sprite.Move(0,(-(*trader)[i].getPosition().y+m_defilement_trader)*32);
            }

            if(m_objetEnMain==-1
            && eventManager->getPositionSouris().x > sprite.GetPosition().x
            && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
            && eventManager->getPositionSouris().y > sprite.GetPosition().y
            && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
            {
                coordonnee temp=eventManager->getPositionSouris();
                //temp.y-=256;
                temp.x-=196;

                int decalage2=(*trader)[i].AfficherCaracteristiques(temp,m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse,(10-(float)m_caracteristiques.charisme/100),1,1,1,trader==&m_coffre);
                retour = true;

                for(int k = 0 ; k < (int)m_classe.emplacements.size() ; k++)
                {
                    bool ok = false;
                    for (int l=0;l<(int)(*trader)[i].m_emplacement.size();l++)
                        if((*trader)[i].m_emplacement[l] == m_classe.emplacements[k].emplacement)
                            ok = true;

                    if(ok)
                        for (int j=0;j<(int)m_inventaire.size();j++)
                            if(m_inventaire[j].m_equipe == k
                            &&(m_inventaire[j].m_equipe_set == m_weaponsSet || m_inventaire[j].m_equipe_set == -1))
                                temp.x=decalage2+12,decalage2=m_inventaire[j].AfficherCaracteristiques(temp,m_classe.border,m_caracteristiques,&m_inventaire,m_cheminClasse,1,1,1,1,1);
                }
            }

           /* sf::Sprite sprite2;
            if (!(*trader)[i].Utilisable(m_caracteristiques,m_cheminClasse))
            {
                sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_unusable));
                sprite2.SetColor(sf::Color(255,255,255,255));

                int taille = (*trader)[i].getTaille().x*32;
                if((*trader)[i].getTaille().y < (*trader)[i].getTaille().x)
                    taille = (*trader)[i].getTaille().y*32;

                sprite2.Resize(taille, taille);
                sprite2.SetX(position.x);
                sprite2.SetY(position.y);

                if((*trader)[i].getTaille().y < (*trader)[i].getTaille().x)
                    sprite2.Move((*trader)[i].getTaille().x*16 - (*trader)[i].getTaille().y*16,0);
                else
                    sprite2.Move(0,(*trader)[i].getTaille().y*16 - (*trader)[i].getTaille().x*16);

                if(sprite2.GetPosition().y < AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y)
                {
                    sprite2.SetSubRect(sf::IntRect(0,(int)(((AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y)-sprite2.GetPosition().y)*128/(taille)),128,128-(int)(((AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y)-sprite2.GetPosition().y)*128/(taille))));
                    sprite2.Move(0,(int)((AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y)-sprite2.GetPosition().y));
                }
                if(sprite2.GetPosition().y + taille > AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y + m_classe.position_contenu_marchand.h * 32 )
                {
                    sprite2.SetSubRect(sf::IntRect(0,0,128,128 - (int)((sprite2.GetPosition().y + taille -
                                                                        (AutoScreenAdjust(0, m_classe.position_contenu_marchand.y - 32, decalage).y+m_classe.position_contenu_marchand.h*32))*128/taille)));
                }

                sprite.SetColor(sf::Color(sprite.GetColor().r,
                                          sprite.GetColor().g,
                                          sprite.GetColor().b,16));
            }*/

            if (!(*trader)[i].Utilisable(m_caracteristiques,m_cheminClasse))
            {
                sf::Sprite sprite2 = sprite;
                sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_item_unusable));
                sprite2.SetColor(sf::Color(96,0,0,128));
                moteurGraphique->AjouterCommande(&sprite2,16,0);
            }
            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.SetOrigin((*trader)[i].getPositionImage().w/2,(*trader)[i].getPositionImage().h/2);

            sprite.SetX(position.x+(*trader)[i].getPositionImage().w/2);
            sprite.SetY(position.y+(*trader)[i].getPositionImage().h/2);

            sprite.SetColor(sf::Color(255,255,255,255));

            sprite.SetImage(*moteurGraphique->getImage((*trader)[i].getImage()));
            sprite.SetSubRect(IntRect((*trader)[i].getPositionImage().x, (*trader)[i].getPositionImage().y, (*trader)[i].getPositionImage().w, (*trader)[i].getPositionImage().h));
            sprite.Resize((*trader)[i].getTaille().x*32,(*trader)[i].getTaille().y*32);

            if ((*trader)[i].getPosition().y+(*trader)[i].getTaille().y-m_defilement_trader>=m_classe.position_contenu_marchand.h)
            {
                sprite.SetSubRect(sf::IntRect((*trader)[i].getPositionImage().x,(*trader)[i].getPositionImage().y,(*trader)[i].getTaille().x*32,(m_defilement_trader+m_classe.position_contenu_marchand.h-(*trader)[i].getPosition().y)*32));
                sprite.Resize((*trader)[i].getTaille().x*32,(m_defilement_trader+m_classe.position_contenu_marchand.h-(*trader)[i].getPosition().y)*32);
            }

            if ((*trader)[i].getPosition().y-m_defilement_trader<0)
            {
                sprite.SetSubRect(sf::IntRect((*trader)[i].getPositionImage().x,(*trader)[i].getPositionImage().y+(-(*trader)[i].getPosition().y+m_defilement_trader)*32,(*trader)[i].getTaille().x*32,(*trader)[i].getTaille().y*32-(-(*trader)[i].getPosition().y+m_defilement_trader)*32));
                sprite.Resize((*trader)[i].getTaille().x*32,(*trader)[i].getTaille().y*32-(-(*trader)[i].getPosition().y+m_defilement_trader)*32);
                sprite.Move(0,(-(*trader)[i].getPosition().y+m_defilement_trader)*32);
            }

            if (m_classe.emplacements[(*trader)[i].m_equipe].emplacement==BOUCLIER && (*trader)[i].m_type==ARME)
                sprite.SetRotation(-45);
            else
                sprite.SetRotation(0);

            if (!(*trader)[i].Utilisable(m_caracteristiques,m_cheminClasse))
                sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.5),(int)(sprite.GetColor().g*0.5),(int)(sprite.GetColor().b*0.5),255));

            moteurGraphique->AjouterCommande(&sprite,17,0);
     //       moteurGraphique->AjouterCommande(&sprite2,17,0);
            sprite.SetRotation(0);
        }
    }

    if (!trader->empty())
    {
        if(m_max_defilement_trader-m_classe.position_contenu_marchand.h > 0)
        {
            sf::Sprite sprite;
            sprite.SetImage(*moteurGraphique->getImage(m_classe.scroll_button.image));
            sprite.SetPosition(AutoScreenAdjust(m_classe.scroll_button.position.x,
                                               (m_classe.scroll_button.position.y+(m_defilement_trader*28*m_classe.position_contenu_marchand.h
                                                /(m_max_defilement_trader-m_classe.position_contenu_marchand.h))),decalage));
            sprite.Resize(m_classe.scroll_button.position.w, m_classe.scroll_button.position.h);

            moteurGraphique->AjouterCommande(&sprite,18,0);
        }
    }

    if(!hideLeft)
        AfficherCaracteristiques(decalage, (!trader->empty() || trader == &m_coffre));

    moteurGraphique->AjouterCommande(&sprite_sort_inv,17,0);


    for(unsigned i = 0 ; i < m_classe.emplacements.size() && !hideLeft ; ++i)
        m_classe.emplacements[i].Afficher(decalage);


    return (retour);
}

void Hero::AfficherRaccourcis()
{
    for (int i=0;i<8;++i)
    {
        sf::Sprite sprite;

        if (m_raccourcis[i].no >= 0
            && ((m_raccourcis[i].no < (int)m_classe.position_miracles.size() && m_raccourcis[i].miracle)
             || (m_raccourcis[i].no < (int)m_inventaire.size() && !m_raccourcis[i].miracle)))
        {
            sprite.SetPosition(AutoScreenAdjust(m_classe.position_raccourcis[i].x,
                                                m_classe.position_raccourcis[i].y));

            if(m_raccourcis[i].miracle)
            {
                sprite.SetImage(*moteurGraphique->getImage(m_classe.icone_miracles[m_raccourcis[i].no].image));
                sprite.SetSubRect(IntRect(  m_classe.icone_miracles[m_raccourcis[i].no].position.x,
                                            m_classe.icone_miracles[m_raccourcis[i].no].position.y,
                                            m_classe.icone_miracles[m_raccourcis[i].no].position.w,
                                            m_classe.icone_miracles[m_raccourcis[i].no].position.h));

                if (m_classe.miracles[m_raccourcis[i].no].m_coutFoi + m_classe.miracles[m_raccourcis[i].no].m_reserveFoi > m_caracteristiques.foi
                 || m_classe.miracles[m_raccourcis[i].no].m_reserveFoi > m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi
                 || m_classe.miracles[m_raccourcis[i].no].m_coutVie + m_classe.miracles[m_raccourcis[i].no].m_reserveVie > m_caracteristiques.vie
                 || m_classe.miracles[m_raccourcis[i].no].m_reserveVie > m_caracteristiques.maxVie - m_caracteristiques.reserveVie
                 ||(m_cas != m_classe.miracles[m_raccourcis[i].no].m_cas1 && m_cas != m_classe.miracles[m_raccourcis[i].no].m_cas2
                     && m_classe.miracles[m_raccourcis[i].no].m_cas1 != -1))
                    sprite.SetColor(sf::Color(32,32,32));

            }

            else
            {
                sprite.SetImage(*moteurGraphique->getImage(m_inventaire[m_raccourcis[i].no].getImage()));

                sprite.SetSubRect(IntRect(  m_inventaire[m_raccourcis[i].no].getPositionImage().x,
                                            m_inventaire[m_raccourcis[i].no].getPositionImage().y,
                                            m_inventaire[m_raccourcis[i].no].getPositionImage().w,
                                            m_inventaire[m_raccourcis[i].no].getPositionImage().h));

                if( m_raccourcis[i].no == m_no_schema_craft ||
                    m_raccourcis[i].no == m_no_result_craft ||
                    m_raccourcis[i].no == m_no_schema_bless ||
                    m_raccourcis[i].no == m_no_result_bless)
                    sprite.SetColor(sf::Color(32,32,32));
            }

            sprite.Resize(m_classe.position_raccourcis[i].w, m_classe.position_raccourcis[i].h);

            moteurGraphique->AjouterCommande(&sprite,18,0);

            if(m_raccourcis[i].nbr > 0 && !m_raccourcis[i].miracle)
            {
                sf::Text text;
                std::ostringstream buf;
                buf<<m_raccourcis[i].nbr;
                text.SetString(buf.str());
                text.SetCharacterSize(10);

                text.SetPosition(AutoScreenAdjust(m_classe.position_raccourcis[i].x,
                                                  m_classe.position_raccourcis[i].y));
                text.Move(26 - text.GetRect().Width,
                          26 - text.GetRect().Height);

                moteurGraphique->AjouterTexte(&text,18);
            }


            if( eventManager->getPositionSouris().x > sprite.GetPosition().x
             && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
             && eventManager->getPositionSouris().y > sprite.GetPosition().y
             && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y && m_objetEnMain < 0)
            {
                if(m_raccourcis[i].miracle)
                    m_classe.miracles[m_raccourcis[i].no].AfficherDescription(eventManager->getPositionSouris(), m_classe.border, false);
                else
                     m_inventaire[m_raccourcis[i].no].AfficherCaracteristiques(eventManager->getPositionSouris(),m_classe.border, m_caracteristiques,&m_inventaire,m_cheminClasse,1,0,0,0,1);
            }


            if(m_raccourcis[i].miracle)
            {
                if(m_classe.miracles[m_raccourcis[i].no].m_cooldown > 0
                && m_classe.miracles[m_raccourcis[i].no].m_cooldown != m_classe.miracles[m_raccourcis[i].no].m_cur_time)
                {
                    float temp = (float)m_classe.icone_miracles[m_raccourcis[i].no].position.h
                                 *(m_classe.miracles[m_raccourcis[i].no].m_cooldown
                                   - m_classe.miracles[m_raccourcis[i].no].m_cur_time)
                                 / m_classe.miracles[m_raccourcis[i].no].m_cooldown;

                    sprite.SetImage(*moteurGraphique->getImage(0));
                    sprite.Resize(m_classe.position_raccourcis[i].w,temp);

                    sprite.SetColor(sf::Color(0,0,0,240));
                    moteurGraphique->AjouterCommande(&sprite,18,0);
                }
            }
            else
            {
                for(unsigned j = 0 ; j < m_items_cooldown.size() ; ++j)
                    if(m_items_cooldown[j].name == m_inventaire[m_raccourcis[i].no].getChemin())
                    {
                        float temp = (float)m_classe.position_raccourcis[i].h
                                     *(m_items_cooldown[j].max_time
                                       - m_items_cooldown[j].time)
                                     / m_items_cooldown[j].max_time;

                        sprite.SetImage(*moteurGraphique->getImage(0));
                        sprite.Resize(m_classe.position_raccourcis[i].w,temp);

                        sprite.SetColor(sf::Color(0,0,0,240));
                        moteurGraphique->AjouterCommande(&sprite,18,0);
                    }
            }

        }
        else
        {
            if( eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x
             && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x + m_classe.position_raccourcis[i].w
             && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y
             && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y + m_classe.position_raccourcis[i].h && m_objetEnMain < 0)
            {
                std::ostringstream buf;
                buf<<configuration->getText(0, 33)<<" ("<<i+1<<")";
                moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                              eventManager->getPositionSouris().y - 20),
                                              m_classe.border,19,0,12,sf::Color(224,224,224));
            }
        }
    }

    if (m_miracle_droite[m_weaponsSet] >= 0
     && m_miracle_droite[m_weaponsSet] < (int)m_classe.position_miracles.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_classe.icone_miracles[m_miracle_droite[m_weaponsSet]].image));

        sprite.SetSubRect(IntRect(  m_classe.icone_miracles[m_miracle_droite[m_weaponsSet]].position.x,
                                    m_classe.icone_miracles[m_miracle_droite[m_weaponsSet]].position.y,
                                    m_classe.icone_miracles[m_miracle_droite[m_weaponsSet]].position.w,
                                    m_classe.icone_miracles[m_miracle_droite[m_weaponsSet]].position.h));

        sprite.SetPosition(AutoScreenAdjust(m_classe.p_miracleD.position.x,
                                            m_classe.p_miracleD.position.y - 2));

        sprite.SetBlendMode(sf::Blend::Alpha);

        sprite.Resize(m_classe.p_miracleD.position.w,m_classe.p_miracleD.position.h);

        if (m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_coutFoi + m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_reserveFoi > m_caracteristiques.foi
         || m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_reserveFoi > m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi
         || m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_coutVie + m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_reserveVie > m_caracteristiques.vie
         || m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_reserveVie > m_caracteristiques.maxVie - m_caracteristiques.reserveVie)
            sprite.SetColor(sf::Color(64,64,64));

        moteurGraphique->AjouterCommande(&sprite,18,0);

        if(eventManager->getPositionSouris().x > sprite.GetPosition().x
        && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
        && eventManager->getPositionSouris().y > sprite.GetPosition().y
        && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
            m_classe.miracles[m_miracle_droite[m_weaponsSet]].AfficherDescription(eventManager->getPositionSouris(), m_classe.border, false,2);


        if(m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cooldown > 0
        && m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cooldown != m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cur_time)
        {
            float temp = (float)m_classe.p_miracleD.position.h
                         *(m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cooldown
                           - m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cur_time)
                         / m_classe.miracles[m_miracle_droite[m_weaponsSet]].m_cooldown;

            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.Resize(m_classe.p_miracleD.position.w,temp);

            sprite.SetColor(sf::Color(0,0,0,240));
            moteurGraphique->AjouterCommande(&sprite,18,0);
        }
    }
    else
        AfficherIconeAttaque(m_classe.p_miracleD);

    if (m_miracle_gauche[m_weaponsSet] >= 0 && m_miracle_gauche[m_weaponsSet] < (int)m_classe.position_miracles.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_classe.icone_miracles[m_miracle_gauche[m_weaponsSet]].image));

        sprite.SetSubRect(IntRect(  m_classe.icone_miracles[m_miracle_gauche[m_weaponsSet]].position.x,
                                    m_classe.icone_miracles[m_miracle_gauche[m_weaponsSet]].position.y,
                                    m_classe.icone_miracles[m_miracle_gauche[m_weaponsSet]].position.w,
                                    m_classe.icone_miracles[m_miracle_gauche[m_weaponsSet]].position.h));

        sprite.SetPosition(AutoScreenAdjust(m_classe.p_miracleG.position.x,
                                            m_classe.p_miracleG.position.y - 2));

        sprite.SetBlendMode(sf::Blend::Alpha);

        sprite.Resize(m_classe.p_miracleG.position.w,m_classe.p_miracleG.position.h);

        if (m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_coutFoi + m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_reserveFoi > m_caracteristiques.foi
         || m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_reserveFoi > m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi
         || m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_coutVie + m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_reserveVie > m_caracteristiques.vie
         || m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_reserveVie > m_caracteristiques.maxVie - m_caracteristiques.reserveVie)
            sprite.SetColor(sf::Color(64,64,64));

        moteurGraphique->AjouterCommande(&sprite,18,0);


        if(eventManager->getPositionSouris().x > sprite.GetPosition().x
        && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
        && eventManager->getPositionSouris().y > sprite.GetPosition().y
        && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
            m_classe.miracles[m_miracle_gauche[m_weaponsSet]].AfficherDescription(eventManager->getPositionSouris(), m_classe.border, false,1);


        if(m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cooldown > 0
        && m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cooldown != m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cur_time)
        {
            float temp = (float)m_classe.p_miracleG.position.h
                         *(m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cooldown
                           - m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cur_time)
                         / m_classe.miracles[m_miracle_gauche[m_weaponsSet]].m_cooldown;

            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.Resize(m_classe.p_miracleG.position.w,temp);

            sprite.SetColor(sf::Color(0,0,0,240));
            moteurGraphique->AjouterCommande(&sprite,18,0);
        }
    }
    else
        AfficherIconeAttaque(m_classe.p_miracleG);
}


void Hero::AfficherIconeAttaque(Image_interface &img)
{
    sf::Sprite sprite;

    sprite.SetImage(*moteurGraphique->getImage(img.image));

    sprite.SetPosition(AutoScreenAdjust(img.position.x,
                                        img.position.y - 2));

    sprite.SetBlendMode(sf::Blend::Alpha);

    sprite.Resize(img.position.w,img.position.h);

    if(&img == &m_classe.p_miracleD)
        sprite.FlipX(true);

    moteurGraphique->AjouterCommande(&sprite,18,0);

    if(eventManager->getPositionSouris().x > sprite.GetPosition().x
    && eventManager->getPositionSouris().x < sprite.GetPosition().x + sprite.GetSize().x
    && eventManager->getPositionSouris().y > sprite.GetPosition().y
    && eventManager->getPositionSouris().y < sprite.GetPosition().y + sprite.GetSize().y)
    {
        if(&img == &m_classe.p_miracleG)
            m_classe.miracles[0].AfficherDescription(eventManager->getPositionSouris(), m_classe.border, false,1);
        else
            m_classe.miracles[0].AfficherDescription(eventManager->getPositionSouris(), m_classe.border, false,2);
    }

}

void Hero::PlacerCamera()
{
    configuration->zoom = configuration->zoom_or + m_personnage.getCoordonneePixel().h*0.002;

    sf::Vector2f next_pos(((m_personnage.getCoordonneePixel().x - m_personnage.getCoordonneePixel().y) * 64.0f * DIVISEUR_COTE_TILE),
                          ((m_personnage.getCoordonneePixel().x + m_personnage.getCoordonneePixel().y) * 32.0f * DIVISEUR_COTE_TILE + 32.0f) - m_personnage.getCoordonneePixel().h);

   /* if(moteurGraphique->m_camera.GetCenter().x < next_pos.x)
        moteurGraphique->m_camera.Move(1,0);
    else if(moteurGraphique->m_camera.GetCenter().x > next_pos.x)
        moteurGraphique->m_camera.Move(-1,0);

    if(moteurGraphique->m_camera.GetCenter().y < next_pos.y)
        moteurGraphique->m_camera.Move(0,1);
    else if(moteurGraphique->m_camera.GetCenter().y > next_pos.y)
        moteurGraphique->m_camera.Move(0,-1);*/

    moteurGraphique->m_camera.SetCenter(next_pos);

    moteurGraphique->m_camera.SetSize(configuration->Resolution.x, configuration->Resolution.y);
    moteurGraphique->m_camera.Zoom(configuration->zoom);

    moteurGraphique->m_camera.Move(moteurGraphique->decalageCamera.x, moteurGraphique->decalageCamera.y);
}

bool Hero::TestMonstreVise(Personnage *monstre)
{
    if (monstre!=NULL)
        if (m_caracteristiques.vie>0)
        {
            if (((!m_personnage.m_shooter||monstre->m_friendly)&&(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)<=1))
            || ((!m_personnage.m_shooter||monstre->m_friendly)&&(fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)<=1))
            || (!monstre->m_friendly&&m_personnage.m_shooter&&(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)<=13&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)<=13))
            || (!monstre->m_friendly&&m_personnage.m_shooter&&(fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)<=13&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)<=13)))
            {

                float m=atan2(-(double)(monstre->getCoordonneePixel().y-m_personnage.getCoordonneePixel().y),
                               (double)(monstre->getCoordonneePixel().x-m_personnage.getCoordonneePixel().x));

                m = (int)(m*180/M_PI);

                if(m < 0)
                    m += 360;
                if(m > 360)
                    m -= 360;

                if(m - (int)(m/45) * 45 > 23)
                    m = (int)(m/45) * 45 + 45;
                else
                    m = (int)(m/45) * 45;

                m-=45;

                m_personnage.setAngle((int)m);

                if(monstre->m_collision)
                    m_personnage.setArrivee(m_personnage.getCoordonnee());
                else
                    m_personnage.setArrivee(monstre->getCoordonnee());


                if (!monstre->m_friendly)
                    m_personnage.Frappe(m_personnage.getCoordonneePixel(),monstre->getCoordonneePixel());

                if(monstre->m_collision)
                    return 1;
            }
            else
                m_personnage.setArrivee(monstre->getProchaineCase());
        }
    return 0;
}

void Hero::GererTemps(float temps)
{
    m_craft_time -= temps;
    m_bless_time -= temps;

    if(m_craft_time <= 0)
        m_craft_time = 0;
    if(m_bless_time <= 0)
        m_bless_time = 0;

    Caracteristique temp = m_personnage.getCaracteristique();

    temp.ancienPointAme += (temp.pointAme-temp.positionAncienAme+1)*temps*0.7;

    if (temp.ancienPointAme > temp.pointAme)
        temp.ancienPointAme = temp.pointAme,temp.positionAncienAme=temp.ancienPointAme;

    if (temp.ancienPointAme >= CALCUL_PA_PROCHAIN_NIVEAU)
    {
        temp.ancienPointAme = temp.pointAme;

        temp.niveau++;
        temp.pts_restant+=10;
        temp.miracles_restant++;

        m_personnage.setCaracteristique(temp);

        RecalculerCaracteristiques();
        RecalculerCaracteristiques();

        temp.vie=m_caracteristiques.maxVie;
        temp.foi=m_caracteristiques.maxFoi;
        m_caracteristiques.vie=m_caracteristiques.maxVie;
        m_caracteristiques.foi=m_caracteristiques.maxFoi;

        UtiliserMiracle(0,NULL,coordonnee (),NULL);
    }

    m_personnage.setCaracteristique(temp);

    RecalculerCaracteristiques();

    m_caracteristiques.vie=temp.vie;
    m_caracteristiques.foi=temp.foi;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.ancienPointAme=temp.ancienPointAme;
}


void Hero::RecalculerCaracteristiques(bool bis)
{
    float vie                   = m_caracteristiques.vie;
    float foi                   = m_caracteristiques.foi;
    float reserveVie            = m_caracteristiques.reserveVie;
    float reserveFoi            = m_caracteristiques.reserveFoi;

    Caracteristique temp        = m_personnage.getCaracteristique();
    Caracteristique buf         = m_caracteristiques;

    reserveVie                  = temp.reserveVie;
    reserveFoi                  = temp.reserveFoi;

    m_caracteristiques          = m_personnage.getCaracteristique();
    m_caracteristiques.maxVie   = 0;
    m_caracteristiques.maxFoi   = 0;

    for(int i = 0 ; i < 4 ; ++i)
    {
        m_caracteristiques.degatsMin[i]=0;
        m_caracteristiques.degatsMax[i]=0;
        m_caracteristiques.armure[i]=0;

        temp.degatsMin[i]=0;
        temp.degatsMax[i]=0;
        temp.armure[i]=0;
    }

    m_caracteristiques.armure[PHYSIQUE] = m_caracteristiques.dexterite*2;
    temp.armure[PHYSIQUE]               = m_caracteristiques.dexterite*2;

    m_lvl_miracles_new = m_lvl_miracles;

    std::vector<Set *> liste_set;

    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0
         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
            if (m_inventaire[i].Utilisable(buf,m_cheminClasse))
            {
                 if (m_inventaire[i].m_type==ARME)
                {
                    if (m_inventaire[i].m_shoot_weapon)
                        m_personnage.m_shooter=true;
                    else
                        m_personnage.m_shooter=false;
                }

                if(!m_inventaire[i].m_chemin_set.empty())
                {
                    bool ajouter = true;
                    for(unsigned j = 0 ; j < liste_set.size(); ++j)
                        if(liste_set[j]->m_chemin == m_inventaire[i].m_chemin_set)
                            ajouter = false, liste_set[j]->m_nombre++;

                    if(ajouter)
                        liste_set.push_back(&m_inventaire[i].m_set), liste_set.back()->m_nombre = 0;
                }

                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    AjouterEffetsBenediction1(m_inventaire[i].m_benedictions[j]);
            }

    for (unsigned i=0;i<liste_set.size();++i)
        for (unsigned k=0;(int)k < liste_set[i]->m_nombre && k < liste_set[i]->m_benedictions.size();++k)
        {
            for (int j=0;j<(int)liste_set[i]->m_benedictions[k].size();++j)
                AjouterEffetsBenediction1(liste_set[i]->m_benedictions[k][j]);
        }

    m_caracteristiques.maxVie += m_caracteristiques.vitalite*10;
    m_caracteristiques.maxFoi += m_caracteristiques.piete*10;

    for(std::vector<EntiteMiracle>::iterator i = m_personnage.m_miracleEnCours.begin() ;
        i != m_personnage.m_miracleEnCours.end() ; ++i)
    {
        for(std::vector<InfosEntiteMiracle*>::iterator j = i->m_infos.begin() ; j != i->m_infos.end() ; ++j)
            if(m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_type == DECLENCHEUR)
                if(m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[0] == D_FRAPPE)
                    if(m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_type == DEGATS)
                    {
                        if(m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[2] >= 0
                        && m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[2] < 4)
                        {
                            bool add = true;
                            for(unsigned o = 0 ; o < m_weaponMiracle.size() ; ++o)
                                if(m_weaponMiracle[o] == i->m_modele)
                                    add =false;

                            if(add)
                            {
                                m_caracteristiques.degatsMin[m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[2]] += m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[0];
                                m_caracteristiques.degatsMax[m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[2]] += m_classe.miracles[i->m_modele].m_effets[m_classe.miracles[i->m_modele].m_effets[(*j)->m_effetEnCours].m_informations[3]].m_informations[1];
                            }
                        }
                    }
    }

    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0
         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
            if (m_inventaire[i].Utilisable(buf,m_cheminClasse))
            {
                int accru=100;
                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    if (m_inventaire[i].m_benedictions[j].type==EFFICACITE_ACCRUE)
                        accru+=m_inventaire[i].m_benedictions[j].info1;

                m_caracteristiques.degatsMin[PHYSIQUE]  += m_inventaire[i].m_degatsMin*accru*(m_caracteristiques.force + 100)/10000;
                m_caracteristiques.degatsMax[PHYSIQUE]  += m_inventaire[i].m_degatsMax*accru*(m_caracteristiques.force + 100)/10000;
                m_caracteristiques.armure[PHYSIQUE]     += m_inventaire[i].m_armure*accru/100;

                temp.degatsMin[PHYSIQUE]  += m_inventaire[i].m_degatsMin*accru*(m_caracteristiques.force + 100)/10000;
                temp.degatsMax[PHYSIQUE]  += m_inventaire[i].m_degatsMax*accru*(m_caracteristiques.force + 100)/10000;
                temp.armure[PHYSIQUE]     += m_inventaire[i].m_armure*accru/100;

                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    AjouterEffetsBenediction2(m_inventaire[i].m_benedictions[j], temp);
            }

    for (unsigned i=0;i<liste_set.size();++i)
        for (unsigned k=0; (int)k<liste_set[i]->m_nombre && k < liste_set[i]->m_benedictions.size();++k)
            for (int j=0;j<(int)liste_set[i]->m_benedictions[k].size();++j)
                AjouterEffetsBenediction2(liste_set[i]->m_benedictions[k][j], temp);

    for (int i = 0; i < (int)m_personnage.m_effets.size(); ++i)
        if (m_personnage.m_effets[i].m_effet.m_actif)
        {
            if (m_personnage.m_effets[i].m_type == AURA_REGENERATION)
            {
                if (!m_personnage.m_effets[i].m_info3)
                {
                    if (m_personnage.m_effets[i].m_info1 == 0)
                        m_caracteristiques.regenVie += m_personnage.m_effets[i].m_info2;
                    if (m_personnage.m_effets[i].m_info1 == 1)
                        m_caracteristiques.regenFoi += m_personnage.m_effets[i].m_info2;
                }
            }

            if (m_personnage.m_effets[i].m_type == AURA_VOL)
            {
                if (m_personnage.m_effets[i].m_info1 == 0)
                    m_caracteristiques.volVie += m_personnage.m_effets[i].m_info2*0.01;
                if (m_personnage.m_effets[i].m_info1 == 1)
                    m_caracteristiques.volFoi += m_personnage.m_effets[i].m_info2*0.01;
            }
        }

    m_caracteristiques.niveau = temp.niveau;

    temp.reserveVie = reserveVie;
    temp.reserveFoi = reserveFoi;

    m_personnage.setCaracteristique(temp);

    m_personnage.RecalculerEffets();

    for(int i = 0 ; i < 4 ; ++i)
        m_caracteristiques.armure[i] = m_personnage.getCaracteristique().armure[i];

    m_caracteristiques.reserveVie  = reserveVie;
    m_caracteristiques.reserveFoi  = reserveFoi;

    m_caracteristiques.vie = vie;
    m_caracteristiques.foi = foi;

    if (m_caracteristiques.vie>(m_caracteristiques.maxVie - m_caracteristiques.reserveVie)*2)
        m_caracteristiques.vie=(m_caracteristiques.maxVie - m_caracteristiques.reserveVie)*2;
    if (m_caracteristiques.foi>(m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi)*2)
        m_caracteristiques.foi=(m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi)*2;

    if (bis)
        RecalculerCaracteristiques(false);

    temp                           = m_personnage.getCaracteristique();
    temp.maxVie                    = m_caracteristiques.maxVie;
    temp.maxFoi                    = m_caracteristiques.maxFoi;
    m_personnage.setCaracteristique(temp);

    RecalculerGolems();
}

void Hero::AjouterEffetsBenediction1(benediction &bene)
{
    switch (bene.type)
    {
        case CARACT_SUPP:
            if(bene.info1 == FORCE)
                m_caracteristiques.force    += bene.info2;
            if(bene.info1 == DEXTERITE)
                m_caracteristiques.dexterite+= bene.info2;
            if(bene.info1 == VITALITE)
                m_caracteristiques.vitalite += bene.info2;
            if(bene.info1 == PIETE)
                m_caracteristiques.piete    += bene.info2;
            if(bene.info1 == CHARISME)
                m_caracteristiques.charisme += bene.info2;
            break;
        case POINTS_SUPP:
            if(bene.info1 == PT_VIE)
                m_caracteristiques.maxVie   += bene.info2;
            if(bene.info1 == PT_FOI)
                m_caracteristiques.maxFoi   += bene.info2;
            break;
        case MIRACLE_SUPP:
            int no = -1;

            for(unsigned k = 0 ; k < m_classe.miracles.size() ; ++k)
                if(m_classe.miracles[k].m_chemin == bene.text)
                    no = k;

            if(no >= 0 && no < (int)m_lvl_miracles_new.size())
                m_lvl_miracles_new[no] += bene.info1;
            break;
    }
}

void Hero::AjouterEffetsBenediction2(benediction &bene, Caracteristique &temp)
{
     if (bene.type==DEGATS_SUPP)
     {
        m_caracteristiques.degatsMin[bene.info1]+=bene.info2;
        m_caracteristiques.degatsMax[bene.info1]+=bene.info3;
     }
     if (bene.type==DEGATS_TEMPS_SUPP)
     {
        m_caracteristiques.degatsMin[bene.info1]+=bene.info2;
        m_caracteristiques.degatsMax[bene.info1]+=bene.info2;
     }

     if (bene.type==ARMURE_SUPP)
     {
        m_caracteristiques.armure[bene.info1]+=bene.info2;
        temp.armure[bene.info1]+=bene.info2;
     }
}

void Hero::RecalculerGolems()
{
    for(unsigned i = 0 ; i < m_inventaire.size() ; ++i)
        if(m_inventaire[i].m_type == GOLEM)
            m_inventaire[i].CalculerGolem();
}

void Hero::JouerSonAttaque(int materiau)
{
    if(m_weaponModel >= 0 && m_weaponModel < NOMBRE_MORCEAU_PERSONNAGE)
    if ((int)(m_personnage.getAngle()/45) >= 0
     && (int)(m_personnage.getAngle()/45) < (int)m_modelePersonnage[m_weaponModel].m_tileset[m_personnage.getEtat()].size())
    {
        Tileset *tileset = &m_modelePersonnage[m_weaponModel].m_tileset[m_personnage.getEtat()][(int)(m_personnage.getAngle()/45)];

        if(materiau > 0)
        if(tileset->getNombreSonsSpecial(materiau) > 0)
        {
            int random = rand()%tileset->getNombreSonsSpecial(materiau);
            int nbr = 0;

            coordonnee position;
            position.x=(m_personnage.getCoordonnee().x-m_personnage.getCoordonnee().y-1)/5;
            position.y=(m_personnage.getCoordonnee().x+m_personnage.getCoordonnee().y)/5;

            while(!tileset->JouerSon(tileset->getSonSpecial(materiau, random++),position, true)
                && nbr++ < tileset->getNombreSonsSpecial(materiau))
            {
                if(random >= tileset->getNombreSonsSpecial(materiau))
                    random = 0;
            }
        }
    }
}

void Hero::StopMiraclesFrappe()
{
    for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)//{cout<<m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours<<endl;
            if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == CORPS_A_CORPS)
            {
                delete m_personnage.m_miracleEnCours[i].m_infos[o];
                m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin()+o--);
            }//}
}

void Hero::StopMiraclesCharme()
{
    for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
            if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == CHARME)
            {
                delete m_personnage.m_miracleEnCours[i].m_infos[o];
                m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin()+o--);
            }
}

bool Hero::UtiliserMiracle(int miracle, Personnage *cible, coordonnee cible_coord, Jeu *jeu)
{
  /*  float m=atan2((double)(m_personnage.getCoordonnee().x-cible_coord.x),(double)(m_personnage.getCoordonnee().y-cible_coord.y));
    m+=M_PI/3;

    m=(int)(m*180/M_PI);
    if (m>=360)
        m=0;
    if (m<0)
        m=360+m;
    m_personnage.setAngle((int)m);*/


    if(!m_personnage.m_miracleBloquant)
    if (miracle>=0&&miracle<(int)m_classe.miracles.size())
        if (m_lvl_miracles[miracle] > 0)
        {
            for (unsigned k = 0 ; k < m_classe.miracles[miracle].m_effets.size() ; ++k)
                if (m_classe.miracles[miracle].m_effets[k].m_type == AURA)
                {
                    bool retour = false;
                    for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
                        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
                            if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == AURA)
                            {
                                 for (int r = o; r < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++r)
                                    if (m_personnage.m_miracleEnCours[i].m_infos[r]->m_effetEnCours == m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_informations[0])
                                    {
                                        if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[r]->m_effetEnCours].m_type == EFFET)
                                            m_personnage.m_miracleEnCours[i].m_infos[r]->m_cible->m_effets[m_personnage.m_miracleEnCours[i].m_infos[r]->m_IDObjet].m_effet.m_actif = false;

                                        m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin() + r);
                                        r--;
                                    }

                                for (int p=0;p<(int)m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                                {
                                    m_personnage.m_miracleEnCours[i].m_infos.push_back(new InfosEntiteMiracle ());
                                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_effetEnCours=m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p];
                                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_position=m_personnage.m_miracleEnCours[i].m_infos[o]->m_position;
                                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_cible=m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible;
                                }

                                m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin()+o);

                                if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_chemin ==
                                    m_classe.miracles[miracle].m_chemin)
                                    retour = true;
                                o--;
                            }

                    if (retour)
                        return 1;
                }

            if(m_classe.miracles[miracle].m_cur_time == m_classe.miracles[miracle].m_cooldown)
            if (m_classe.miracles[miracle].m_coutFoi + m_classe.miracles[miracle].m_reserveFoi <= m_caracteristiques.foi
             && m_classe.miracles[miracle].m_reserveFoi <= m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi
            && m_classe.miracles[miracle].m_coutVie + m_classe.miracles[miracle].m_reserveVie
            <= m_caracteristiques.vie && m_classe.miracles[miracle].m_reserveVie <= m_caracteristiques.maxVie - m_caracteristiques.reserveVie)
            {
                    if((m_cas == m_classe.miracles[miracle].m_cas1 || m_cas == m_classe.miracles[miracle].m_cas2)
                     || m_classe.miracles[miracle].m_cas1 == -1)

                    {
                        if ((cible != NULL && m_classe.miracles[miracle].m_effets[0].m_type == CORPS_A_CORPS)
                         || m_classe.miracles[miracle].m_effets[0].m_type != CORPS_A_CORPS
                         || eventManager->getEvenement(sf::Keyboard::LShift, EventKey))
                        {

                            if(m_classe.miracles[miracle].m_unique)
                                for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
                                    if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_chemin == m_classe.miracles[miracle].m_chemin)
                                    {
                                        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
                                        {
                                            if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET)
                                                m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible->m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_IDObjet].m_effet.m_actif = false;
                                            if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET_GRAPHIQUE)
                                                jeu->map->DelEffet(m_personnage.m_miracleEnCours[i].m_infos[o]->m_IDObjet);
                                        }

                                        m_personnage.m_miracleEnCours.erase(m_personnage.m_miracleEnCours.begin()+i--);
                                    }

                            m_personnage.m_lancementMiracleEnCours = true;
                            m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());

                            m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                            m_personnage.m_miracleEnCours.back().m_modele = miracle;

                            m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x;
                            m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y;

                            m_personnage.m_miracleEnCours.back().m_coordonneeCible  = cible_coord;
                            m_personnage.m_miracleEnCours.back().m_coordonneeDepart = m_personnage.getCoordonnee();

                            float m=atan2(-(double)(cible_coord.y-m_personnage.getCoordonnee().y),(double)(cible_coord.x-m_personnage.getCoordonnee().x));

                            m = (int)(m*180/M_PI);

                            if(m < 0)
                                m += 360;
                            if(m > 360)
                                m -= 360;

                            if(m - (int)(m/45) * 45 > 23)
                                m = (int)(m/45) * 45 + 45;
                            else
                                m = (int)(m/45) * 45;

                            m-=45;

                            m_personnage.setAngle((int)m);

                            return 1;
                        }
                    }
                    else
                    {
                        m_personnage.m_speak = configuration->getText(0, 42);
                        m_personnage.m_speak_time = 2000;
                    }
                }
        }
    return 0;
}

void Hero::addPotale(int x, int y, int nom, const std::string &chemin)
{
    bool add = true;
    for(unsigned i = 0 ; i < m_potales.size() ; ++i)
        if(m_potales[i].position.x == x)
            if(m_potales[i].position.y == y)
                if(m_potales[i].chemin == chemin)
                {
                    add = false;
                    m_last_potale = i;
                }


    if(chemin.empty())
        add = false;

    if(add)
    {
        m_last_potale = m_potales.size();
        m_potales.push_back(Potale ());
        m_potales.back().chemin = chemin;
        m_potales.back().position.x = x;
        m_potales.back().position.y = y;
        m_potales.back().nom = nom;
    }
}

bool Hero::AjouterObjet(Objet &objet,bool enMain, bool playSound)
{
    bool ramasser=false;

    if (!enMain)
    {
        if (objet.m_type == ARME
         || objet.m_type == ARMURE
         || objet.m_type == JEWELERY)
        {
            bool continuer = true;
            for (int j=0;j<(int)m_classe.emplacements.size() && continuer;j++)
            {
                if(PossibleEquiper(objet,j))
                    for (int i=0;i<(int)objet.m_emplacement.size() && continuer;++i)
                        if (objet.m_emplacement[i] == m_classe.emplacements[j].emplacement && continuer)
                        {
                            continuer=false;

                            for (int k=0;k<(int)objet.m_emplacementImpossible.size();k++)
                                for(int o = 0 ; o < (int)m_inventaire.size() ; ++o)
                                    if(m_inventaire[o].m_equipe >= 0 && m_inventaire[o].m_equipe < (int)m_classe.emplacements.size()
                                    &&(m_inventaire[o].m_equipe_set == m_weaponsSet || m_inventaire[o].m_equipe_set == -1))
                                        if (objet.m_emplacementImpossible[k]==m_classe.emplacements[m_inventaire[o].m_equipe].emplacement)
                                            continuer = true;

                            for(unsigned k = 0 ; k < m_inventaire.size() ; ++k)
                                if(m_inventaire[k].m_equipe >= 0 && m_inventaire[k].m_equipe < (int)m_classe.emplacements.size()
                                &&(m_inventaire[k].m_equipe_set == m_weaponsSet || m_inventaire[k].m_equipe_set == -1))
                                    for (unsigned o=0;o<m_inventaire[k].m_emplacementImpossible.size();o++)
                                        if(m_classe.emplacements[j].emplacement == m_inventaire[k].m_emplacementImpossible[o])
                                            continuer = true;


                            for(unsigned k = 0 ; k < m_inventaire.size() ; ++k)
                                if(m_inventaire[k].m_equipe == j)
                                    continuer = true;

                            if(!continuer)
                            {
                                ramasser = true;
                                AjouterObjetInventaire(objet,&m_inventaire,m_classe.position_contenu_inventaire, true);
                                Equiper(m_inventaire.size() - 1, j, playSound);
                            }
                        }
            }

            if(!continuer)
                ChargerModele();
        }

        if(objet.m_type == GOLD)
        {
            ramasser = true;
            m_argent += objet.m_vie;
        }
        else
        {
            if(!ramasser)
                ramasser = AjouterObjetInventaire(objet,&m_inventaire,m_classe.position_contenu_inventaire, false);
        }


        if (ramasser)
        {
            if(playSound)
                objet.JouerSon();

            if (m_inventaire.back().m_type == CONSOMMABLE)
            {
                for(int i = 7 ; i >= 0 ; --i)
                {
                    if(m_raccourcis[i].no == -1)
                        m_raccourcis[i].no = m_inventaire.size() - 1, i = -1;
                    else if(m_raccourcis[i].no >= 0 && m_raccourcis[i].no < (int)m_inventaire.size() && !m_raccourcis[i].miracle)
                        if(m_inventaire[m_raccourcis[i].no].getChemin() == m_inventaire.back().getChemin())
                            i = -1;
                }
            }
        }

    }
    else if (m_objetEnMain==-1)
    {
        m_inventaire.push_back(objet);

        m_objetEnMain=m_inventaire.size()-1;

        ramasser = true;
    }

    UpdateRaccourcis();

    if(!ramasser)
    {
        m_personnage.m_speak_time = 2000;
        m_personnage.m_speak = configuration->getText(0, 43);
    }

    return ramasser;
}

Objet Hero::DeposerObjet()
{
    Objet temp;
    if (m_objetADeposer>=0&&m_objetADeposer<(int)m_inventaire.size())
    {
        temp = m_inventaire[m_objetADeposer];

        temp.JouerSon();

        delObjet(m_objetADeposer);

        temp.setPosition(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y);

        temp.m_equipe=-1;

        RecalculerCaracteristiques();

        m_objetADeposer=-1;
        m_objetEnMain=-1;
        m_achat=false;
    }
    return temp;
}

void Hero::AutoTrierCoffre()
{
    int ordre_tri[9] = {CONSOMMABLE,AUCUN,JEWELERY,LITANIE,SCHEMA,DOCUMENT,GOLEM,ARMURE,ARME};
    std::vector<Objet> inventaire_bis;

    std::vector<std::string> nom_objets;
    std::vector<int> taille_objets;

    for(int o = 0 ; o < 9 ; ++o)
    for(unsigned i = 0 ; i < m_coffre.size() ; ++i)
    if(m_coffre[i].m_type == ordre_tri[o]
    && m_coffre[i].m_equipe < 0)
    {
        std::vector<std::string> nom_objets_bis;
        std::vector<int> taille_objets_bis;
        bool ajouter = true;
        for(unsigned k = 0 ; k < nom_objets.size() ; ++k)
            if(nom_objets[k] == m_coffre[i].getChemin())
                ajouter = false;

        if(ajouter)
        {
            unsigned t = 0;
            for(t = 0 ; t < taille_objets.size() ; ++t)
                if(taille_objets[t] < m_coffre[i].getTaille().y)
                {
                    for(unsigned T = 0 ; T < t ; ++T)
                    {
                        nom_objets_bis.push_back(nom_objets[T]);
                        taille_objets_bis.push_back(taille_objets[T]);
                    }

                    taille_objets_bis.push_back(m_coffre[i].getTaille().y);
                    nom_objets_bis.push_back(m_coffre[i].getChemin());


                    for(unsigned T = t ; T < taille_objets.size() ; ++T)
                    {
                        nom_objets_bis.push_back(nom_objets[T]);
                        taille_objets_bis.push_back(taille_objets[T]);
                    }

                    t = taille_objets.size() + 1;
                }

            if(t == taille_objets.size())
            {
                taille_objets.push_back(m_coffre[i].getTaille().y);
                nom_objets.push_back(m_coffre[i].getChemin());
            }
            else
            {
                nom_objets.clear();
                taille_objets.clear();
                for(unsigned T = 0 ; T < taille_objets_bis.size() ; ++T)
                {
                        nom_objets.push_back(nom_objets_bis[T]);
                        taille_objets.push_back(taille_objets_bis[T]);
                }
            }
        }
    }

    for(unsigned t = 0 ; t < 9 ; ++t)
    for(unsigned n = 0 ; n < nom_objets.size() ; ++n)
    for(unsigned i = 0 ; i < m_coffre.size() ; ++i)
    if(m_coffre[i].m_type == ordre_tri[t]
    && m_coffre[i].m_equipe < 0
    && m_coffre[i].getChemin() == nom_objets[n])
    {
        inventaire_bis.push_back(m_coffre[i]);
    }

    for(unsigned i = 0 ; i < m_coffre.size() ; ++i)
        if(m_coffre[i].m_equipe >= 0)
            inventaire_bis.push_back(m_coffre[i]);

    m_coffre.clear();
    for(unsigned i = 0 ; i < inventaire_bis.size() ; ++i)
        m_coffre.push_back(inventaire_bis[i]), m_coffre.back().m_dejaTrie = false;


    TrierInventaire(&m_coffre,m_classe.position_contenu_marchand.w);
}

void Hero::AutoTrierInventaire()
{
    int ordre_tri[9] = {ARME,ARMURE,GOLEM,SCHEMA,LITANIE,DOCUMENT,AUCUN,JEWELERY,CONSOMMABLE};
    int new_raccourci[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

    int new_no_schema_craft = -1;
    int new_no_schema_bless = -1;
    int new_no_result_craft = -1;
    int new_no_result_bless = -1;

    std::vector<Objet> inventaire_bis;

    std::vector<std::string> nom_objets;
    std::vector<int> taille_objets;

    for(int o = 0 ; o < 9 ; ++o)
    for(unsigned i = 0 ; i < m_inventaire.size() ; ++i)
    if(m_inventaire[i].m_type == ordre_tri[o]
    && m_inventaire[i].m_equipe < 0
    && m_no_schema_bless != (int)i
    && m_no_schema_craft != (int)i
    && m_no_result_craft != (int)i
    && m_no_result_bless != (int)i)
    {
        std::vector<std::string> nom_objets_bis;
        std::vector<int> taille_objets_bis;
        bool ajouter = true;
        for(unsigned k = 0 ; k < nom_objets.size() ; ++k)
            if(nom_objets[k] == m_inventaire[i].getChemin())
                ajouter = false;

        if(ajouter)
        {
            unsigned t = 0;
            for(t = 0 ; t < taille_objets.size() ; ++t)
                if(taille_objets[t] < m_inventaire[i].getTaille().y)
                {
                    for(unsigned T = 0 ; T < t ; ++T)
                    {
                        nom_objets_bis.push_back(nom_objets[T]);
                        taille_objets_bis.push_back(taille_objets[T]);
                    }

                    taille_objets_bis.push_back(m_inventaire[i].getTaille().y);
                    nom_objets_bis.push_back(m_inventaire[i].getChemin());


                    for(unsigned T = t ; T < taille_objets.size() ; ++T)
                    {
                        nom_objets_bis.push_back(nom_objets[T]);
                        taille_objets_bis.push_back(taille_objets[T]);
                    }

                    t = taille_objets.size() + 1;
                }

            if(t == taille_objets.size())
            {
                taille_objets.push_back(m_inventaire[i].getTaille().y);
                nom_objets.push_back(m_inventaire[i].getChemin());
            }
            else
            {
                nom_objets.clear();
                taille_objets.clear();
                for(unsigned T = 0 ; T < taille_objets_bis.size() ; ++T)
                {
                        nom_objets.push_back(nom_objets_bis[T]);
                        taille_objets.push_back(taille_objets_bis[T]);
                }
            }
        }
    }

    for(unsigned t = 0 ; t < 9 ; ++t)
    for(unsigned n = 0 ; n < nom_objets.size() ; ++n)
    for(unsigned i = 0 ; i < m_inventaire.size() ; ++i)
    if(m_inventaire[i].m_type == ordre_tri[t]
    && m_inventaire[i].m_equipe < 0
    && m_inventaire[i].getChemin() == nom_objets[n]
    && m_no_schema_bless != (int)i
    && m_no_schema_craft != (int)i
    && m_no_result_craft != (int)i
    && m_no_result_bless != (int)i)
    {
        for(int k = 0 ; k < 8 ; ++k)
            if(m_raccourcis[k].miracle == false
            && m_raccourcis[k].no == (int)i)
                new_raccourci[k] = inventaire_bis.size();

        inventaire_bis.push_back(m_inventaire[i]);
    }

    for(unsigned i = 0 ; i < m_inventaire.size() ; ++i)
        if(m_inventaire[i].m_equipe >= 0
        || m_no_schema_bless == (int)i
        || m_no_schema_craft == (int)i
        || m_no_result_craft == (int)i
        || m_no_result_bless == (int)i)
        {
            if(m_no_schema_craft == (int)i)
                new_no_schema_craft = inventaire_bis.size();
            if(m_no_schema_bless == (int)i)
                new_no_schema_bless = inventaire_bis.size();
            if(m_no_result_craft == (int)i)
                new_no_result_craft = inventaire_bis.size();
            if(m_no_result_bless == (int)i)
                new_no_result_bless = inventaire_bis.size();
            inventaire_bis.push_back(m_inventaire[i]);
        }


    m_inventaire.clear();
    for(unsigned i = 0 ; i < inventaire_bis.size() ; ++i)
    {
        m_inventaire.push_back(inventaire_bis[i]);
        m_inventaire.back().m_dejaTrie = (inventaire_bis[i].m_equipe >= 0);

        if(new_no_schema_craft == (int)i)
            m_inventaire.back().m_dejaTrie = true;
        if(new_no_schema_bless == (int)i)
            m_inventaire.back().m_dejaTrie = true;
        if(new_no_result_craft == (int)i)
            m_inventaire.back().m_dejaTrie = true;
        if(new_no_result_bless == (int)i)
            m_inventaire.back().m_dejaTrie = true;
    }

    for(int k = 0 ; k < 8 ; ++k)
        if(m_raccourcis[k].miracle == false)
            m_raccourcis[k].no = new_raccourci[k];

    m_no_schema_craft = new_no_schema_craft;
    m_no_schema_bless = new_no_schema_bless;
    m_no_result_craft = new_no_result_craft;
    m_no_result_bless = new_no_result_bless;

    int taille = 0;

    if(m_trier_en_hauteur)
        taille = TrierInventaire(&m_inventaire,m_classe.position_contenu_inventaire.h,m_trier_en_hauteur);
    else
        taille = TrierInventaire(&m_inventaire,m_classe.position_contenu_inventaire.w,m_trier_en_hauteur);

    m_trier_en_hauteur = !m_trier_en_hauteur;


    for(unsigned i = 0 ; i < m_inventaire.size() ; ++i)
        m_inventaire[i].m_dejaTrie = false;

    if(!m_trier_en_hauteur && taille > m_classe.position_contenu_inventaire.w)
        TrierInventaire(&m_inventaire,m_classe.position_contenu_inventaire.w,m_trier_en_hauteur);//,cout<<taille<<endl;

    else if(m_trier_en_hauteur && taille > m_classe.position_contenu_inventaire.h)
        TrierInventaire(&m_inventaire,m_classe.position_contenu_inventaire.h,m_trier_en_hauteur);//,cout<<taille<<endl;

}

bool Hero::PrendreEnMain(std::vector<Objet> *trader, bool craft, bool bless )
{
    if (trader)
    {
        if(eventManager->getPositionSouris().x > AutoScreenAdjust(14,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(34,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,260).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,290).y)
            m_defilement_trader--;

        if(eventManager->getPositionSouris().x > AutoScreenAdjust(14,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(34,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,442).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,472).y)
            m_defilement_trader++;
    }

    {
        for(int i = 0 ; i < 8 ; ++i)
            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_raccourcis[i].x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_raccourcis[i].x + m_classe.position_raccourcis[i].w,0).x
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_raccourcis[i].y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_raccourcis[i].y + m_classe.position_raccourcis[i].h).y)
            {
                if (m_objetEnMain >= 0)
                {
                    m_raccourcis[i].no = m_objetEnMain;
                    m_raccourcis[i].miracle = false;
                    RangerObjet(m_objetEnMain);
                    m_objetEnMain = -1;
                }
                else if(!m_raccourcis[i].miracle &&
                        m_raccourcis[i].no != m_no_schema_craft &&
                        m_raccourcis[i].no != m_no_result_craft &&
                        m_raccourcis[i].no != m_no_schema_bless &&
                        m_raccourcis[i].no != m_no_result_bless)
                {
                    m_objetEnMain = m_raccourcis[i].no;
                    m_raccourcis[i].no = -1;
                }
            }
        UpdateRaccourcis();
    }

    m_objetADeposer=-1;
    if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x
    && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_contenu_inventaire.x + m_classe.position_contenu_inventaire.w * 32,0).x
    && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y - 32).y
    && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y + m_classe.position_contenu_inventaire.h  * 32).y)
    {
        coordonnee caseVisee;
        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            caseVisee.x=(int)(((eventManager->getPositionSouris().x+8)-AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x)/32 - m_inventaire[m_objetEnMain].getTaille().x/2);
            caseVisee.y=(int)(((eventManager->getPositionSouris().y+8)-AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32).y)/32 - m_inventaire[m_objetEnMain].getTaille().y/2);
        }
        else
        {
            caseVisee.x=(int)(((eventManager->getPositionSouris().x)-AutoScreenAdjust(m_classe.position_contenu_inventaire.x,0).x)/32);
            caseVisee.y=(int)(((eventManager->getPositionSouris().y)-AutoScreenAdjust(0,m_classe.position_contenu_inventaire.y-32).y)/32);
        }

        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            int temp=-1;
            bool ok=true;

            for (int i=caseVisee.y;i<caseVisee.y+m_inventaire[m_objetEnMain].getTaille().y;++i)
                for (int j=caseVisee.x;j<caseVisee.x+m_inventaire[m_objetEnMain].getTaille().x;j++)
                    if (i<m_classe.position_contenu_inventaire.h&&j<m_classe.position_contenu_inventaire.w&&i>=0&&j>=0)
                    {
                        for (int z=0;z<(int)m_inventaire.size();z++)
                            for (int y=m_inventaire[z].getPosition().y;y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y;y++)
                                for (int x=m_inventaire[z].getPosition().x;x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x;x++)
                                    if (m_inventaire[z].m_equipe==-1
                                     && z != m_no_schema_craft && z != m_no_result_craft
                                     && z != m_no_schema_bless && z != m_no_result_bless)
                                        if (j==x&&i==y&&m_objetEnMain!=z)
                                        {
                                            if (temp!=z&&temp!=-1)
                                                ok=false;

                                            temp=z;
                                        }
                    }
                    else
                        ok=false;

            if (ok)
            {
                m_inventaire[m_objetEnMain].setPosition(caseVisee.x, caseVisee.y);

                m_inventaire[m_objetEnMain].JouerSon();
                m_inventaire[m_objetEnMain].m_equipe = -1;

                m_objetEnMain=-1;

                if (temp!=-1)
                    m_objetEnMain=temp;
                m_achat=false;
            }
        }
        else
        {
            for (int z=0;z<(int)m_inventaire.size();z++)
                if (m_inventaire[z].m_equipe==-1
                && z != m_no_schema_craft && z != m_no_result_craft
                && z != m_no_schema_bless && z != m_no_result_bless)// && m_inventaire[z].getPrix() >= 0)
                    if (caseVisee.x >= m_inventaire[z].getPosition().x
                      &&caseVisee.x <  m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x
                      &&caseVisee.y >= m_inventaire[z].getPosition().y
                      &&caseVisee.y <  m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y)
                    {
                        if (eventManager->getEvenement(Keyboard::LControl,EventKey) && m_inventaire[z].getPrix() >= 0)
                        {
                            if(trader)
                            if((trader->empty() && (trader == &m_coffre || craft || bless)) || !trader->empty())
                            {
                                m_inventaire[z].JouerSon();
                                if(trader != &m_coffre && !craft && !bless)
                                {
                                    m_argent+=m_inventaire[z].getPrix();
                                    moteurSons->JouerSon(configuration->sound_trade,coordonnee (0,0),0);
                                }
                                m_inventaire[z].m_equipe=-1;
                                AjouterObjetInventaire(m_inventaire[z],trader,m_classe.position_contenu_marchand,true);
                                delObjet(z);

                                if(trader == &m_coffre)
                                    AutoTrierCoffre();
                            }
                        }
                        else if (eventManager->getEvenement(Keyboard::LShift,EventKey) && m_inventaire[z].getPrix() >= 0)
                        {
                            if(craft || bless)
                            {
                                if(trader)
                                {
                                    std::string buf = m_inventaire[z].getChemin();
                                    for(unsigned a = 0, b = 0 ; a < m_inventaire.size() && b < 5 ; ++a)
                                    if(m_inventaire[a].getChemin() == buf)
                                    {
                                        m_inventaire[a].JouerSon();
                                        m_inventaire[a].m_equipe=-1;
                                        AjouterObjetInventaire(m_inventaire[a],trader,m_classe.position_contenu_marchand,true);
                                        delObjet(a);
                                        b++;
                                    }
                                }
                            }
                            else
                            {
                                m_objetADeposer = z;
                                return 1;
                            }
                        }
                        else
                            m_objetEnMain=z,m_achat=false;
                    }
        }

    }
    else if ( trader
            &&eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_contenu_marchand.x,0).x
            &&eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_contenu_marchand.x + m_classe.position_contenu_marchand.w * 32,0).x
            &&eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_contenu_marchand.y - 32).y
            &&eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_contenu_marchand.y - 32 + m_classe.position_contenu_marchand.h  * 32).y)
    {
        coordonnee caseVisee;
        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            caseVisee.x=(int)(((eventManager->getPositionSouris().x+8)-AutoScreenAdjust(m_classe.position_contenu_marchand.x,0).x)/32 - m_inventaire[m_objetEnMain].getTaille().x/2);
            caseVisee.y=(int)(((eventManager->getPositionSouris().y+8)-AutoScreenAdjust(0,m_classe.position_contenu_marchand.y-32).y)/32 - m_inventaire[m_objetEnMain].getTaille().y/2) + m_defilement_trader;
        }
        else
        {
            caseVisee.x=(int)(((eventManager->getPositionSouris().x)-AutoScreenAdjust(m_classe.position_contenu_marchand.x,0).x)/32);
            caseVisee.y=(int)(((eventManager->getPositionSouris().y + 32)-AutoScreenAdjust(0,m_classe.position_contenu_marchand.y).y)/32) + m_defilement_trader;
        }


        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size()
        && m_inventaire[m_objetEnMain].getPrix() >= 0)
        {
            m_inventaire[m_objetEnMain].JouerSon();

            if(trader != &m_coffre && !craft && !bless)
            {
                if (m_achat)
                    m_argent+=(int)((float)m_inventaire[m_objetEnMain].getPrix()*(10-(float)m_caracteristiques.charisme/100));
                else
                    m_argent+=m_inventaire[m_objetEnMain].getPrix();

                moteurSons->JouerSon(configuration->sound_trade,coordonnee (0,0),0);
            }

            m_inventaire[m_objetEnMain].m_equipe=-1;
            if (m_achat && m_inventaire[m_objetEnMain].m_type == CONSOMMABLE)
                delObjet(m_objetEnMain);
            else if(AjouterObjetInventaire(m_inventaire[m_objetEnMain],trader,m_classe.position_contenu_marchand,true))
                delObjet(m_objetEnMain);
            m_objetEnMain=-1;
            m_achat=false;

            if(trader == &m_coffre)
                AutoTrierCoffre();
        }
        else
        {
            for (int z=0;z<(int)trader->size();z++)
                if (caseVisee.x >= (*trader)[z].getPosition().x
                  &&caseVisee.x <  (*trader)[z].getPosition().x+(*trader)[z].getTaille().x
                  &&caseVisee.y >= (*trader)[z].getPosition().y
                  &&caseVisee.y <  (*trader)[z].getPosition().y+(*trader)[z].getTaille().y)
                    if ((int)((float)(*trader)[z].getPrix()*(10-(float)m_caracteristiques.charisme/100))<=m_argent
                     || trader == &m_coffre || craft || bless)
                    {
                        m_achat=true;

                        if (AjouterObjet((*trader)[z],!eventManager->getEvenement(Keyboard::LControl,EventKey)))
                        {
                            if(trader != &m_coffre && !craft && !bless)
                            {
                                m_argent-=(int)((float)(*trader)[z].getPrix()*(10-(float)m_caracteristiques.charisme/100));
                                moteurSons->JouerSon(configuration->sound_trade,coordonnee (0,0),0);
                            }
                            if ((*trader)[z].m_type!=CONSOMMABLE || trader == &m_coffre || craft || bless)
                                trader->erase(trader->begin()+z);

                            if(trader == &m_coffre)
                                AutoTrierCoffre();
                        }
                    }
                    else if((int)((float)(*trader)[z].getPrix()*(10-(float)m_caracteristiques.charisme/100))>m_argent)
                        moteurSons->JouerSon(configuration->sound_trade_i,coordonnee (0,0),0);
        }
    }
    else
    {
        if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.position_sac_inventaire.x,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.position_sac_inventaire.x + m_classe.position_sac_inventaire.w * 32,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.position_sac_inventaire.y).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.position_sac_inventaire.y + m_classe.position_sac_inventaire.h  * 20).y)
        {
            if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
            {
                m_objetADeposer=m_objetEnMain;

                m_objetEnMain=-1;
                m_achat=false;

                RecalculerCaracteristiques();

                return 1;
            }
        }
        else if (!craft && !bless)
        {
            bool equipe=false;

            for (int k=0;k<(int)m_classe.emplacements.size();k++)
            {
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.emplacements[k].position.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.emplacements[k].position.x + m_classe.emplacements[k].position.w,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.emplacements[k].position.y).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.emplacements[k].position.y + m_classe.emplacements[k].position.h).y)
                    equipe=true,Equiper(m_objetEnMain,k);
            }

            if (!equipe&&m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
            {
                if (m_inventaire[m_objetEnMain].m_equipe==-1)
                    RangerObjet(m_objetEnMain);
                else
                    m_objetEnMain=-1,m_achat=false;
            }
        }
    }

    RecalculerCaracteristiques();
    return 0;
}

void Hero::GererCraft(std::vector<Objet> *trader)
{
    if(m_no_schema_craft >= 0)
        m_classe.schema_craft.empty = false;
    else
        m_classe.schema_craft.empty = true;

     if(m_no_result_craft >= 0)
        m_classe.result_craft.empty = false;
    else
        m_classe.result_craft.empty = true;

    if(m_craft_time <= 0)
    {
        if(m_no_result_craft >= 0)
        {
            delObjet(m_no_schema_craft);
            m_no_schema_craft = -1;

            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.result_craft.position.x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.result_craft.position.x + m_classe.result_craft.position.w,0).x
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.result_craft.position.y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.result_craft.position.y + m_classe.result_craft.position.h).y
            && eventManager->getEvenement(Mouse::Left,EventClic))
            {
                if(m_objetEnMain < 0 && m_no_result_craft >= 0)
                    m_objetEnMain = m_no_result_craft, m_no_result_craft = -1;
            }
        }
        else
        {
            if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.schema_craft.position.x,0).x
            && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.schema_craft.position.x + m_classe.schema_craft.position.w,0).x
            && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.schema_craft.position.y).y
            && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.schema_craft.position.y + m_classe.schema_craft.position.h).y
            && eventManager->getEvenement(Mouse::Left,EventClic)
            && m_no_result_craft == -1)
            {
                if(m_objetEnMain >= 0)
                {
                    if(m_inventaire[m_objetEnMain].m_type == SCHEMA)
                    {
                        int buf = m_no_schema_craft;
                        m_no_schema_craft = m_objetEnMain;
                        m_objetEnMain = buf;

                        if(m_no_schema_craft >= 0 && m_no_schema_craft < (int)m_inventaire.size())
                            m_inventaire[m_no_schema_craft].setPosition(-10,-10);
                    }
                }
                else
                     m_objetEnMain = m_no_schema_craft, m_no_schema_craft = -1;
            }


            if(m_no_schema_craft >= 0)
            {
                bool ok = true;

                for(unsigned i = 0 ; i < m_inventaire[m_no_schema_craft].m_craft_ingredients.size() ; ++i)
                {
                    int nbr = 0;
                    for(unsigned j = 0 ; j < trader->size() ; ++j)
                        if((*trader)[j].getChemin() == m_inventaire[m_no_schema_craft].m_craft_ingredients[i].nom)
                            nbr ++;

                    if(nbr < m_inventaire[m_no_schema_craft].m_craft_ingredients[i].nombre)
                        ok = false;
                }

                if(ok)
                {
                    m_classe.button_craft.empty = true;
                    if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.button_craft.position.x,0).x
                    && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.button_craft.position.x + m_classe.button_craft.position.w,0).x
                    && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.button_craft.position.y).y
                    && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.button_craft.position.y + m_classe.button_craft.position.h).y
                    && eventManager->getEvenement(Mouse::Left,EventClic))
                    {
                        for(unsigned i = 0 ; i < m_inventaire[m_no_schema_craft].m_craft_ingredients.size() ; ++i)
                            for(unsigned k = 0 ; (int)k < m_inventaire[m_no_schema_craft].m_craft_ingredients[i].nombre ; ++k)
                            {
                                for(unsigned j = 0 ; j < (*trader).size() ; ++j)
                                    if((*trader)[j].getChemin() == m_inventaire[m_no_schema_craft].m_craft_ingredients[i].nom)
                                        trader->erase(trader->begin() + j), j = trader->size();
                            }

                        Objet result;
                        result.Charger(m_inventaire[m_no_schema_craft].m_craft_result, m_caracteristiques);
                        result.Generer(0);

                        moteurSons->JouerSon(configuration->sound_create_item,coordonnee (0,0),0);

                        m_inventaire.push_back(result);
                        m_no_result_craft = m_inventaire.size() - 1;

                        m_craft_time = m_inventaire[m_no_schema_craft].m_time;
                        m_craft_time_max = m_craft_time;
                    }
                }
                else
                    m_classe.button_craft.empty = false;
            }
            else
                m_classe.button_craft.empty = false;
        }
    }
}

void Hero::GererBless()
{
    if(m_no_schema_bless >= 0)
        m_classe.schema_craft.empty = false;
    else
        m_classe.schema_craft.empty = true;

     if(m_no_result_bless >= 0)
        m_classe.result_craft.empty = false;
    else
        m_classe.result_craft.empty = true;

    if(m_bless_time <= 0)
    {
        if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.schema_craft.position.x,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.schema_craft.position.x + m_classe.schema_craft.position.w,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.schema_craft.position.y).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.schema_craft.position.y + m_classe.schema_craft.position.h).y
        && eventManager->getEvenement(Mouse::Left,EventClic))
        {
            if(m_objetEnMain >= 0)
            {
                if(m_inventaire[m_objetEnMain].m_type == LITANIE)
                {
                    int buf = m_no_schema_bless;
                    m_no_schema_bless = m_objetEnMain;
                    m_objetEnMain = buf;

                    if(m_no_schema_bless >= 0 && m_no_schema_bless < (int)m_inventaire.size())
                        m_inventaire[m_no_schema_bless].setPosition(-10,-10);
                }
            }
            else
                 m_objetEnMain = m_no_schema_bless, m_no_schema_bless = -1;
        }

        if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.result_craft.position.x,0).x
        && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.result_craft.position.x + m_classe.result_craft.position.w,0).x
        && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.result_craft.position.y).y
        && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.result_craft.position.y + m_classe.result_craft.position.h).y
        && eventManager->getEvenement(Mouse::Left,EventClic))
        {
            if(m_objetEnMain >= 0)
            {
                if(m_inventaire[m_objetEnMain].m_type == ARME
                || m_inventaire[m_objetEnMain].m_type == ARMURE
                || m_inventaire[m_objetEnMain].m_type == GOLEM
                || m_inventaire[m_objetEnMain].m_type == JEWELERY)
                {
                    int buf = m_no_result_bless;
                    m_no_result_bless = m_objetEnMain;
                    m_objetEnMain = buf;

                    if(m_no_result_bless >= 0 && m_no_result_bless < (int)m_inventaire.size())
                        m_inventaire[m_no_result_bless].setPosition(-10,-10);
                }
            }
            else
                 m_objetEnMain = m_no_result_bless, m_no_result_bless = -1;
        }


        if( m_no_schema_bless >= 0 && m_no_result_bless >= 0)
        {
            bool ok = false;

            for(unsigned i = 0 ; i < m_inventaire[m_no_schema_bless].m_conditions.size() ; ++i)
            {
                if(m_inventaire[m_no_schema_bless].m_conditions[i].type == L_TYPE)
                    if(m_inventaire[m_no_result_bless].m_type == m_inventaire[m_no_schema_bless].m_conditions[i].valeur)
                        ok = true;
                if(m_inventaire[m_no_schema_bless].m_conditions[i].type == L_EMPLACEMENT)
                    for(unsigned j = 0 ; j < m_inventaire[m_no_result_bless].m_emplacement.size() ; ++j)
                        if(m_inventaire[m_no_result_bless].m_emplacement[j] == m_inventaire[m_no_schema_bless].m_conditions[i].valeur)
                            ok = true;
                if(m_inventaire[m_no_schema_bless].m_conditions[i].type == L_NOM)
                    if(m_inventaire[m_no_result_bless].getChemin() == m_inventaire[m_no_schema_bless].m_conditions[i].valeur_string)
                        ok = true;
            }

            if(m_inventaire[m_no_schema_bless].m_conditions.empty())
                ok =true;

            for(unsigned i = 0 ; i < m_inventaire[m_no_schema_bless].m_benedictions.size() ; ++i)
            {
                if(m_inventaire[m_no_schema_bless].m_benedictions[i].type == DEGATS_SUPP
                && m_inventaire[m_no_result_bless].m_type != ARME
                && m_inventaire[m_no_result_bless].m_type != GOLEM)
                    ok = false;
                if(m_inventaire[m_no_schema_bless].m_benedictions[i].type == DEGATS_TEMPS_SUPP
                && m_inventaire[m_no_result_bless].m_type != ARME
                && m_inventaire[m_no_result_bless].m_type != GOLEM)
                    ok = false;
                if(m_inventaire[m_no_schema_bless].m_benedictions[i].type == ARMURE_SUPP
                && m_inventaire[m_no_result_bless].m_type != ARMURE
                && m_inventaire[m_no_result_bless].m_type != GOLEM)
                    ok = false;
            }

            if(m_inventaire[m_no_result_bless].m_nbr_bless <= 0)
                ok = false;


            if(ok)
            {
                m_classe.button_craft.empty = true;
                if(eventManager->getPositionSouris().x > AutoScreenAdjust(m_classe.button_craft.position.x,0).x
                && eventManager->getPositionSouris().x < AutoScreenAdjust(m_classe.button_craft.position.x + m_classe.button_craft.position.w,0).x
                && eventManager->getPositionSouris().y > AutoScreenAdjust(0,m_classe.button_craft.position.y).y
                && eventManager->getPositionSouris().y < AutoScreenAdjust(0,m_classe.button_craft.position.y + m_classe.button_craft.position.h).y
                && eventManager->getEvenement(Mouse::Left,EventClic))
                {
                    for(unsigned i = 0 ; i < m_inventaire[m_no_schema_bless].m_benedictions.size() ; ++i)
                        m_inventaire[m_no_result_bless].AddBenediction(m_inventaire[m_no_schema_bless].m_benedictions[i]);

                    for(unsigned i = 0 ; i < m_inventaire[m_no_schema_bless].m_chemin_miracles.size() ; ++i)
                    {
                        m_inventaire[m_no_result_bless].m_miracles_benedictions.push_back(Miracle (m_inventaire[m_no_schema_bless].m_chemin_miracles[i], m_caracteristiques,0));
                        if(m_inventaire[m_no_result_bless].m_useMiracle)
                            m_inventaire[m_no_result_bless].m_miracle.Concatenencer(m_inventaire[m_no_schema_bless].m_chemin_miracles[i],m_caracteristiques,0);
                        else
                            m_inventaire[m_no_result_bless].m_miracle.Charger(m_inventaire[m_no_schema_bless].m_chemin_miracles[i],m_caracteristiques,0);
                        m_inventaire[m_no_result_bless].m_useMiracle = true;
                        m_inventaire[m_no_result_bless].m_chemin_miracles.push_back(m_inventaire[m_no_schema_bless].m_chemin_miracles[i]);
                    }


                    if(m_inventaire[m_no_result_bless].m_benedictions.size() + m_inventaire[m_no_result_bless].m_miracles_benedictions.size() * 3 >= 8
                    && m_inventaire[m_no_result_bless].getRarete() < SANCTIFIE)
                        m_inventaire[m_no_result_bless].setRarete(SANCTIFIE);
                    if(m_inventaire[m_no_result_bless].m_benedictions.size() + m_inventaire[m_no_result_bless].m_miracles_benedictions.size() * 3 >= 4
                    && m_inventaire[m_no_result_bless].getRarete() < SACRE)
                        m_inventaire[m_no_result_bless].setRarete(SACRE);
                    if(m_inventaire[m_no_result_bless].m_benedictions.size() + m_inventaire[m_no_result_bless].m_miracles_benedictions.size() * 3 >= 2
                    && m_inventaire[m_no_result_bless].getRarete() < BENI)
                        m_inventaire[m_no_result_bless].setRarete(BENI);
                    if(m_inventaire[m_no_result_bless].m_benedictions.size() + m_inventaire[m_no_result_bless].m_miracles_benedictions.size() * 3 >= 1
                    && m_inventaire[m_no_result_bless].getRarete() < BONNEFACTURE)
                        m_inventaire[m_no_result_bless].setRarete(BONNEFACTURE);

                    m_inventaire[m_no_result_bless].m_nbr_bless --;
                    m_bless_time = m_inventaire[m_no_schema_bless].m_time;
                    m_bless_time_max = m_bless_time;
                    delObjet(m_no_schema_bless);
                    m_no_schema_bless = -1;

                    moteurSons->JouerSon(configuration->sound_bless_item,coordonnee (0,0),0);
                }
            }
            else
                m_classe.button_craft.empty = false;
        }
        else
            m_classe.button_craft.empty = false;
    }
}

bool Hero::PossibleEquiper(Objet &objet, int emplacement)
{
    if (objet.Utilisable(m_caracteristiques,m_cheminClasse))
        for (int i=0;i<(int)objet.m_emplacement.size();++i)
            if (objet.m_emplacement[i]==m_classe.emplacements[emplacement].emplacement)
                return true;
    return false;
}

bool Hero::PossibleEquiper(int numero, int emplacement)
{
    if (numero >= 0 && numero < (int)m_inventaire.size())
        return PossibleEquiper(m_inventaire[numero], emplacement);

    return false;
}

void Hero::delObjet(int numero)
{
    if(numero >=0 && numero < (int) m_inventaire.size())
    {
        string temp = m_inventaire[numero].getNom();
        m_inventaire.erase(m_inventaire.begin()+numero);

        if(m_no_schema_bless > numero)
            m_no_schema_bless--;
        if(m_no_result_bless > numero)
            m_no_result_bless--;
        if(m_no_schema_craft > numero)
            m_no_schema_craft--;
        if(m_no_result_craft > numero)
            m_no_result_craft--;

        for (int i=0;i<8;++i)
        {
            if (numero < m_raccourcis[i].no && m_raccourcis[i].no >= 0 && !m_raccourcis[i].miracle)
                m_raccourcis[i].no --;

            else if (numero == m_raccourcis[i].no && !m_raccourcis[i].miracle)
            {
                m_raccourcis[i].no = -1;

                for (int j=0;j<(int)m_inventaire.size();j++)
                    if (m_inventaire[j].getNom()==temp)
                        m_raccourcis[i].no = j;
            }
        }
    }
    UpdateRaccourcis();
}

void Hero::UpdateRaccourcis()
{
    for (int i=0;i<8;++i)
    if(!m_raccourcis[i].miracle)
    {
        m_raccourcis[i].nbr = 0;
        if (m_raccourcis[i].no >= 0 && m_raccourcis[i].no < (int)m_inventaire.size())
        if(m_inventaire[m_raccourcis[i].no].m_type == CONSOMMABLE)
        {
            for (int j=0;j<(int)m_inventaire.size();j++)
                if (m_inventaire[j].getChemin() == m_inventaire[m_raccourcis[i].no].getChemin())
                    m_raccourcis[i].nbr ++;
        }
    }
}

const std::string &Hero::getNomObjet(int numero)
{
    if(numero >= 0 && numero < (int)m_inventaire.size())
        return m_inventaire[numero].getChemin();
    else
        return configuration->getText(0,0);
}
int Hero::getNombreObjet()
{
    return m_inventaire.size();
}

bool Hero::UtiliserObjet(int numero)
{
    if (numero >= 0 && numero < (int)m_inventaire.size())
    {
        if (m_inventaire[numero].m_type == ARME || m_inventaire[numero].m_type == ARMURE || m_inventaire[numero].m_type == JEWELERY)
        {
            bool continuer = true;
            for (int j=0;j<(int)m_classe.emplacements.size() && continuer;j++)
                for (int i=0;i<(int)m_inventaire[numero].m_emplacement.size() && continuer;++i)
                    if (m_inventaire[numero].m_emplacement[i] == m_classe.emplacements[j].emplacement && continuer)
                        Equiper(numero, j),continuer=false;

            ChargerModele();
        }
        else if (m_inventaire[numero].m_type == CONSOMMABLE || m_inventaire[numero].m_type == GOLEM)
        {
            bool ok = true;

            for(unsigned i = 0 ; i < m_items_cooldown.size() ; ++i)
                if(m_items_cooldown[i].name == m_inventaire[numero].getChemin())
                    ok = false;

            if(ok)
            {
                m_classe.miracles.push_back(Miracle (m_inventaire[numero].m_miracle.m_chemin,m_caracteristiques,0));

                m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());
                m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                m_personnage.m_miracleEnCours.back().m_source = &m_inventaire[numero];

                m_personnage.m_miracleEnCours.back().m_modele = (int)m_classe.miracles.size()-1;

                m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x+cos(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;
                m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y+sin(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;

                m_items_cooldown.push_back(Item_Used ());
                m_items_cooldown.back().name = m_inventaire[numero].getChemin();
                m_items_cooldown.back().max_time = m_classe.miracles.back().m_cooldown;

                if(m_inventaire[numero].m_type == CONSOMMABLE)
                    delObjet(numero);
            }


            return 1;
        }
        else if (m_inventaire[numero].m_type == DOCUMENT)
        {
            bool add = true;
            for(unsigned i =0 ; i< m_docs.size() ; ++i)
                if(m_docs.back().m_chemin == m_inventaire[numero].getChemin())
                    add = false;

            if(add)
            {
                m_docs.push_back(Document ());
                m_docs.back().m_chemin = m_inventaire[numero].getChemin();
                m_docs.back().m_nom = m_inventaire[numero].getNom();
                std::string temp = configuration->getText(7,m_inventaire[numero].m_doc);
                temp = DecouperTexte(temp,m_classe.position_contenu_docs.w,16);
                m_docs.back().m_description = temp;
                m_docs.back().GenerateDescription();
                newDoc = true;

                moteurSons->JouerSon(configuration->sound_book,coordonnee (0,0),0);
            }

            delObjet(numero);
        }
    }
    RecalculerCaracteristiques();

    return 0;
}

bool Hero::Equiper(int numero, int emplacement, bool playsound)
{
    int ancienEquipe=-1;
    bool ok=true;

    if (numero>=0 && numero<(int)m_inventaire.size())
    {
        ok=true;

        if (PossibleEquiper(numero, emplacement))
        {
            for (int i=0;i<(int)m_inventaire.size();++i)
            {
                if (m_inventaire[i].m_equipe==emplacement
                 &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
                    if (numero!=i)
                        ancienEquipe=i;

                for (int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if (numero!=i)
                        if (m_inventaire[i].m_equipe>=0
                         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
                            if (m_inventaire[i].m_emplacementImpossible[j]==m_classe.emplacements[emplacement].emplacement)
                                ok=false;


                if (numero>=0&&numero<(int)m_inventaire.size())
                    for (int j=0;j<(int)m_inventaire[numero].m_emplacementImpossible.size();j++)
                        if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size()
                         &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
                            if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==m_inventaire[numero].m_emplacementImpossible[j])
                                ok=false;
            }

            if (ok)
                if (numero>=0&&numero<(int)m_inventaire.size())
                {
                    ok=false;
                    for (int i=0;i<(int)m_inventaire[numero].m_emplacement.size();++i)
                        if (m_inventaire[numero].m_emplacement[i]==m_classe.emplacements[emplacement].emplacement)
                            ok=true;

                    if (ok)
                    {
                        m_inventaire[numero].m_equipe=emplacement;
                        if(m_classe.emplacements[emplacement].weaponsSet)
                            m_inventaire[numero].m_equipe_set=m_weaponsSet;
                        else
                            m_inventaire[numero].m_equipe_set=-1;
                    }

                    if (ancienEquipe>=0&&ancienEquipe<(int)m_inventaire.size())
                        m_inventaire[ancienEquipe].m_equipe=-1;
                }

            if (ok)
            {
                if(playsound)
                    m_inventaire[numero].JouerSon();
                if (m_objetEnMain>=0)
                    m_objetEnMain=ancienEquipe;
                else
                {
                    if(!RangerObjet(ancienEquipe))
                    {
                        m_inventaire[ancienEquipe].m_equipe = emplacement;
                        m_inventaire[numero].m_equipe = -1;
                    }
                }
                m_achat=false;
            }
        }
    }
    else
    {
        for (int i=0;i<(int)m_inventaire.size();++i)
            if (m_inventaire[i].m_equipe==emplacement
             &&(m_inventaire[i].m_equipe_set == m_weaponsSet || m_inventaire[i].m_equipe_set == -1))
                m_objetEnMain=i, m_achat=false;
    }

    RecalculerCaracteristiques();
    return 0;
}

bool Hero::RangerObjet(int numero)
{
    bool continuer = true;
    bool ranger = false;
    if (numero>=0&&numero<(int)m_inventaire.size())
    {
        if (m_inventaire[numero].m_equipe==-1)
        {
            for (int y=0;y<m_classe.position_contenu_inventaire.h;y++)
                for (int x=0;x<m_classe.position_contenu_inventaire.w && continuer;x++)
                {

                    bool ajouter=true;
                    for (int h=0;h<m_inventaire[numero].getTaille().y;h++)
                        for (int w=0;w<m_inventaire[numero].getTaille().x;w++)
                            if (x+w<m_classe.position_contenu_inventaire.w && y+h<m_classe.position_contenu_inventaire.h)
                            {
                                for (int j=0;j<(int)m_inventaire.size();j++)
                                    if (m_inventaire[j].m_equipe<0 && j!=m_objetEnMain && j!=numero)
                                        for (int Y=0;Y<m_inventaire[j].getTaille().y;Y++)
                                            for (int X=0;X<m_inventaire[j].getTaille().x;X++)
                                                if (m_inventaire[j].getPosition().x+X==x+w && m_inventaire[j].getPosition().y+Y==y+h)
                                                    ajouter = false;
                            }
                            else
                                ajouter = false;

                    if (ajouter)
                    {
                        continuer = false;
                        m_inventaire[numero].setPosition(x,y);
                        ranger = true;
                    }
                }
        }
    }
    else
        return true;

    return ranger;
}

/*void Hero::InfligerDegats(float degats)
{
    float temp = degats;
    degats -= (float)m_caracteristiques.armure/50;
    if (degats < 0)
        degats = 0;
    if (degats > temp)
        degats = temp;

    m_caracteristiques.vie -= degats;
    m_personnage.InfligerDegats(degats);
}*/

void Hero::RegenererVie(float vie)
{
    Caracteristique temp=m_personnage.getCaracteristique();

    if (temp.reserveVie < 0)
        temp.reserveVie = 0;

    if (temp.vie > m_caracteristiques.maxVie  - temp.reserveVie )
    {
        temp.vie -= (temp.vie - m_caracteristiques.maxVie + temp.reserveVie)*vie/10;
        if (temp.vie < m_caracteristiques.maxVie - temp.reserveVie)
            temp.vie = m_caracteristiques.maxVie - temp.reserveVie;
    }
    else
        temp.vie += ((float)m_caracteristiques.maxVie*(vie/100)) + m_caracteristiques.regenVie * ((float)m_caracteristiques.maxVie*(vie/100));

    if (temp.vie > (m_caracteristiques.maxVie - temp.reserveVie) * 2)
        temp.vie = (m_caracteristiques.maxVie - temp.reserveVie) * 2;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.vie = temp.vie;
    m_caracteristiques.reserveVie = temp.reserveVie;
}

void Hero::RegenererFoi(float foi)
{
    Caracteristique temp=m_personnage.getCaracteristique();

    if (temp.reserveFoi < 0)
        temp.reserveFoi = 0;

    if (temp.foi > m_caracteristiques.maxFoi - temp.reserveFoi)
    {
        temp.foi -= (temp.foi - m_caracteristiques.maxFoi + temp.reserveFoi)*foi/50;
        if (temp.foi < m_caracteristiques.maxFoi - temp.reserveFoi)
            temp.foi = m_caracteristiques.maxFoi - temp.reserveFoi;
    }
    else
        temp.foi += ((float)m_caracteristiques.maxFoi*(foi/50)) + m_caracteristiques.regenFoi * ((float)m_caracteristiques.maxFoi*(foi/50));

    if (temp.foi > (m_caracteristiques.maxFoi - temp.reserveFoi) * 2)
        temp.foi = (m_caracteristiques.maxFoi - temp.reserveFoi) * 2;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.foi = temp.foi;
    m_caracteristiques.reserveFoi = temp.reserveFoi;
}
void Hero::RegenererMiracles(float time)
{
    for(unsigned i = 0 ; i < m_classe.miracles.size() ; ++i)
    {
        m_classe.miracles[i].m_cur_time += time;
        if(m_classe.miracles[i].m_cur_time > m_classe.miracles[i].m_cooldown)
            m_classe.miracles[i].m_cur_time = m_classe.miracles[i].m_cooldown;
    }

    for(unsigned i = 0 ; i < m_items_cooldown.size() ; ++i)
    {
        m_items_cooldown[i].time += time;
        if(m_items_cooldown[i].time >= m_items_cooldown[i].max_time)
            m_items_cooldown.erase(m_items_cooldown.begin() + i--);
    }
}

void Hero::setChercherSac(coordonnee a)
{
    m_chercherSac=a;
}
void Hero::setSacVise(coordonnee a)
{
    m_sacVise=a;
}

Lumiere Hero::getPorteeLumineuse()
{
    Lumiere moyenne;

    moyenne.rouge=255;
    moyenne.vert=255;
    moyenne.bleu=255;
    moyenne.intensite=192;

    /*for(int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        moyenne.rouge=(moyenne.rouge*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().rouge*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);
        moyenne.vert=(moyenne.vert*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().vert*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);
        moyenne.bleu=(moyenne.bleu*moyenne.intensite+m_modelePersonnage[i].getPorteeLumineuse().bleu*m_modelePersonnage[i].getPorteeLumineuse().intensite)/(m_modelePersonnage[i].getPorteeLumineuse().intensite+moyenne.intensite);

        moyenne.intensite+=m_modelePersonnage[i].getPorteeLumineuse().intensite;

        if(moyenne.intensite>255)
            moyenne.intensite=255;
        if(moyenne.intensite<0)
            moyenne.intensite=0;
    }*/

    return moyenne;
}

coordonnee Hero::getChercherSac()
{
    return m_chercherSac;
};
coordonnee Hero::getSacVise()
{
    return m_sacVise;
};
#endif

