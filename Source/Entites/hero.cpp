

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
                if ((!infini && y<taille.h) || (infini))
                {
                    bool ajouter=true;
                    for (int h=0;h<inventaire->back().getTaille().y;h++)
                        for (int w=0;w<inventaire->back().getTaille().x;w++)
                            if (x+w<taille.w && ((infini) || (!infini && y+h<taille.h )))
                            {
                                for (int j=0;j<(int)inventaire->size()-1;j++)
                                    if ((*inventaire)[j].m_equipe<0)
                                        for (int Y=0;Y<(*inventaire)[j].getTaille().y;Y++)
                                            for (int X=0;X<(*inventaire)[j].getTaille().x;X++)
                                                if ((*inventaire)[j].getPosition().x+X==x+w && (*inventaire)[j].getPosition().y+Y==y+h)
                                                    ajouter = false;
                            }
                            else
                                ajouter = false;

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
    ///Je donnes des valeur à mes variables juste pour les tests
    m_personnage.setEtat(ARRET);

   /* if (rand()%2==1)
        m_cheminClasse="Data/Entities/Heroes/Crusader.class.hs";
    else
        m_cheminClasse="Data/Entities/Heroes/Concubine.class.hs";*/
    m_cheminClasse = configuration->player_class;

    m_chercherSac.x=-1;
    m_chercherSac.y=-1;

    coordonnee position;
    position.x=-10000;
    position.y=-10000;

    m_personnage.setCoordonnee(position);

    Lumiere lumiere;
    lumiere.intensite=192;
    lumiere.rouge=255;
    lumiere.vert=255;
    lumiere.bleu=255;
    lumiere.hauteur=20;

    m_modelePersonnage[0].setPorteeLumineuse(lumiere);

    lumiere.intensite=0;

    for (int i=1;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_modelePersonnage[i].setPorteeLumineuse(lumiere);

    m_personnage.setPorteeLumineuse(lumiere);
    m_personnage.m_friendly = 1;

    Caracteristique temp;

    temp.vie                = 100;
    temp.maxVie             = 100;
    temp.reserveVie         = 0;
    temp.regenVie           = 0;

    temp.foi                = 100;
    temp.maxFoi             = 100;
    temp.reserveFoi         = 0;
    temp.regenFoi           = 0;

    temp.volVie             = 0;
    temp.volFoi             = 0;

    temp.vitesse            = 1;
    temp.pointAme           = 0;
    temp.ancienPointAme     = 0;
    temp.positionAncienAme  = 0;
    temp.niveau             = 1;
    temp.nom                = "Héro";

    temp.force              = 4;
    temp.dexterite          = 4;
    temp.vitalite           = 4;
    temp.piete              = 4;
    temp.charisme           = 4;
    temp.pts_restant        = 0;
    temp.miracles_restant   = 0;

    temp.modificateurTaille = 1;

    m_personnage.setCaracteristique(temp);
    m_personnage.m_friendly = true;

    m_monstreVise=-1;

    m_objetEnMain=-1;
    m_achat=false;

    RecalculerCaracteristiques();

    miracleEnCours=0;

    m_miracleEnMain = -1;

    m_cas=0;

    m_argent=0;

    m_angleFleche = 0;


    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_cheminModeleNouveau[i]="",m_cheminModele[i]="";

    m_contenuSave.push_back(configuration->chemin_temps+"hero.sav.txt");

    m_personnage.m_light=moteurGraphique->LightManager->Add_Dynamic_Light(sf::Vector2f(m_personnage.getCoordonnee().x,m_personnage.getCoordonnee().y),512,2048,16,sf::Color(255,255,255));

    m_defilement_trader=0;

    m_objets_raccourcis[0] = -1;
    m_objets_raccourcis[1] = -1;
    m_objets_raccourcis[2] = -1;
    m_objets_raccourcis[3] = -1;

    m_miracles_raccourcis[0] = -1;
    m_miracles_raccourcis[1] = -1;
    m_miracles_raccourcis[2] = -1;
    m_miracles_raccourcis[3] = -1;

    m_queteSelectionnee = -1;
    m_personnage.m_miracleALancer = -1;
}

Hero::~Hero()
{
    if (configuration->debug)
        console->Ajouter("Destruction du héro ...");

    m_inventaire.clear();
    if (configuration->debug)
        console->Ajouter("Destruction de l'inventaire.");
    m_contenuSave.clear();
    if (configuration->debug)
        console->Ajouter("Destruction de la save.");

    console->Ajouter("Destruction du héro terminée.");
}

void Hero::Sauvegarder()
{
    console->Ajouter("");
    console->Ajouter("Sauvegarde du héro...");

    ofstream fichier((configuration->chemin_temps+"hero.sav.txt").c_str(), ios::out | ios::trunc | ios::binary);

    if (fichier)
    {
        if (configuration->debug)
            console->Ajouter("Ouverture du fichier.");

        fichier<<m_personnage.getNom().c_str()<<" "<<endl;
        fichier<<m_cheminClasse<<" "<<endl;

        fichier<<m_personnage.getCaracteristique().vitesse<<endl;
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

        fichier<<m_objets_raccourcis[0]<<endl;
        fichier<<m_objets_raccourcis[1]<<endl;
        fichier<<m_objets_raccourcis[2]<<endl;
        fichier<<m_objets_raccourcis[3]<<endl;

        fichier<<m_miracles_raccourcis[0]<<endl;
        fichier<<m_miracles_raccourcis[1]<<endl;
        fichier<<m_miracles_raccourcis[2]<<endl;
        fichier<<m_miracles_raccourcis[3]<<endl;

        fichier<<m_personnage.m_miracleALancer<<endl;

        if (configuration->debug)
            console->Ajouter("/Ecriture des caracterstiques.");

        for (int i=0;i<(int)m_inventaire.size();++i)
            m_inventaire[i].SauvegarderTexte(&fichier);

        if (configuration->debug)
            console->Ajouter("/Ecriture des objets.");

        fichier<<'$'<<endl;


        for (int i=0;i<(int)m_lvl_miracles.size();++i)
            fichier<<"* l"<<m_lvl_miracles[i]<<" $ ";

        if (configuration->debug)
            console->Ajouter("/Ecriture des miracles.");

        fichier<<'$'<<endl;

        for (int i=0;i<(int)m_quetes.size();++i)
            m_quetes[i].SauvegarderTexte(&fichier);

        if (configuration->debug)
            console->Ajouter("/Ecriture des quetes.");

        fichier<<'$'<<endl;

        fichier.close();

        console->Ajouter("Sauvegarde du héro terminée !");

    }
    else
        console->Ajouter("Impossible de sauvegarder le héro !",1);

    cDAT fichierSave;

    fichierSave.Create(m_contenuSave, configuration->chemin_saves+"hero.sav.hs");
    string buffer=configuration->chemin_temps+"hero.sav.txt";
    //remove(buffer.c_str());
    //m_contenuSave.clear();

}



void Hero::Charger()
{
    console->Ajouter("Chargement du hero.");
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_cheminModele[i]="";

    bool nouveau=true;
    string chemin;

    cDAT reader;
    if (reader.Read(configuration->chemin_saves+"hero.sav.hs"))
    {
        ifstream* fichier=reader.GetInfos(configuration->chemin_temps+"hero.sav.txt");
        //fichier.open((configuration->chemin_saves+"hero.sav.hs").c_str(), ios::in | ios::binary);
        if (fichier)
        {
            char caractere;
            Caracteristique charTemp;
            charTemp=m_personnage.getCaracteristique();

            *fichier>>charTemp.nom;

            *fichier>>m_cheminClasse;

            *fichier>>charTemp.vitesse;
            *fichier>>charTemp.pointAme;
            *fichier>>charTemp.niveau;

            *fichier>>charTemp.force;
            *fichier>>charTemp.dexterite;
            *fichier>>charTemp.vitalite;
            *fichier>>charTemp.piete;
            *fichier>>charTemp.charisme;
            *fichier>>charTemp.pts_restant;
            *fichier>>charTemp.miracles_restant;

            *fichier>>m_argent;

            *fichier>>m_objets_raccourcis[0];
            *fichier>>m_objets_raccourcis[1];
            *fichier>>m_objets_raccourcis[2];
            *fichier>>m_objets_raccourcis[3];

            *fichier>>m_miracles_raccourcis[0];
            *fichier>>m_miracles_raccourcis[1];
            *fichier>>m_miracles_raccourcis[2];
            *fichier>>m_miracles_raccourcis[3];

            *fichier>>m_personnage.m_miracleALancer;

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

                    if (m_inventaire.back().m_equipe>=0&&m_inventaire.back().m_type==ARME)
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
                console->Ajouter("/Lectures des quêtes.");

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



            m_personnage.setCaracteristique(charTemp);
            nouveau=false;
        }
        fichier->close();
    }
    //}
    //}
    // closedir(repertoire);

    m_classe.Charger(m_cheminClasse, m_lvl_miracles, m_caracteristiques);

    m_lvl_miracles.resize(m_classe.miracles.size(),0);

    ChargerModele();

    RecalculerCaracteristiques();
    RecalculerCaracteristiques();


    if (nouveau)
    {
        sf::Color color;
        color.r=255;
        color.g=255;
        color.b=255;

        Caracteristique charTemp;
        charTemp=m_personnage.getCaracteristique();

        charTemp.force=m_classe.caracteristique.force;
        charTemp.vitalite=m_classe.caracteristique.vitalite;
        charTemp.dexterite=m_classe.caracteristique.dexterite;
        charTemp.piete=m_classe.caracteristique.piete;
        charTemp.charisme=m_classe.caracteristique.charisme;

        m_personnage.setCaracteristique(charTemp);

        RecalculerCaracteristiques();

        for (int i=0;i<(int)m_classe.equipementParDefaut.size();++i)
        {
            Objet temp;

            temp.Charger(m_classe.equipementParDefaut[i],m_caracteristiques);
            temp.Generer(1);
            AjouterObjet(temp);
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



void Hero::ChargerModele(bool tout)
{
    m_personnage.setPose(0);
    m_cas=0;

    Lumiere color[NOMBRE_MORCEAU_PERSONNAGE];

    int nombreArme=0;
    m_weaponMiracle=-1;

    std::vector <Miracle> miraclesLances;
    for (unsigned i = 0 ; i < m_personnage.m_miracleEnCours.size() ; ++i)
        miraclesLances.push_back(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele]);

    m_classe.miracles.clear();
    m_classe.Charger(m_cheminClasse, m_lvl_miracles, m_caracteristiques);

    for (int i=0;i<(int)m_inventaire.size();++i)
    {
        if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
        {
            m_inventaire[i].ChargerMiracle(m_caracteristiques);

            if ((m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)||(m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER&&m_inventaire[i].m_type==ARME))
                nombreArme++;

            if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL&&m_inventaire[i].m_type==ARME)
                for (int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if (m_inventaire[i].m_emplacementImpossible[j]==BOUCLIER)
                        m_cas=2;

            if (m_inventaire[i].m_useMiracle)
                if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==ARME_PRINCIPAL||m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER)
                {
                    m_classe.miracles.push_back(m_inventaire[i].m_miracle);

                    m_weaponMiracle=m_classe.miracles.size()-1;
                }
        }
    }


    if (nombreArme==2)
        m_cas=1;


    bool pasEquipe[NOMBRE_MORCEAU_PERSONNAGE];

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        pasEquipe[i]=true,m_cheminModeleNouveau[i]="";
        if (tout)
            m_cheminModele[i]="";
    }


    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0)
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
                    m_cheminModeleNouveau[m_inventaire[i].m_emplacementImageHero[temp]]=m_inventaire[i].m_cheminImageHero[temp];
                    color[m_inventaire[i].m_emplacementImageHero[temp]].rouge=m_inventaire[i].m_color.r;
                    color[m_inventaire[i].m_emplacementImageHero[temp]].vert=m_inventaire[i].m_color.g;
                    color[m_inventaire[i].m_emplacementImageHero[temp]].bleu=m_inventaire[i].m_color.b;
                    color[m_inventaire[i].m_emplacementImageHero[temp]].intensite=255;
                    pasEquipe[m_inventaire[i].m_emplacementImageHero[temp]]=false;
                }
            }
        }

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        if (m_cheminModeleNouveau[i]!="" && m_cheminModeleNouveau[i]!=m_cheminModele[i])
            m_modelePersonnage[i].Charger(m_classe.chemin_modele+m_cheminModeleNouveau[i]),m_modelePersonnage[i].setPorteeLumineuse(color[i]);
        else if (m_cheminModeleNouveau[i]=="")
            m_modelePersonnage[i].Reinitialiser();

        m_cheminModele[i]=m_cheminModeleNouveau[i];
    }

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE ;++i)
    {
        if (pasEquipe[i] && !m_classe.modeleNu[i][m_cas].empty())
        {
            if (m_cheminModele[i]!=m_classe.modeleNu[i][m_cas])
            {
                m_modelePersonnage[i].Charger(m_classe.chemin_modele+m_classe.modeleNu[i][m_cas]);
                m_cheminModele[i]=m_classe.modeleNu[i][m_cas];
            }
        }
    }

    for (unsigned i = 0 ; i < m_personnage.m_miracleEnCours.size() ; ++i)
    {
        m_personnage.m_miracleEnCours[i].m_modele = m_classe.miracles.size();
        m_classe.miracles.push_back(miraclesLances[i]);
    }

    CalculerOrdreAffichage();

}

void Hero::CalculerOrdreAffichage()
{

    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        m_ordreAffichage[i]=-1;

    int ordre;


    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
    {
        if (m_cheminModele[i] != "")
        {
            ordre = m_personnage.getOrdre(&m_modelePersonnage[i]);
            if (ordre!=-10)
                m_ordreAffichage[(int)(NOMBRE_MORCEAU_PERSONNAGE/2+ordre)]=i;
        }
    }

}

void Hero::Afficher(coordonnee dimensionsMap)
{
    for (int i=0;i<NOMBRE_MORCEAU_PERSONNAGE;++i)
        if (m_ordreAffichage[i]!=-1)
        {
            //m_personnage.setPorteeLumineuse(m_modelePersonnage[ordreAffichage[i]].getPorteeLumineuse());
            m_personnage.Afficher(dimensionsMap,&m_modelePersonnage[m_ordreAffichage[i]], false, m_ordreAffichage[i]!=0);
        }

    AfficherRaccourcis();
}

void Hero::AfficherCaracteristiques(float decalage)
{
    sf::Sprite sprite;
    sprite.SetImage(*moteurGraphique->getImage(m_classe.plus_button.image));
    sprite.Resize(m_classe.plus_button.position.w*configuration->Resolution.w/800, m_classe.plus_button.position.h*configuration->Resolution.h/600);
    sprite.SetX(m_classe.plus_button.position.x*configuration->Resolution.x/800);


    sf::Text string;
    string.SetFont(moteurGraphique->m_font);
    string.SetSize(16*configuration->Resolution.h/600);

    string.SetColor(sf::Color(255,255,255));


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.niveau;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+22*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    {
        string.SetString(m_caracteristiques.nom.c_str());
        string.SetX((1*configuration->Resolution.w/800)+62*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    {
        std::ostringstream  buf;
        buf<<configuration->getText(0,3)<<" : "<<(int)m_caracteristiques.vie<<" / "<<(int)m_caracteristiques.maxVie;
        string.SetString(buf.str());
    }

    if ((string.GetRect().Right-string.GetRect().Left)>104)
    {
        string.SetSize(14*configuration->Resolution.h/600);
        if ((string.GetRect().Right-string.GetRect().Left)>104)
        {
            std::ostringstream  buf;
            buf<<(int)m_caracteristiques.vie<<" / "<<(int)m_caracteristiques.maxVie;
            string.SetString(buf.str());
        }
    }

    string.SetX((178*configuration->Resolution.w/800)+54*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if (m_caracteristiques.maxVie!=m_personnage.getCaracteristique().maxVie)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    string.SetSize(16*configuration->Resolution.h/600);


    {
        std::ostringstream  buf;
        buf<<configuration->getText(0,4)<<" : "<<(int)m_caracteristiques.foi<<" / "<<(int)m_caracteristiques.maxFoi;
        string.SetString(buf.str());
    }

    if ((string.GetRect().Right-string.GetRect().Left)>104)
    {
        string.SetSize(14*configuration->Resolution.h/600);
        if ((string.GetRect().Right-string.GetRect().Left)>104)
        {
            std::ostringstream  buf;
            buf<<(int)m_caracteristiques.foi<<" / "<<(int)m_caracteristiques.maxFoi;
            string.SetString(buf.str());
        }
    }


    string.SetX((288*configuration->Resolution.w/800)+54*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
    string.SetY(263*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    if (m_caracteristiques.maxFoi!=m_personnage.getCaracteristique().maxFoi)
        string.SetColor(sf::Color(0,128,255));
    else
        string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    string.SetSize(16*configuration->Resolution.h/600);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.force;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(311*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.force!=m_personnage.getCaracteristique().force)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    m_buttonPointe=0;

    if (m_caracteristiques.pts_restant>0)
    {
        sprite.SetY(string.GetPosition().y);

        if (eventManager->getEvenement(Mouse::Left,"CA"))
            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    &&eventManager->getPositionSouris().x < sprite.GetPosition().x+sprite.GetSize().x
                    &&eventManager->getPositionSouris().y > sprite.GetPosition().y
                    &&eventManager->getPositionSouris().y < sprite.GetPosition().y+sprite.GetSize().y)
                sprite.SetColor(sf::Color(128,128,128)),m_buttonPointe=1;

        moteurGraphique->AjouterCommande(&sprite,15,0);
        sprite.SetColor(sf::Color(255,255,255));
    }

    if(configuration->text_menus.size() > 5)
        string.SetString(configuration->text_menus[5].c_str());
    else
        string.SetString("Error");

    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(311*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.dexterite;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(338*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.dexterite!=m_personnage.getCaracteristique().dexterite)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_caracteristiques.pts_restant>0)
    {
        sprite.SetY(string.GetPosition().y);

        if (eventManager->getEvenement(Mouse::Left,"CA"))
            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    &&eventManager->getPositionSouris().x < sprite.GetPosition().x+sprite.GetSize().x
                    &&eventManager->getPositionSouris().y > sprite.GetPosition().y
                    &&eventManager->getPositionSouris().y < sprite.GetPosition().y+sprite.GetSize().y)
                sprite.SetColor(sf::Color(128,128,128)),m_buttonPointe=2;

        moteurGraphique->AjouterCommande(&sprite,15,0);
        sprite.SetColor(sf::Color(255,255,255));
    }

    string.SetString(configuration->getText(0,6).c_str());
    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(338*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.vitalite;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(365*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.vitalite!=m_personnage.getCaracteristique().vitalite)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_caracteristiques.pts_restant>0)
    {
        sprite.SetY(string.GetPosition().y);

        if (eventManager->getEvenement(Mouse::Left,"CA"))
            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    &&eventManager->getPositionSouris().x < sprite.GetPosition().x+sprite.GetSize().x
                    &&eventManager->getPositionSouris().y > sprite.GetPosition().y
                    &&eventManager->getPositionSouris().y < sprite.GetPosition().y+sprite.GetSize().y)
                sprite.SetColor(sf::Color(128,128,128)),m_buttonPointe=3;

        moteurGraphique->AjouterCommande(&sprite,15,0);
        sprite.SetColor(sf::Color(255,255,255));
    }

    string.SetString(configuration->getText(0,7).c_str());
    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(365*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.piete;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(392*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.piete!=m_personnage.getCaracteristique().piete)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_caracteristiques.pts_restant>0)
    {
        sprite.SetY(string.GetPosition().y);

        if (eventManager->getEvenement(Mouse::Left,"CA"))
            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    &&eventManager->getPositionSouris().x < sprite.GetPosition().x+sprite.GetSize().x
                    &&eventManager->getPositionSouris().y > sprite.GetPosition().y
                    &&eventManager->getPositionSouris().y < sprite.GetPosition().y+sprite.GetSize().y)
                sprite.SetColor(sf::Color(128,128,128)),m_buttonPointe=4;

        moteurGraphique->AjouterCommande(&sprite,15,0);
        sprite.SetColor(sf::Color(255,255,255));
    }

    string.SetString(configuration->getText(0,8).c_str());
    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(392*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.charisme;
        string.SetString(buf.str());
        string.SetX((129*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(419*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.charisme!=m_personnage.getCaracteristique().charisme)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    if (m_caracteristiques.pts_restant>0)
    {
        sprite.SetY(string.GetPosition().y);

        if (eventManager->getEvenement(Mouse::Left,"CA"))
            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    &&eventManager->getPositionSouris().x < sprite.GetPosition().x+sprite.GetSize().x
                    &&eventManager->getPositionSouris().y > sprite.GetPosition().y
                    &&eventManager->getPositionSouris().y < sprite.GetPosition().y+sprite.GetSize().y)
                sprite.SetColor(sf::Color(128,128,128)),m_buttonPointe=5;

        moteurGraphique->AjouterCommande(&sprite,15,0);
        sprite.SetColor(sf::Color(255,255,255));
    }

    string.SetString(configuration->getText(0,9).c_str());
    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(419*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    string.SetString(configuration->getText(0,12).c_str());
    string.SetX(16*configuration->Resolution.w/800);
    string.SetY(446*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.pts_restant;
        string.SetString(buf.str());
        string.SetX((172*configuration->Resolution.w/800)+21*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(450*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }


    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.degatsMin<<" - "<<m_caracteristiques.degatsMax;
        string.SetString(buf.str());
        string.SetX((314*configuration->Resolution.w/800)+32*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(300*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.degatsMin!=m_personnage.getCaracteristique().degatsMin||m_caracteristiques.degatsMax!=m_personnage.getCaracteristique().degatsMax)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    string.SetString(configuration->getText(0,10).c_str());
    string.SetX(234*configuration->Resolution.w/800);
    string.SetY(300*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_caracteristiques.armure;
        string.SetString(buf.str());
        string.SetX((314*configuration->Resolution.w/800)+32*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(327*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        if (m_caracteristiques.armure!=m_personnage.getCaracteristique().armure)
            string.SetColor(sf::Color(0,128,255));
        else
            string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }

    string.SetString(configuration->getText(0,11).c_str());
    string.SetX(234*configuration->Resolution.w/800);
    string.SetY(327*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
    string.SetColor(sf::Color(255,255,255));
    moteurGraphique->AjouterTexte(&string,15);

    {
        std::ostringstream  buf;
        buf<<m_argent;
        string.SetString(buf.str());
        string.SetSize(14*configuration->Resolution.h/600);
        string.SetX((112*configuration->Resolution.w/800)+32*configuration->Resolution.w/800-(string.GetRect().Right-string.GetRect().Left)/2);
        string.SetY(218*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        string.SetColor(sf::Color(255,255,255));
        moteurGraphique->AjouterTexte(&string,15);
    }
}

void Hero::AfficherAmis()
{
    for(unsigned i = 0 ; i < m_amis.size() ; ++i)
    {
        moteurGraphique->AjouterTexte(m_amis[i]->getCaracteristique().nom, coordonnee(8, 64 + i * 26), 14, 0, 14);

        sf::Sprite temp;
        temp.SetImage(*moteurGraphique->getImage(0));
        temp.SetColor(sf::Color(32,32,32));
        temp.SetPosition(12,82 + i * 26);
        temp.Resize(142,8);
        moteurGraphique->AjouterCommande(&temp, 14, 0);

        temp.Resize(m_amis[i]->getCaracteristique().vie/m_amis[i]->getCaracteristique().maxVie * 140,6);
        temp.Move(1,1);
        temp.SetColor(sf::Color(224,32,32));
        moteurGraphique->AjouterCommande(&temp, 14, 0);

        temp.Resize(8,8);
        temp.SetPosition(162,82 + i * 26);
        temp.SetColor(sf::Color(164,32,32));


        if(    eventManager->getPositionSouris().x > temp.GetPosition().x
            && eventManager->getPositionSouris().x < temp.GetPosition().x + 8
            && eventManager->getPositionSouris().y > temp.GetPosition().y
            && eventManager->getPositionSouris().y < temp.GetPosition().y + 8)
            {
                temp.SetColor(sf::Color(64,32,32));

                if(eventManager->getEvenement(Mouse::Left,"CA"))
                {
                    bool charme = false;
                    eventManager->StopEvenement(Mouse::Left,"C");
                    eventManager->StopEvenement(Mouse::Left,"CA");
                    for(unsigned j = 0 ; j < m_personnage.m_miracleEnCours.size() ; ++j)
                        for(unsigned k = 0 ; k < m_personnage.m_miracleEnCours[j].m_infos.size() ; ++k)
                        {
                            if(m_classe.miracles[m_personnage.m_miracleEnCours[j].m_modele].m_effets[m_personnage.m_miracleEnCours[j].m_infos[k]->m_effetEnCours].m_type == CHARME)
                                if(m_personnage.m_miracleEnCours[j].m_infos[k]->m_cible == m_amis[i])
                                   m_amis[i]->m_friendly = false, charme = true;
                        }

                    if(!charme)
                        m_amis[i]->InfligerDegats(m_amis[i]->getCaracteristique().vie * 2, NULL);

                    m_amis.erase(m_amis.begin() + i);
                    i--;
                }
            }

        moteurGraphique->AjouterCommande(&temp, 14, 0);
    }
}

void Hero::AfficherQuetes(float decalage)
{
    m_quetePointee = -1;
    coordonnee position = m_classe.position_contenu_quetes;
    for (int i = 0;i < (int)m_quetes.size();++i)
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetSize(12 * configuration->Resolution.h/600);
        texte.SetPosition(position.x * configuration->Resolution.w/800, (position.y - decalage) * configuration->Resolution.h/600);
        texte.SetString(m_quetes[i].m_nom);

        moteurGraphique->AjouterTexte(&texte,15);

        if (eventManager->getPositionSouris().x > m_classe.position_contenu_quetes.x * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().x < (m_classe.position_contenu_quetes.x + m_classe.position_contenu_quetes.w) * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().y > texte.GetRect().Top
                &&eventManager->getPositionSouris().y < texte.GetRect().Bottom)
        {
            sf::Sprite sprite;
            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.Resize(m_classe.position_contenu_quetes.w * configuration->Resolution.w/800, texte.GetRect().Bottom - texte.GetRect().Top+4);
            sprite.SetPosition(position.x * configuration->Resolution.w/800, (position.y - decalage) * configuration->Resolution.h/600);
            sprite.SetColor(sf::Color(255, 255, 255, 128));

            m_quetePointee = i;

            moteurGraphique->AjouterCommande(&sprite,15,0);
        }

        position.y += (int)texte.GetRect().Bottom - (int)texte.GetRect().Top;
    }

    int queteAffichee = m_queteSelectionnee;
    if (m_quetePointee >= 0)
        queteAffichee = m_quetePointee;

    if (queteAffichee >= 0 && queteAffichee < (int)m_quetes.size())
    {
        sf::Text texte;
        texte.SetFont(moteurGraphique->m_font);
        texte.SetSize(18 * configuration->Resolution.h/600);
        texte.SetPosition(m_classe.position_contenu_description_quete.x * configuration->Resolution.w/800, m_classe.position_contenu_description_quete.y * configuration->Resolution.h/600 - decalage);
        texte.SetString(m_quetes[queteAffichee].m_nom);

        moteurGraphique->AjouterTexte(&texte,15,0);

        texte.SetSize(12 * configuration->Resolution.h/600);
        texte.SetPosition(m_classe.position_contenu_description_quete.x * configuration->Resolution.w/800, m_classe.position_contenu_description_quete.y * configuration->Resolution.h/600 - decalage + 64);
        texte.SetString(m_quetes[queteAffichee].m_description);

        moteurGraphique->AjouterTexte(&texte,15,0);
    }
}

void Hero::AfficherFlecheQuetes(const std::string &nomMap, float temps)
{
    if(m_queteSelectionnee >= 0)
    {
        if(nomMap == m_quetes[m_queteSelectionnee].m_map)
        {
            coordonneeDecimal pos,dir;

            pos.x = (m_personnage.getCoordonneePixel().x/COTE_TILE-m_personnage.getCoordonneePixel().y/COTE_TILE-1)*(64);
            pos.y = (m_personnage.getCoordonneePixel().x/COTE_TILE+m_personnage.getCoordonneePixel().y/COTE_TILE)*(32);

            dir.x = (m_quetes[m_queteSelectionnee].m_position.x-m_quetes[m_queteSelectionnee].m_position.y-1)*(64);
            dir.y = (m_quetes[m_queteSelectionnee].m_position.x+m_quetes[m_queteSelectionnee].m_position.y)*(32);


            float m=atan2((double)(pos.x-dir.x),(double)(pos.y-dir.y));
            //m+=M_PI/3;

            m_angleFleche=(int)(m*180/M_PI);
        }
        else
            m_angleFleche += temps * 200;

        if (m_angleFleche>=360)
            m_angleFleche=0;
        if (m_angleFleche<0)
            m_angleFleche=360+m_angleFleche;

        Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_classe.arrow.image));
        sprite.SetX(m_classe.arrow.position.x*configuration->Resolution.x/800);
        sprite.SetY(m_classe.arrow.position.y*configuration->Resolution.h/600);
        sprite.Resize(m_classe.arrow.position.w*configuration->Resolution.w/800, m_classe.arrow.position.h*configuration->Resolution.h/600);
        sprite.SetOrigin(m_classe.arrow.centre.x, m_classe.arrow.centre.y);
        sprite.SetRotation(m_angleFleche);

        moteurGraphique->AjouterCommande(&sprite,18,0);
    }
}



bool Hero::AfficherMiracles(float decalage, int fenetreEnCours)
{
    bool retour = false;
    sf::Text texte;
    texte.SetSize(14 * configuration->Resolution.h/600);

    std::ostringstream buf;
    buf<<m_personnage.getCaracteristique().miracles_restant<<endl;

    texte.SetString(buf.str());
    texte.SetX(m_classe.position_points_miracles.x*configuration->Resolution.w/800 + (m_classe.position_points_miracles.w*configuration->Resolution.w/800 -texte.GetRect().Right + texte.GetRect().Left)/2);
    texte.SetY((m_classe.position_points_miracles.y- decalage)*configuration->Resolution.h/600);

    moteurGraphique->AjouterTexte(&texte,15,0);

    if (eventManager->getPositionSouris().x >  m_classe.position_points_miracles.x * configuration->Resolution.w/800
     && eventManager->getPositionSouris().x < (m_classe.position_points_miracles.x + m_classe.position_points_miracles.w)*configuration->Resolution.w/800
     && eventManager->getPositionSouris().y >  m_classe.position_points_miracles.y * configuration->Resolution.h/600
     && eventManager->getPositionSouris().y < (m_classe.position_points_miracles.y + m_classe.position_points_miracles.h)*configuration->Resolution.h/600)
        moteurGraphique->AjouterTexte(configuration->getText(0,12), coordonnee(eventManager->getPositionSouris().x,
                                      eventManager->getPositionSouris().y - 20),20,false,12,sf::Color(224,224,224),true);

    texte.SetSize(12 * configuration->Resolution.h/600);

    for (int i = 0;i < (int)m_classe.position_miracles.size(); ++i)
        if (m_classe.page_miracles[i] == fenetreEnCours)
        {
            std::ostringstream buf;
            buf<<m_lvl_miracles[i]<<endl;
            texte.SetString(buf.str());
            texte.SetX(m_classe.position_miracles[i].x*configuration->Resolution.w/800 + (m_classe.position_miracles[i].w*configuration->Resolution.w/800 -texte.GetRect().Right + texte.GetRect().Left)/2);
            texte.SetY((m_classe.position_miracles[i].y + 51 - decalage) *configuration->Resolution.h/600);

            moteurGraphique->AjouterTexte(&texte,15,0);
        }

    for (int i = 0;i < (int)m_classe.miracles.size(); ++i)
        if (m_classe.page_miracles[i] == fenetreEnCours)
        {
            /*if (m_lvl_miracles[i] <= 0)
            {
                sf::Sprite temp;
                temp.SetImage(*moteurGraphique->getImage(0));
                temp.SetPosition(m_classe.position_miracles[i].x*configuration->Resolution.w/800, (m_classe.position_miracles[i].y - decalage)*configuration->Resolution.h/600);
                temp.Resize(m_classe.position_miracles[i].w*configuration->Resolution.w/800, m_classe.position_miracles[i].h*configuration->Resolution.h/600);
                temp.SetColor(sf::Color(32,32,32));
                temp.SetBlendMode(sf::Blend::Multiply);
                moteurGraphique->AjouterCommande(&temp, 15, 0);
            }*/

            if (eventManager->getPositionSouris().x > m_classe.position_miracles[i].x*configuration->Resolution.w/800
                    &&eventManager->getPositionSouris().x < (m_classe.position_miracles[i].x + m_classe.position_miracles[i].w)*configuration->Resolution.w/800
                    &&eventManager->getPositionSouris().y > m_classe.position_miracles[i].y*configuration->Resolution.h/600
                    &&eventManager->getPositionSouris().y < (m_classe.position_miracles[i].y + m_classe.position_miracles[i].h)*configuration->Resolution.h/600)
            {
                m_classe.miracles[i].AfficherDescription(coordonnee((m_classe.position_miracles[i].x + m_classe.position_miracles[i].w)*configuration->Resolution.w/800,
                        m_classe.position_miracles[i].y*configuration->Resolution.h/600));

                retour = true;

                if (eventManager->getEvenement(Mouse::Left,"C"))
                    if (m_lvl_miracles[i] > 0)
                    {
                        if (m_classe.miracles[i].m_buf != -1)
                            m_personnage.m_miracleALancer = m_classe.miracles[i].m_buf;
                        else
                            m_personnage.m_miracleALancer = i;
                        eventManager->StopEvenement(Mouse::Left,"C");

                        m_miracleEnMain = m_personnage.m_miracleALancer;
                    }

                if (eventManager->getEvenement(Mouse::Right,"C") && m_miracleEnMain < 0)
                {
                    if (m_personnage.getCaracteristique().miracles_restant > 0)
                        if (m_classe.miracles[i].m_buf == -1 || m_classe.miracles[i].m_buf != -1 && m_lvl_miracles[m_classe.miracles[i].m_buf] > 0)
                        {

                            Caracteristique temp = m_personnage.getCaracteristique();
                            temp.miracles_restant--;
                            m_lvl_miracles[i]++;
                            m_personnage.setCaracteristique(temp);
                        }
                    eventManager->StopEvenement(Mouse::Right,"C");

                    ChargerModele();
                }
            }
        }

    if (eventManager->getEvenement(Mouse::Left,"C"))
    {
        if (eventManager->getPositionSouris().x > 432 * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().x < 452 * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            if (m_miracleEnMain >= 0)
            {
                m_miracles_raccourcis[0] = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracles_raccourcis[0], m_miracles_raccourcis[0] = -1;

            eventManager->StopEvenement(Mouse::Left,"C");
        }
        else if (eventManager->getPositionSouris().x > 464 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 484 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            if (m_miracleEnMain >= 0)
            {
                m_miracles_raccourcis[1] = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracles_raccourcis[1], m_miracles_raccourcis[1] = -1;

            eventManager->StopEvenement(Mouse::Left,"C");
        }
        else if (eventManager->getPositionSouris().x > 496 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 516 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            if (m_miracleEnMain >= 0)
            {
                m_miracles_raccourcis[2] = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracles_raccourcis[2], m_miracles_raccourcis[2] = -1;

            eventManager->StopEvenement(Mouse::Left,"C");
        }
        else if (eventManager->getPositionSouris().x > 528 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 548 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            if (m_miracleEnMain >= 0)
            {
                m_miracles_raccourcis[3] = m_miracleEnMain;
                m_miracleEnMain = -1;
            }
            else
                m_miracleEnMain = m_miracles_raccourcis[3], m_miracles_raccourcis[3] = -1;

            eventManager->StopEvenement(Mouse::Left,"C");
        }
    }

    if (m_miracleEnMain >= 0 && m_miracleEnMain < (int)m_classe.position_miracles.size())
    {
        sf::Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(m_classe.interface_miracles[m_classe.page_miracles[m_miracleEnMain]].image));

        sprite.SetSubRect(IntRect(  m_classe.position_miracles[m_miracleEnMain].x,
                                    m_classe.position_miracles[m_miracleEnMain].y,
                                    m_classe.position_miracles[m_miracleEnMain].x + m_classe.position_miracles[m_miracleEnMain].w,
                                    m_classe.position_miracles[m_miracleEnMain].y + m_classe.position_miracles[m_miracleEnMain].h));
        sprite.SetX(eventManager->getPositionSouris().x - 16 * configuration->Resolution.x/800);
        sprite.SetY(eventManager->getPositionSouris().y - 16 * configuration->Resolution.h/600);

        sprite.Resize(32 * configuration->Resolution.x/800,32 * configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,19,0);
    }

    if (eventManager->getEvenement(Mouse::Right,"C"))
        m_miracleEnMain = -1;

    return (retour);
}

bool Hero::AfficherInventaire(float decalage,std::vector<Objet> trader)
{
    bool retour = false;
    m_objetVise = -1;
    for (int i=0;i<(int)m_inventaire.size();++i)
        if (i!=m_objetEnMain)
        {
            coordonneeDecimal position(0,0,0,0);
            sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.SetColor(GetItemColor(m_inventaire[i].getRarete()));
            sprite.SetColor(sf::Color(sprite.GetColor().r,sprite.GetColor().g,sprite.GetColor().b,96));

            if (!m_inventaire[i].Utilisable(m_caracteristiques,m_classe.ID))
                sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.5),(int)(sprite.GetColor().g*0.5),(int)(sprite.GetColor().b*0.5),32));

            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600);

            if (m_inventaire[i].m_equipe==-1)
            {
                sprite.SetX((m_inventaire[i].getPosition().x*32+m_classe.position_contenu_inventaire.x)*configuration->Resolution.w/800);
                sprite.SetY(((m_inventaire[i].getPosition().y-1)*32+(m_classe.position_contenu_inventaire.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);

                position.x=(m_inventaire[i].getPosition().x*32+m_classe.position_contenu_inventaire.x)*configuration->Resolution.w/800;
                position.y=((m_inventaire[i].getPosition().y-1)*32+(m_classe.position_contenu_inventaire.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                position.h=m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600;
                position.w=m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800;
            }
            else
            {

                if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
                {
                    sprite.Resize(m_classe.emplacements[m_inventaire[i].m_equipe].position.w*configuration->Resolution.w/800,m_classe.emplacements[m_inventaire[i].m_equipe].position.h*configuration->Resolution.h/600);

                    position.x=(m_classe.emplacements[m_inventaire[i].m_equipe].position.x + m_classe.emplacements[m_inventaire[i].m_equipe].position.w/2 - m_inventaire[i].getPositionImage().w/2 )*configuration->Resolution.w/800;
                    position.y=(m_classe.emplacements[m_inventaire[i].m_equipe].position.y + m_classe.emplacements[m_inventaire[i].m_equipe].position.h/2 - m_inventaire[i].getPositionImage().h/2 )*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                    position.h=m_classe.emplacements[m_inventaire[i].m_equipe].position.h*configuration->Resolution.w/800;
                    position.w=m_classe.emplacements[m_inventaire[i].m_equipe].position.w*configuration->Resolution.h/600;

                    sprite.SetX((m_classe.emplacements[m_inventaire[i].m_equipe].position.x)*configuration->Resolution.w/800);
                    sprite.SetY((m_classe.emplacements[m_inventaire[i].m_equipe].position.y)*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);

                    if (m_objetEnMain==-1&&eventManager->getPositionSouris().x>m_classe.emplacements[m_inventaire[i].m_equipe].position.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(m_classe.emplacements[m_inventaire[i].m_equipe].position.x+m_classe.emplacements[m_inventaire[i].m_equipe].position.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>m_classe.emplacements[m_inventaire[i].m_equipe].position.y*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<(m_classe.emplacements[m_inventaire[i].m_equipe].position.y+m_classe.emplacements[m_inventaire[i].m_equipe].position.h)*configuration->Resolution.y/600)
                        m_inventaire[i].AfficherCaracteristiques(coordonnee ((int)(position.x+ m_classe.emplacements[m_inventaire[i].m_equipe].position.w),(int)(position.y+position.h),0,0),m_caracteristiques), retour = true;
                }
            }

            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.Resize(position.w-4*configuration->Resolution.w/800,position.h-4*configuration->Resolution.h/600);
            sprite.Move(2*configuration->Resolution.w/800,2*configuration->Resolution.h/600);

            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.SetOrigin(m_inventaire[i].getPositionImage().w/2,m_inventaire[i].getPositionImage().h/2);

            sprite.SetX(position.x+m_inventaire[i].getPositionImage().w/2*configuration->Resolution.w/800);
            sprite.SetY(position.y+m_inventaire[i].getPositionImage().h/2*configuration->Resolution.h/600);

            sprite.SetColor(m_inventaire[i].m_color);

            sprite.SetImage(*moteurGraphique->getImage(m_inventaire[i].getImage()));
            sprite.SetSubRect(IntRect(m_inventaire[i].getPositionImage().x, m_inventaire[i].getPositionImage().y, m_inventaire[i].getPositionImage().x+m_inventaire[i].getPositionImage().w, m_inventaire[i].getPositionImage().y+m_inventaire[i].getPositionImage().h));
            sprite.Resize(m_inventaire[i].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[i].getTaille().y*32*configuration->Resolution.h/600);

            if (m_classe.emplacements[m_inventaire[i].m_equipe].emplacement==BOUCLIER && m_inventaire[i].m_type==ARME)
                sprite.SetRotation(-45);
            else
                sprite.SetRotation(0);

            if (!m_inventaire[i].Utilisable(m_caracteristiques,m_classe.ID))
                sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.5),(int)(sprite.GetColor().g*0.5),(int)(sprite.GetColor().b*0.5),255));

            moteurGraphique->AjouterCommande(&sprite,17,0);
            sprite.SetRotation(0);
        }

    m_max_defilement_trader=0;

    for (int i=0;i<(int)trader.size();++i)
    {
        if (trader[i].getPosition().y+trader[i].getTaille().y>m_max_defilement_trader)
            m_max_defilement_trader=trader[i].getPosition().y+trader[i].getTaille().y;

        if (trader[i].getPosition().y+trader[i].getTaille().y-m_defilement_trader>0&&trader[i].getPosition().y-m_defilement_trader<m_classe.position_contenu_marchand.h)
        {
            coordonneeDecimal position;
            sf::Sprite sprite;

            sprite.SetImage(*moteurGraphique->getImage(0));
            sprite.SetColor(GetItemColor(trader[i].getRarete()));
            sprite.SetColor(sf::Color(sprite.GetColor().r,sprite.GetColor().g,sprite.GetColor().b,96));

            if (!trader[i].Utilisable(m_caracteristiques,m_classe.ID))
                sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.5),(int)(sprite.GetColor().g*0.5),(int)(sprite.GetColor().b*0.5),32));

            sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800,trader[i].getTaille().y*32*configuration->Resolution.h/600);

            if (trader[i].m_equipe==-1)
            {
                sprite.SetX((trader[i].getPosition().x*32+m_classe.position_contenu_marchand.x)*configuration->Resolution.w/800);
                sprite.SetY(((trader[i].getPosition().y-1-m_defilement_trader)*32+(m_classe.position_contenu_marchand.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);

                position.x=(trader[i].getPosition().x*32+m_classe.position_contenu_marchand.x)*configuration->Resolution.w/800;
                position.y=((trader[i].getPosition().y-1-m_defilement_trader)*32+(m_classe.position_contenu_marchand.y))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                position.h=trader[i].getTaille().y*32*configuration->Resolution.h/600;
                position.w=trader[i].getTaille().x*32*configuration->Resolution.w/800;
            }
            else
            {
                if (trader[i].m_equipe>=0&&trader[i].m_equipe<(int)m_classe.emplacements.size())
                {
                    sprite.Resize(m_classe.emplacements[trader[i].m_equipe].position.w*configuration->Resolution.w/800,m_classe.emplacements[trader[i].m_equipe].position.h*configuration->Resolution.h/600);

                    position.x=(m_classe.emplacements[trader[i].m_equipe].position.x + m_classe.emplacements[trader[i].m_equipe].position.w/2 - trader[i].getPositionImage().w/2 )*configuration->Resolution.w/800;
                    position.y=(m_classe.emplacements[trader[i].m_equipe].position.y + m_classe.emplacements[trader[i].m_equipe].position.h/2 - trader[i].getPositionImage().h/2 )*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600;

                    position.h=m_classe.emplacements[trader[i].m_equipe].position.h*configuration->Resolution.w/800;
                    position.w=m_classe.emplacements[trader[i].m_equipe].position.w*configuration->Resolution.h/600;

                    sprite.SetX((m_classe.emplacements[trader[i].m_equipe].position.x)*configuration->Resolution.w/800);
                    sprite.SetY((m_classe.emplacements[trader[i].m_equipe].position.y)*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
                    if (m_objetEnMain==-1&&eventManager->getPositionSouris().x>m_classe.emplacements[trader[i].m_equipe].position.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(m_classe.emplacements[trader[i].m_equipe].position.x+m_classe.emplacements[trader[i].m_equipe].position.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>m_classe.emplacements[trader[i].m_equipe].position.y*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<(m_classe.emplacements[trader[i].m_equipe].position.y+m_classe.emplacements[trader[i].m_equipe].position.h)*configuration->Resolution.y/600)
                        trader[i].AfficherCaracteristiques(eventManager->getPositionSouris(),m_caracteristiques,(5-(float)m_caracteristiques.charisme/100)), retour = true;
                }
            }

            if (trader[i].getPosition().y+trader[i].getTaille().y-m_defilement_trader>=m_classe.position_contenu_marchand.h)
                sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800,(m_defilement_trader+m_classe.position_contenu_marchand.h-trader[i].getPosition().y)*32*configuration->Resolution.h/600);

            if (trader[i].getPosition().y-m_defilement_trader<0)
            {
                sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800,trader[i].getTaille().y*32*configuration->Resolution.h/600-(-trader[i].getPosition().y+m_defilement_trader)*32*configuration->Resolution.h/600);
                sprite.Move(0,(-trader[i].getPosition().y+m_defilement_trader)*32*configuration->Resolution.h/600);
            }

            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.Resize(position.w-4*configuration->Resolution.w/800,position.h-4*configuration->Resolution.h/600);

            if (trader[i].getPosition().y+trader[i].getTaille().y-m_defilement_trader>=m_classe.position_contenu_marchand.h)
                sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800-4,(m_defilement_trader+m_classe.position_contenu_marchand.h-trader[i].getPosition().y)*32*configuration->Resolution.h/600-4);

            if (trader[i].getPosition().y-m_defilement_trader<0)
            {
                sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800-4,trader[i].getTaille().y*32*configuration->Resolution.h/600-(-trader[i].getPosition().y+m_defilement_trader)*32*configuration->Resolution.h/600-4);
            }

            sprite.Move(2*configuration->Resolution.w/800,2*configuration->Resolution.h/600);

            moteurGraphique->AjouterCommande(&sprite,16,0);

            sprite.SetOrigin(trader[i].getPositionImage().w/2,trader[i].getPositionImage().h/2);

            sprite.SetX(position.x+trader[i].getPositionImage().w/2*configuration->Resolution.w/800);
            sprite.SetY(position.y+trader[i].getPositionImage().h/2*configuration->Resolution.h/600);

            sprite.SetColor(trader[i].m_color);

            sprite.SetImage(*moteurGraphique->getImage(trader[i].getImage()));
            sprite.SetSubRect(IntRect(trader[i].getPositionImage().x, trader[i].getPositionImage().y, trader[i].getPositionImage().x+trader[i].getPositionImage().w, trader[i].getPositionImage().y+trader[i].getPositionImage().h));
            sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800,trader[i].getTaille().y*32*configuration->Resolution.h/600);

            if (trader[i].getPosition().y+trader[i].getTaille().y-m_defilement_trader>=m_classe.position_contenu_marchand.h)
            {
                sprite.SetSubRect(sf::IntRect(trader[i].getPositionImage().x,trader[i].getPositionImage().y,trader[i].getPositionImage().x+trader[i].getTaille().x*32,trader[i].getPositionImage().y+(m_defilement_trader+m_classe.position_contenu_marchand.h-trader[i].getPosition().y)*32));
                sprite.Resize(trader[i].getTaille().x*32*configuration->Resolution.w/800,(m_defilement_trader+m_classe.position_contenu_marchand.h-trader[i].getPosition().y)*32*configuration->Resolution.h/600);
            }

            if (trader[i].getPosition().y-m_defilement_trader<0)
            {
                sprite.SetSubRect(sf::IntRect(trader[i].getPositionImage().x,trader[i].getPositionImage().y+(-trader[i].getPosition().y+m_defilement_trader)*32,trader[i].getPositionImage().x+trader[i].getTaille().x*32,trader[i].getPositionImage().y+trader[i].getTaille().y*32));
                sprite.Resize(trader[i].getTaille().x*32,trader[i].getTaille().y*32-(-trader[i].getPosition().y+m_defilement_trader)*32);
                sprite.Move(0,(-trader[i].getPosition().y+m_defilement_trader)*32);
            }

            if (m_classe.emplacements[trader[i].m_equipe].emplacement==BOUCLIER && trader[i].m_type==ARME)
                sprite.SetRotation(-45);
            else
                sprite.SetRotation(0);

            if (!trader[i].Utilisable(m_caracteristiques,m_classe.ID))
                sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.5),(int)(sprite.GetColor().g*0.5),(int)(sprite.GetColor().b*0.5),255));

            moteurGraphique->AjouterCommande(&sprite,17,0);
            sprite.SetRotation(0);
        }
    }

    if (!trader.empty())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(m_classe.scroll_button.image));
        sprite.SetX(m_classe.scroll_button.position.x*configuration->Resolution.x/800);
        sprite.SetY((m_classe.scroll_button.position.y+(m_defilement_trader*m_classe.position_contenu_marchand.h*24)/(m_max_defilement_trader-m_classe.position_contenu_marchand.h))*configuration->Resolution.h/600-decalage*configuration->Resolution.h/600);
        sprite.Resize(m_classe.scroll_button.position.w*configuration->Resolution.w/800, m_classe.scroll_button.position.h*configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,18,0);
    }

    if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
    {
        sf::Sprite sprite;

        sprite.SetImage(*moteurGraphique->getImage(0));

        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.h/600);

        coordonnee caseVisee;
        caseVisee.x=((eventManager->getPositionSouris().x+8*configuration->Resolution.x/800)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
        caseVisee.y=((eventManager->getPositionSouris().y+8*configuration->Resolution.y/600)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;

        sprite.SetX(eventManager->getPositionSouris().x*configuration->Resolution.w/configuration->Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.x/1600);
        sprite.SetY(eventManager->getPositionSouris().y*configuration->Resolution.h/configuration->Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.y/1200);

        sprite.SetColor(GetItemColor(m_inventaire[m_objetEnMain].getRarete()));

        int emplacement=-1;
        for (int k=0;k<(int)m_classe.emplacements.size();k++)
            if (eventManager->getPositionSouris().x>m_classe.emplacements[k].position.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<(m_classe.emplacements[k].position.x+m_classe.emplacements[k].position.w)*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>m_classe.emplacements[k].position.y*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<(m_classe.emplacements[k].position.y+m_classe.emplacements[k].position.h)*configuration->Resolution.y/600)
                emplacement=k;

        if (!PossibleEquiper(m_objetEnMain, emplacement))
            sprite.SetColor(sf::Color((int)(sprite.GetColor().r*0.25),(int)(sprite.GetColor().g*0.25),(int)(sprite.GetColor().b*0.25),128));

        if (eventManager->getPositionSouris().x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800||eventManager->getPositionSouris().x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800||eventManager->getPositionSouris().y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600||eventManager->getPositionSouris().y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
        {
            if (eventManager->getPositionSouris().x>m_classe.position_sac_inventaire.x*configuration->Resolution.x/800
                    &&eventManager->getPositionSouris().x<(m_classe.position_sac_inventaire.x+m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800
                    &&eventManager->getPositionSouris().y>m_classe.position_sac_inventaire.y*configuration->Resolution.y/600
                    &&eventManager->getPositionSouris().y<(m_classe.position_sac_inventaire.y+m_classe.position_sac_inventaire.h*20)*configuration->Resolution.x/600)
                sprite.SetColor(sf::Color(128,0,0,128));
        }
        else
        {
            sprite.SetX(caseVisee.x*32*configuration->Resolution.w/800 + m_classe.position_contenu_inventaire.x*configuration->Resolution.w/800);
            sprite.SetY(caseVisee.y*32*configuration->Resolution.h/600 + (m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.h/600);
        }

        moteurGraphique->AjouterCommande(&sprite,18,0);

        sprite.SetColor(m_inventaire[m_objetEnMain].m_color);

        sprite.SetImage(*moteurGraphique->getImage(m_inventaire[m_objetEnMain].getImage()));
        sprite.SetSubRect(IntRect(m_inventaire[m_objetEnMain].getPositionImage().x, m_inventaire[m_objetEnMain].getPositionImage().y, m_inventaire[m_objetEnMain].getPositionImage().x+m_inventaire[m_objetEnMain].getPositionImage().w, m_inventaire[m_objetEnMain].getPositionImage().y+m_inventaire[m_objetEnMain].getPositionImage().h));
        sprite.Resize(m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.w/800,m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.h/600);

        sprite.SetX(eventManager->getPositionSouris().x*configuration->Resolution.w/configuration->Resolution.x - m_inventaire[m_objetEnMain].getTaille().x*32*configuration->Resolution.x/1600);
        sprite.SetY(eventManager->getPositionSouris().y*configuration->Resolution.h/configuration->Resolution.y - m_inventaire[m_objetEnMain].getTaille().y*32*configuration->Resolution.y/1200);

        moteurGraphique->AjouterCommande(&sprite,19,0);
    }
    else if (eventManager->getPositionSouris().x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        caseVisee.x=(eventManager->getPositionSouris().x-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
        caseVisee.y=(eventManager->getPositionSouris().y-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);

        for (int i=0;i<(int)m_inventaire.size();++i)
            if (caseVisee.x>=m_inventaire[i].getPosition().x&&caseVisee.x<=m_inventaire[i].getPosition().x+m_inventaire[i].getTaille().x-1
                    &&caseVisee.y>=m_inventaire[i].getPosition().y&&caseVisee.y<=m_inventaire[i].getPosition().y+m_inventaire[i].getTaille().y-1)
                if (m_inventaire[i].m_equipe==-1)
                {
                    m_objetVise = i;
                    coordonnee temp=eventManager->getPositionSouris();
                    // temp.y-=32*configuration->Resolution.h/600;
                    temp.y+=48;
                    temp.x+=128;
                    int decalage = m_inventaire[i].AfficherCaracteristiques(temp,m_caracteristiques,1,1,0);
                    retour = true;

                    for (int j=0;j<(int)m_inventaire.size();j++)
                        if (m_inventaire[j].m_equipe>=0/*&&m_inventaire[j].m_type==m_inventaire[i].m_type*/)
                            for (int k=0;k<(int)m_inventaire[i].m_emplacement.size();k++)
                                for (int l=0;l<(int)m_inventaire[j].m_emplacement.size();l++)
                                    if (m_inventaire[i].m_emplacement[k]==m_inventaire[j].m_emplacement[l])
                                        temp.x=decalage-4,decalage=m_inventaire[j].AfficherCaracteristiques(temp,m_caracteristiques,1,1,0,1),l=(int)m_inventaire[j].m_emplacement.size(),k=(int)m_inventaire[i].m_emplacement.size();
                }
    }
    else if (eventManager->getPositionSouris().x>m_classe.position_contenu_marchand.x*configuration->Resolution.x/800&&eventManager->getPositionSouris().x<m_classe.position_contenu_marchand.x*configuration->Resolution.x/800+32*m_classe.position_contenu_marchand.w*configuration->Resolution.x/800&&eventManager->getPositionSouris().y>(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600&&eventManager->getPositionSouris().y<(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_marchand.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        caseVisee.x=(eventManager->getPositionSouris().x-m_classe.position_contenu_marchand.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
        caseVisee.y=m_defilement_trader+(eventManager->getPositionSouris().y-(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);

        for (int i=0;i<(int)trader.size();++i)
            if (caseVisee.x>=trader[i].getPosition().x&&caseVisee.x<=trader[i].getPosition().x+trader[i].getTaille().x-1
                    &&caseVisee.y>=trader[i].getPosition().y&&caseVisee.y<=trader[i].getPosition().y+trader[i].getTaille().y-1)
            {
                coordonnee temp=eventManager->getPositionSouris();
                temp.y+=48;
                temp.x-=96;
                int decalage=trader[i].AfficherCaracteristiques(temp,m_caracteristiques,(5-(float)m_caracteristiques.charisme/100),1,1);
                retour = true;

                for (int j=0;j<(int)m_inventaire.size();j++)
                    if (m_inventaire[j].m_equipe>=0)
                        for (int k=0;k<(int)trader[i].m_emplacement.size();k++)
                            for (int l=0;l<(int)m_inventaire[j].m_emplacement.size();l++)
                                if (trader[i].m_emplacement[k]==m_inventaire[j].m_emplacement[l])
                                    temp.x=decalage+12,decalage=m_inventaire[j].AfficherCaracteristiques(temp,m_caracteristiques,1,1,1,1),l=(int)m_inventaire[j].m_emplacement.size(),k=(int)trader[i].m_emplacement.size();
            }
    }

    AfficherCaracteristiques(decalage);

    return (retour);
}

void Hero::AfficherRaccourcis()
{
    for (int i=0;i<4;++i)
    {
        sf::Sprite sprite;

        if (m_objets_raccourcis[i] >= 0 && m_objets_raccourcis[i] < (int)m_inventaire.size())
        {
            sprite.SetImage(*moteurGraphique->getImage(m_inventaire[m_objets_raccourcis[i]].getImage()));

            sprite.SetSubRect(IntRect(  m_inventaire[m_objets_raccourcis[i]].getPositionImage().x,
                                        m_inventaire[m_objets_raccourcis[i]].getPositionImage().y,
                                        m_inventaire[m_objets_raccourcis[i]].getPositionImage().x+m_inventaire[m_objets_raccourcis[i]].getPositionImage().w,
                                        m_inventaire[m_objets_raccourcis[i]].getPositionImage().y+m_inventaire[m_objets_raccourcis[i]].getPositionImage().h));
            sprite.SetX(255 * configuration->Resolution.x/800 + 32*i*configuration->Resolution.x/800);
            sprite.SetY(492 * configuration->Resolution.h/600);

            sprite.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.h/600);

            moteurGraphique->AjouterCommande(&sprite,18,0);

            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    && eventManager->getPositionSouris().x < sprite.GetPosition().x + 20 * configuration->Resolution.x/800
                    && eventManager->getPositionSouris().y > sprite.GetPosition().y
                    && eventManager->getPositionSouris().y < sprite.GetPosition().y + 20 * configuration->Resolution.h/600 && m_objetEnMain < 0)
                m_inventaire[m_objets_raccourcis[i]].AfficherCaracteristiques(eventManager->getPositionSouris(), m_caracteristiques);
        }
        else
        {
            sprite.SetX(255 * configuration->Resolution.x/800 + 32*i*configuration->Resolution.x/800);
            sprite.SetY(492 * configuration->Resolution.h/600);

            sprite.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.h/600);

            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    && eventManager->getPositionSouris().x < sprite.GetPosition().x + 20 * configuration->Resolution.x/800
                    && eventManager->getPositionSouris().y > sprite.GetPosition().y
                    && eventManager->getPositionSouris().y < sprite.GetPosition().y + 20 * configuration->Resolution.h/600)
            {
                std::ostringstream buf;
                buf<<configuration->getText(0, 33)<<" ("<<i+1<<")";
                moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                              eventManager->getPositionSouris().y - 20),
                                              19,0,12,sf::Color(224,224,224),1);
            }
        }

        if (m_miracles_raccourcis[i] >= 0 && m_miracles_raccourcis[i] < (int)m_classe.position_miracles.size())
        {
            sf::Sprite sprite;
            sprite.SetImage(*moteurGraphique->getImage(m_classe.interface_miracles[m_classe.page_miracles[m_miracles_raccourcis[i]]].image));

            sprite.SetSubRect(IntRect(  m_classe.position_miracles[m_miracles_raccourcis[i]].x,
                                        m_classe.position_miracles[m_miracles_raccourcis[i]].y,
                                        m_classe.position_miracles[m_miracles_raccourcis[i]].x + m_classe.position_miracles[m_miracles_raccourcis[i]].w,
                                        m_classe.position_miracles[m_miracles_raccourcis[i]].y + m_classe.position_miracles[m_miracles_raccourcis[i]].h));
            sprite.SetX(431 * configuration->Resolution.x/800 + 32*i*configuration->Resolution.x/800);
            sprite.SetY(491 * configuration->Resolution.h/600);

            sprite.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.h/600);

            moteurGraphique->AjouterCommande(&sprite,18,0);

            if (m_personnage.m_miracleALancer == m_miracles_raccourcis[i])
            {
                sprite.SetImage(*moteurGraphique->getImage(0));
                sprite.SetColor(sf::Color(96,96,96));
                sprite.SetBlendMode(sf::Blend::Add);
                moteurGraphique->AjouterCommande(&sprite,18,0);
            }

            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    && eventManager->getPositionSouris().x < sprite.GetPosition().x + 20 * configuration->Resolution.x/800
                    && eventManager->getPositionSouris().y > sprite.GetPosition().y
                    && eventManager->getPositionSouris().y < sprite.GetPosition().y + 20 * configuration->Resolution.h/600 && m_miracleEnMain < 0)
                m_classe.miracles[m_miracles_raccourcis[i]].AfficherDescription(eventManager->getPositionSouris(), false);
        }
        else
        {
            sprite.SetX(432 * configuration->Resolution.x/800 + 32*i*configuration->Resolution.x/800);
            sprite.SetY(492 * configuration->Resolution.h/600);

            sprite.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.h/600);

            if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                    && eventManager->getPositionSouris().x < sprite.GetPosition().x + 20 * configuration->Resolution.x/800
                    && eventManager->getPositionSouris().y > sprite.GetPosition().y
                    && eventManager->getPositionSouris().y < sprite.GetPosition().y + 20 * configuration->Resolution.h/600)
            {
                std::ostringstream buf;
                buf<<configuration->getText(0, 34)<<" (F"<<i+1<<")";
                moteurGraphique->AjouterTexte(buf.str(),coordonnee(eventManager->getPositionSouris().x,
                                              eventManager->getPositionSouris().y - 20),
                                              19,0,12,sf::Color(224,224,224),1);
            }
        }
    }

    if (m_personnage.m_miracleALancer >= 0 && m_personnage.m_miracleALancer < (int)m_classe.position_miracles.size())
    {
        sf::Sprite sprite;
        sprite.SetImage(*moteurGraphique->getImage(m_classe.interface_miracles[m_classe.page_miracles[m_personnage.m_miracleALancer]].image));

        sprite.SetSubRect(IntRect(  m_classe.position_miracles[m_personnage.m_miracleALancer].x,
                                    m_classe.position_miracles[m_personnage.m_miracleALancer].y,
                                    m_classe.position_miracles[m_personnage.m_miracleALancer].x + m_classe.position_miracles[m_personnage.m_miracleALancer].w,
                                    m_classe.position_miracles[m_personnage.m_miracleALancer].y + m_classe.position_miracles[m_personnage.m_miracleALancer].h));
        sprite.SetX(512 * configuration->Resolution.x/800);
        sprite.SetY(545 * configuration->Resolution.h/600);

        sprite.Resize(20 * configuration->Resolution.x/800,20 * configuration->Resolution.h/600);

        moteurGraphique->AjouterCommande(&sprite,18,0);

        if (eventManager->getPositionSouris().x > sprite.GetPosition().x
                && eventManager->getPositionSouris().x < sprite.GetPosition().x + 20 * configuration->Resolution.x/800
                && eventManager->getPositionSouris().y > sprite.GetPosition().y
                && eventManager->getPositionSouris().y < sprite.GetPosition().y + 20 * configuration->Resolution.h/600)
            m_classe.miracles[m_personnage.m_miracleALancer].AfficherDescription(eventManager->getPositionSouris(), false);
    }
}

void Hero::PlacerCamera()
{
    moteurGraphique->m_camera.SetCenter((m_personnage.getCoordonneePixel().x - m_personnage.getCoordonneePixel().y) * 64.0f * DIVISEUR_COTE_TILE,
                                        (m_personnage.getCoordonneePixel().x + m_personnage.getCoordonneePixel().y) * 32.0f * DIVISEUR_COTE_TILE + 32.0f);
    moteurGraphique->m_camera.SetSize(800, 600);
    moteurGraphique->m_camera.Zoom(configuration->zoom);

    moteurGraphique->m_camera.Move(moteurGraphique->decalageCamera.x, moteurGraphique->decalageCamera.y);
}

bool Hero::TestMonstreVise(Monstre *monstre)
{
    if (monstre!=NULL)
        if (m_monstreVise>-1&&m_caracteristiques.vie>0)
        {
            if (((!m_personnage.m_shooter||monstre->m_friendly)&&(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)<=1)) || ((!m_personnage.m_shooter||monstre->m_friendly)&&(fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)<=1&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)<=1))
                    ||(!monstre->m_friendly&&m_personnage.m_shooter&&(fabs(m_personnage.getCoordonnee().x-monstre->getCoordonnee().x)<=13&&fabs(m_personnage.getCoordonnee().y-monstre->getCoordonnee().y)<=13)) || (!monstre->m_friendly&&m_personnage.m_shooter&&(fabs(m_personnage.getCoordonnee().x-monstre->getProchaineCase().x)<=13&&fabs(m_personnage.getCoordonnee().y-monstre->getProchaineCase().y)<=13)))
            {
                m_personnage.setArrivee(m_personnage.getCoordonnee());

                if (!monstre->m_friendly)
                    m_personnage.Frappe(m_personnage.getCoordonneePixel(),monstre->getCoordonneePixel());

                return 1;
            }
            else
                m_personnage.setArrivee(monstre->getProchaineCase());
        }
    return 0;
}

void Hero::AugmenterAme(float temps)
{
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

        temp.maxVie=temp.vitalite*10;
        temp.vie=temp.maxVie;

        temp.maxFoi=temp.piete*10;
        temp.foi=temp.maxFoi;

        m_personnage.setCaracteristique(temp);

        RecalculerCaracteristiques();

        temp.vie=m_caracteristiques.maxVie;
        temp.foi=m_caracteristiques.maxFoi;
        m_caracteristiques.vie=m_caracteristiques.maxVie;
        m_caracteristiques.foi=m_caracteristiques.maxFoi;
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

    temp.maxVie                 = temp.vitalite*10;
    temp.maxFoi                 = temp.piete*10;

    m_caracteristiques          = m_personnage.getCaracteristique();
    m_caracteristiques.maxVie   = 0;
    m_caracteristiques.maxFoi   = 0;

    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0)
            if (m_inventaire[i].Utilisable(buf,m_classe.ID))
                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    switch (m_inventaire[i].m_benedictions[j].type)
                    {
                    case FO_SUPP:
                        m_caracteristiques.force+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case VIT_SUPP:
                        m_caracteristiques.vitalite+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case PI_SUPP:
                        m_caracteristiques.piete+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case DEX_SUPP:
                        m_caracteristiques.dexterite+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case CH_SUPP:
                        m_caracteristiques.charisme+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case VIE_SUPP:
                        m_caracteristiques.maxVie+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    case FOI_SUPP:
                        m_caracteristiques.maxFoi+=m_inventaire[i].m_benedictions[j].info1;
                        break;
                    }

    m_caracteristiques.degatsMin=0/*m_caracteristiques.force/3*/;
    m_caracteristiques.degatsMax=0/*(int)(m_caracteristiques.force)/2*/;
    m_caracteristiques.armure=m_caracteristiques.dexterite;

    temp.degatsMin=0/*m_caracteristiques.force/3*/;
    temp.degatsMax=0/*(int)(m_caracteristiques.force)/2*/;
    temp.armure=m_caracteristiques.dexterite;

    m_caracteristiques.maxVie+=m_caracteristiques.vitalite*10;
    m_caracteristiques.maxFoi+=m_caracteristiques.piete*10;


    for (int i=0;i<(int)m_inventaire.size();++i)
        if (m_inventaire[i].m_equipe>=0)
            if (m_inventaire[i].Utilisable(buf,m_classe.ID))
            {
                int accru=100;
                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    if (m_inventaire[i].m_benedictions[j].type==EFFICACITE_ACCRUE)
                        accru+=m_inventaire[i].m_benedictions[j].info1;

                m_caracteristiques.degatsMin+=m_inventaire[i].m_degatsMin*(m_caracteristiques.force + 100)/100*accru/100;
                m_caracteristiques.degatsMax+=m_inventaire[i].m_degatsMax*(m_caracteristiques.force + 100)/100*accru/100;
                m_caracteristiques.armure+=m_inventaire[i].m_armure*accru/100;

                for (int j=0;j<(int)m_inventaire[i].m_benedictions.size();++j)
                    if (m_inventaire[i].m_benedictions[j].type==DEGATS_FEU||m_inventaire[i].m_benedictions[j].type==DEGATS_FOI)
                        m_caracteristiques.degatsMin+=m_inventaire[i].m_benedictions[j].info1,m_caracteristiques.degatsMax+=m_inventaire[i].m_benedictions[j].info1;

                temp.degatsMin+=m_inventaire[i].m_degatsMin*(m_caracteristiques.force + 100)/100;
                temp.degatsMax+=m_inventaire[i].m_degatsMax*(m_caracteristiques.force + 100)/100;
                temp.armure+=m_inventaire[i].m_armure;
            }

    for (int i = 0; i < (int)m_personnage.m_effets.size(); ++i)
        if (m_personnage.m_effets[i].m_effet.m_actif)
        {
            if (m_personnage.m_effets[i].m_type == AURA_DEGATS)
            {
                m_caracteristiques.degatsMin += (int)m_personnage.m_effets[i].m_info2;
                m_caracteristiques.degatsMax += (int)m_personnage.m_effets[i].m_info3;
            }

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

    m_caracteristiques.niveau=temp.niveau;

    temp.reserveVie  = reserveVie;
    temp.reserveFoi  = reserveFoi;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.reserveVie  = reserveVie;
    m_caracteristiques.reserveFoi  = reserveFoi;

    m_caracteristiques.vie=vie,m_caracteristiques.foi=foi;

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


}

bool Hero::AjouterMiracleArme()
{
    if (m_weaponMiracle>=0&&m_weaponMiracle<(int)m_classe.miracles.size())
    {
        m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());
        m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

        m_personnage.m_miracleEnCours.back().m_modele=m_weaponMiracle;

        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x+cos(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;
        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y+sin(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;

        return 1;
    }
    return 0;
}

void Hero::StopMiraclesFrappe()
{
    for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
            if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == CORPS_A_CORPS)
            {
                delete m_personnage.m_miracleEnCours[i].m_infos[i];
                m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin()+i);
            }
}

void Hero::StopMiraclesCharme()
{
    for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
            if (m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == CHARME)
            {
                delete m_personnage.m_miracleEnCours[i].m_infos[i];
                m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin()+i);
            }
}

/*void Hero::DetruireMiracle(int i, int effet)
{

    for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
        if (m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours == effet)
        {
            for (int p=0;p<(int)m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                DetruireMiracle(i, m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p])

            if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == AURA)
            {
                DetruireMiracle(i, m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_informations[0]);

                for (int p=0;p<(int)m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                {
                    m_personnage.m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_effetEnCours=m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p];
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_position=m_personnage.m_miracleEnCours[i].m_infos[o]->m_position;
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_cible=m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible;
                }
            }

            if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET)
            {


                for (int p=0;p<(int)m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien.size();p++)
                {
                    m_personnage.m_miracleEnCours[i].m_infos.push_back(InfosEntiteMiracle ());
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_effetEnCours=m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_lien[p];
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_position=m_personnage.m_miracleEnCours[i].m_infos[o]->m_position;
                    m_personnage.m_miracleEnCours[i].m_infos.back()->m_cible=m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible;
                }
            }

            m_personnage.m_miracleEnCours[i].m_infos.erase(m_personnage.m_miracleEnCours[i].m_infos.begin() + o);
        }

}
*/

bool Hero::UtiliserMiracle(int miracle, Personnage *cible, coordonnee cible_coord)
{
    float m=atan2((double)(m_personnage.getCoordonnee().x-cible_coord.x),(double)(m_personnage.getCoordonnee().y-cible_coord.y));
    m+=M_PI/3;

    m=(int)(m*180/M_PI);
    if (m>=360)
        m=0;
    if (m<0)
        m=360+m;
    m_personnage.setAngle((int)m);

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

                                if (m_personnage.m_miracleEnCours[i].m_modele == miracle)
                                    retour = true;
                                o = -1;
                            }

                    if (retour)
                        return 1;
                }
            if(m_classe.miracles[miracle].m_unique)
                for (int i = 0; i < (int)m_personnage.m_miracleEnCours.size(); ++i)
                    if(m_personnage.m_miracleEnCours[i].m_modele == miracle)
                    {
                        for (int o = 0; o < (int) m_personnage.m_miracleEnCours[i].m_infos.size() ; ++o)
                            if(m_classe.miracles[m_personnage.m_miracleEnCours[i].m_modele].m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_effetEnCours].m_type == EFFET)
                                m_personnage.m_miracleEnCours[i].m_infos[o]->m_cible->m_effets[m_personnage.m_miracleEnCours[i].m_infos[o]->m_IDObjet].m_effet.m_actif = false;

                        m_personnage.m_miracleEnCours.erase(m_personnage.m_miracleEnCours.begin()+i);
                        i = -1;
                    }


            if (m_classe.miracles[miracle].m_coutFoi + m_classe.miracles[miracle].m_reserveFoi <= m_caracteristiques.foi && m_classe.miracles[miracle].m_reserveFoi <= m_caracteristiques.maxFoi - m_caracteristiques.reserveFoi
                    && m_classe.miracles[miracle].m_coutVie + m_classe.miracles[miracle].m_reserveVie
                    <= m_caracteristiques.vie && m_classe.miracles[miracle].m_reserveVie <= m_caracteristiques.maxVie - m_caracteristiques.reserveVie)
                if (m_cas == m_classe.miracles[miracle].m_cas || m_classe.miracles[miracle].m_cas == -1)
                    if (cible != NULL && m_classe.miracles[miracle].m_effets[0].m_type == CORPS_A_CORPS || m_classe.miracles[miracle].m_effets[0].m_type != CORPS_A_CORPS )
                    {
                        m_personnage.m_lancementMiracleEnCours = true;
                        m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());

                        if (m_classe.miracles[miracle].m_effets[0].m_type != CORPS_A_CORPS)
                        {
                            Caracteristique temp = m_personnage.getCaracteristique();
                            temp.foi        -= m_classe.miracles[miracle].m_coutFoi + m_classe.miracles[miracle].m_reserveFoi;
                            temp.vie        -= m_classe.miracles[miracle].m_coutVie + m_classe.miracles[miracle].m_reserveVie;
                            temp.reserveFoi += m_classe.miracles[miracle].m_reserveFoi;
                            temp.reserveVie += m_classe.miracles[miracle].m_reserveVie;
                            m_personnage.setCaracteristique(temp);

                            m_personnage.m_miracleEnCours.back().m_dejaConsommeFoi = true;
                        }

                        m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

                        m_personnage.m_miracleEnCours.back().m_modele = miracle;

                        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x;
                        m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y;

                        m_personnage.m_miracleEnCours.back().m_coordonneeCible  = cible_coord;
                        m_personnage.m_miracleEnCours.back().m_coordonneeDepart = m_personnage.getCoordonnee();

                        return 1;
                    }
        }
    return 0;
}

bool Hero::AjouterObjet(Objet objet,bool enMain)
{
    bool ramasser=false;

    if (!enMain)
    {
        ramasser = AjouterObjetInventaire(objet,&m_inventaire,m_classe.position_contenu_inventaire, false);
        if (ramasser)
            m_inventaire.back().JouerSon();

        if (m_inventaire.back().m_type == ARME || m_inventaire.back().m_type == ARMURE)
        {
            bool continuer = true;
            for (int j=0;j<(int)m_classe.emplacements.size() && continuer;j++)
                for (int i=0;i<(int)m_inventaire.back().m_emplacement.size() && continuer;++i)
                    if (m_inventaire.back().m_emplacement[i] == m_classe.emplacements[j].emplacement && continuer)
                    {
                        continuer=false;
                        for(unsigned k = 0 ; k < m_inventaire.size() - 1 ; ++k)
                            if(m_inventaire[k].m_equipe == j)
                                continuer = true;
                        if(!continuer)
                            Equiper(m_inventaire.size() - 1, j);
                    }
            if(!continuer)
                ChargerModele();
        }
    }
    else if (m_objetEnMain==-1)
    {
        m_inventaire.push_back(objet);

        m_objetEnMain=m_inventaire.size()-1;

        ramasser = true;
    }

    return ramasser;
}

Objet Hero::DeposerObjet()
{
    Objet temp;
    if (m_objetADeposer>=0&&m_objetADeposer<(int)m_inventaire.size())
    {
        temp=m_inventaire[m_objetADeposer];

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

bool Hero::PrendreEnMain(std::vector<Objet> *trader)
{
    if (trader)
    {
        if (eventManager->getPositionSouris().x>14*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().x<34*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().y>260*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().y<290*configuration->Resolution.x/800)
            m_defilement_trader--;

        if (eventManager->getPositionSouris().x>14*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().x<34*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().y>442*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().y<472*configuration->Resolution.x/800)
            m_defilement_trader++;
    }

    if (m_buttonPointe>0)
    {
        if (m_buttonPointe==1&&m_caracteristiques.pts_restant>0)
        {
            Caracteristique temp = m_personnage.getCaracteristique();
            temp.pts_restant--;
            temp.force++;
            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
        else if (m_buttonPointe==2&&m_caracteristiques.pts_restant>0)
        {
            Caracteristique temp = m_personnage.getCaracteristique();
            temp.pts_restant--;
            temp.dexterite++;
            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
        else if (m_buttonPointe==3&&m_caracteristiques.pts_restant>0)
        {
            Caracteristique temp = m_personnage.getCaracteristique();
            temp.pts_restant--;
            temp.vitalite++;
            //temp.maxVie=temp.vitalite*10;
            //temp.vie=temp.maxVie;
            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
            m_caracteristiques.vie+=10;
        }
        else if (m_buttonPointe==4&&m_caracteristiques.pts_restant>0)
        {
            Caracteristique temp = m_personnage.getCaracteristique();
            temp.pts_restant--;
            temp.piete++;
            //temp.maxFoi=temp.piete*10;
            //temp.foi=temp.maxFoi;
            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
            m_caracteristiques.foi+=10;
        }
        else if (m_buttonPointe==5&&m_caracteristiques.pts_restant>0)
        {
            Caracteristique temp = m_personnage.getCaracteristique();
            temp.pts_restant--;
            temp.charisme++;
            m_personnage.setCaracteristique(temp);
            RecalculerCaracteristiques();
        }
    }
    if (m_objetEnMain >= 0)
    {
        if (eventManager->getPositionSouris().x > 255 * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().x < 275 * configuration->Resolution.w/800
                &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            m_objets_raccourcis[0] = m_objetEnMain;
            RangerObjet(m_objetEnMain);
            m_objetEnMain = -1;
        }
        else if (eventManager->getPositionSouris().x > 287 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 307 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            m_objets_raccourcis[1] = m_objetEnMain;
            RangerObjet(m_objetEnMain);
            m_objetEnMain = -1;
        }
        else if (eventManager->getPositionSouris().x > 319 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 339 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            m_objets_raccourcis[2] = m_objetEnMain;
            RangerObjet(m_objetEnMain);
            m_objetEnMain = -1;
        }
        else if (eventManager->getPositionSouris().x > 351 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().x < 371 * configuration->Resolution.w/800
                 &&eventManager->getPositionSouris().y > 492 * configuration->Resolution.h/600
                 &&eventManager->getPositionSouris().y < 512 * configuration->Resolution.h/600)
        {
            m_objets_raccourcis[3] = m_objetEnMain;
            RangerObjet(m_objetEnMain);
            m_objetEnMain = -1;
        }
    }

    m_objetADeposer=-1;
    if (eventManager->getPositionSouris().x>m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800
            &&eventManager->getPositionSouris().x<m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800+32*m_classe.position_contenu_inventaire.w*configuration->Resolution.x/800
            &&eventManager->getPositionSouris().y>(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600
            &&eventManager->getPositionSouris().y<(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_inventaire.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            caseVisee.x=((eventManager->getPositionSouris().x+8*configuration->Resolution.x/800)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
            caseVisee.y=((eventManager->getPositionSouris().y+8*configuration->Resolution.y/600)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;
        }
        else
        {
            caseVisee.x=((eventManager->getPositionSouris().x)-m_classe.position_contenu_inventaire.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
            caseVisee.y=((eventManager->getPositionSouris().y)-(m_classe.position_contenu_inventaire.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);
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
                                    if (m_inventaire[z].m_equipe==-1)
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
                if (m_inventaire[z].m_equipe==-1)
                    if (caseVisee.x>=m_inventaire[z].getPosition().x&&caseVisee.x<m_inventaire[z].getPosition().x+m_inventaire[z].getTaille().x
                            &&caseVisee.y>=m_inventaire[z].getPosition().y&&caseVisee.y<m_inventaire[z].getPosition().y+m_inventaire[z].getTaille().y)
                    {
                        if (eventManager->getEvenement(Key::LControl,"ET"))
                        {
                            m_inventaire[z].JouerSon();
                            m_argent+=m_inventaire[z].getPrix();
                            m_inventaire[z].m_equipe=-1;
                            AjouterObjetInventaire(m_inventaire[z],trader,m_classe.position_contenu_marchand,true);
                            delObjet(z);
                        }
                        else
                            m_objetEnMain=z,m_achat=false;
                    }
        }

    }
    else if (trader&&eventManager->getPositionSouris().x>m_classe.position_contenu_marchand.x*configuration->Resolution.x/800
             &&eventManager->getPositionSouris().x<m_classe.position_contenu_marchand.x*configuration->Resolution.x/800+32*m_classe.position_contenu_marchand.w*configuration->Resolution.x/800
             &&eventManager->getPositionSouris().y>(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600
             &&eventManager->getPositionSouris().y<(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600+32*m_classe.position_contenu_marchand.h*configuration->Resolution.y/600)
    {
        coordonnee caseVisee;
        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {
            caseVisee.x=((eventManager->getPositionSouris().x+8*configuration->Resolution.x/800)-m_classe.position_contenu_marchand.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800) - m_inventaire[m_objetEnMain].getTaille().x/2;
            caseVisee.y=m_defilement_trader+((eventManager->getPositionSouris().y+8*configuration->Resolution.y/600)-(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600) - m_inventaire[m_objetEnMain].getTaille().y/2;
        }
        else
        {
            caseVisee.x=((eventManager->getPositionSouris().x)-m_classe.position_contenu_marchand.x*configuration->Resolution.x/800)/(32*configuration->Resolution.x/800);
            caseVisee.y=m_defilement_trader+((eventManager->getPositionSouris().y)-(m_classe.position_contenu_marchand.y-32)*configuration->Resolution.y/600)/(32*configuration->Resolution.y/600);
        }

        if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
        {

            m_inventaire[m_objetEnMain].JouerSon();
            if (m_achat)
                m_argent+=(int)((float)m_inventaire[m_objetEnMain].getPrix()*(5-(float)m_caracteristiques.charisme/100));
            else
                m_argent+=m_inventaire[m_objetEnMain].getPrix();
            m_inventaire[m_objetEnMain].m_equipe=-1;
            AjouterObjetInventaire(m_inventaire[m_objetEnMain],trader,m_classe.position_contenu_marchand,true);
            delObjet(m_objetEnMain);
            m_objetEnMain=-1;
            m_achat=false;
        }
        else
        {
            for (int z=0;z<(int)trader->size();z++)
                if (caseVisee.x>=(*trader)[z].getPosition().x&&caseVisee.x<(*trader)[z].getPosition().x+(*trader)[z].getTaille().x
                        &&caseVisee.y>=(*trader)[z].getPosition().y&&caseVisee.y<(*trader)[z].getPosition().y+(*trader)[z].getTaille().y)
                    if ((int)((float)(*trader)[z].getPrix()*(5-(float)m_caracteristiques.charisme/100))<=m_argent)
                    {
                        m_achat=true;

                        if (AjouterObjet((*trader)[z],!eventManager->getEvenement(Key::LControl,"ET")));
                        {
                            m_argent-=(int)((float)(*trader)[z].getPrix()*(5-(float)m_caracteristiques.charisme/100));
                            if ((*trader)[z].m_type!=CONSOMMABLE)
                                trader->erase(trader->begin()+z);
                        }
                    }
        }
    }
    else
    {
        if (eventManager->getPositionSouris().x>m_classe.position_sac_inventaire.x*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().x<(m_classe.position_sac_inventaire.x+m_classe.position_sac_inventaire.w)*configuration->Resolution.x/800
                &&eventManager->getPositionSouris().y>m_classe.position_sac_inventaire.y*configuration->Resolution.y/600
                &&eventManager->getPositionSouris().y<(m_classe.position_sac_inventaire.y+m_classe.position_sac_inventaire.h*20)*configuration->Resolution.y/600)
        {
            if (m_objetEnMain>=0&&m_objetEnMain<(int)m_inventaire.size())
            {
                m_objetADeposer=m_objetEnMain;

                m_objetEnMain=-1;
                m_achat=false;

                //GenererGrille();
                RecalculerCaracteristiques();

                return 1;
            }
        }
        else
        {
            bool equipe=false;

            for (int k=0;k<(int)m_classe.emplacements.size();k++)
            {
                if (eventManager->getPositionSouris().x>m_classe.emplacements[k].position.x*configuration->Resolution.x/800
                        &&eventManager->getPositionSouris().x<(m_classe.emplacements[k].position.x+m_classe.emplacements[k].position.w)*configuration->Resolution.x/800
                        &&eventManager->getPositionSouris().y>m_classe.emplacements[k].position.y*configuration->Resolution.y/600
                        &&eventManager->getPositionSouris().y<(m_classe.emplacements[k].position.y+m_classe.emplacements[k].position.h)*configuration->Resolution.y/600)
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
    //GenererGrille();
    return 0;
}

bool Hero::PossibleEquiper(int numero, int emplacement)
{
    if (numero >= 0 && numero < (int)m_inventaire.size())
        if (m_inventaire[ numero].Utilisable(m_caracteristiques,m_classe.ID))
            for (int i=0;i<(int)m_inventaire[ numero].m_emplacement.size();++i)
                if (m_inventaire[ numero].m_emplacement[i]==m_classe.emplacements[emplacement].emplacement)
                    return true;
    return false;
}

void Hero::delObjet(int numero)
{
    if(numero >=0 && numero < (int) m_inventaire.size())
    {
        string temp = m_inventaire[numero].getNom();
        m_inventaire.erase(m_inventaire.begin()+numero);

        for (int i=0;i<4;++i)
        {
            if (numero < m_objets_raccourcis[i] && m_objets_raccourcis[i] >= 0)
                m_objets_raccourcis[i] --;

            if (numero == m_objets_raccourcis[i])
            {
                bool continuer=true;

                for (int j=0;j<(int)m_inventaire.size() && continuer;j++)
                    if (m_inventaire[j].getNom()==temp)
                    {
                        m_objets_raccourcis[i] = j;
                        continuer=false;
                    }

                if (continuer)
                    m_objets_raccourcis[i] = -1;
            }
        }
    }
}

bool Hero::UtiliserObjet(int numero)
{
    if (numero >= 0 && numero < (int)m_inventaire.size())
    {
        if (m_inventaire[numero].m_type == ARME || m_inventaire[numero].m_type == ARMURE)
        {
            bool continuer = true;
            for (int j=0;j<(int)m_classe.emplacements.size() && continuer;j++)
                for (int i=0;i<(int)m_inventaire[numero].m_emplacement.size() && continuer;++i)
                    if (m_inventaire[numero].m_emplacement[i] == m_classe.emplacements[j].emplacement && continuer)
                        Equiper(numero, j),continuer=false;

            ChargerModele();
        }
        else if (m_inventaire[numero].m_type == CONSOMMABLE)
        {
            m_classe.miracles.push_back(m_inventaire[numero].m_miracle);

            m_personnage.m_miracleEnCours.push_back(EntiteMiracle ());
            m_personnage.m_miracleEnCours.back().m_infos.push_back(new InfosEntiteMiracle ());

            m_personnage.m_miracleEnCours.back().m_modele = (int)m_classe.miracles.size()-1;

            m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.x=m_personnage.getCoordonneePixel().x+cos(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;
            m_personnage.m_miracleEnCours.back().m_infos.back()->m_position.y=m_personnage.getCoordonneePixel().y+sin(-(m_personnage.getAngle()+22.5)*M_PI/180)*96;

            delObjet(numero);

            return 1;
        }
    }
    RecalculerCaracteristiques();

    return 0;
}

bool Hero::Equiper(int numero, int emplacement)
{
    //GenererGrille();
    int ancienEquipe=-1;
    bool ok=true;

    if (numero>=0 && numero<(int)m_inventaire.size())
    {
        ok=true;

        if (PossibleEquiper(numero, emplacement))
        {
            for (int i=0;i<(int)m_inventaire.size();++i)
            {
                if (m_inventaire[i].m_equipe==emplacement)
                    if (numero!=i)
                        ancienEquipe=i;

                for (int j=0;j<(int)m_inventaire[i].m_emplacementImpossible.size();j++)
                    if (numero!=i)
                        if (m_inventaire[i].m_equipe>=0)
                            if (m_inventaire[i].m_emplacementImpossible[j]==m_classe.emplacements[emplacement].emplacement)
                                ok=false;


                if (numero>=0&&numero<(int)m_inventaire.size())
                    for (int j=0;j<(int)m_inventaire[numero].m_emplacementImpossible.size();j++)
                        if (m_inventaire[i].m_equipe>=0&&m_inventaire[i].m_equipe<(int)m_classe.emplacements.size())
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
                        if (m_inventaire[numero].m_type==ARME)
                        {
                            if (m_inventaire[numero].m_shoot_weapon)
                                m_personnage.m_shooter=true;
                            else
                                m_personnage.m_shooter=false;
                        }

                        m_inventaire[numero].m_equipe=emplacement;
                    }

                    if (ancienEquipe>=0&&ancienEquipe<(int)m_inventaire.size())
                        m_inventaire[ancienEquipe].m_equipe=-1;
                }

            if (ok)
            {
                m_inventaire[numero].JouerSon();
                if (m_objetEnMain>=0)
                    m_objetEnMain=ancienEquipe;
                else
                    RangerObjet(ancienEquipe);
                m_achat=false;
            }
        }
    }
    else
    {
        for (int i=0;i<(int)m_inventaire.size();++i)
            if (m_inventaire[i].m_equipe==emplacement)
                m_objetEnMain=i, m_achat=false;
    }

    RecalculerCaracteristiques();
    return 0;
}

void Hero::RangerObjet(int numero)
{
    bool continuer = true;
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
                    }
                }
        }
    }
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

    if (temp.vie > temp.maxVie  - temp.reserveVie )
    {
        temp.vie-=(temp.vie - temp.maxVie + temp.reserveVie)*vie/50;
        if (temp.vie < temp.maxVie - temp.reserveVie)
            temp.vie = temp.maxVie - temp.reserveVie;
    }
    else
        temp.vie += vie + m_caracteristiques.regenVie * vie;

    if (temp.vie > (temp.maxVie - temp.reserveVie) * 2)
        temp.vie = (temp.maxVie - temp.reserveVie) * 2;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.vie = temp.vie;
    m_caracteristiques.reserveVie = temp.reserveVie;

    /*if (m_caracteristiques.vie > m_personnage.getCaracteristique().vie)
        m_caracteristiques.vie += (m_personnage.getCaracteristique().vie-m_caracteristiques.vie)*(vie + m_caracteristiques.regenVie * vie)*5;
    else if (m_caracteristiques.vie < m_personnage.getCaracteristique().vie)
        m_caracteristiques.vie += (m_personnage.getCaracteristique().vie-m_caracteristiques.vie)*(vie + m_caracteristiques.regenVie * vie);*/

    /*if (m_caracteristiques.vie > (m_caracteristiques.maxVie - m_caracteristiques.reserveVie) * 2)
        m_caracteristiques.vie = (m_caracteristiques.maxVie - m_caracteristiques.reserveVie) * 2;*/

}
void Hero::RegenererFoi(float foi)
{
    Caracteristique temp=m_personnage.getCaracteristique();

    if (temp.reserveFoi < 0)
        temp.reserveFoi = 0;

    if (temp.foi > temp.maxFoi - temp.reserveFoi)
    {
        temp.foi -= (temp.foi - temp.maxFoi + temp.reserveFoi)*foi/50;
        if (temp.foi < temp.maxFoi - temp.reserveFoi)
            temp.foi = temp.maxFoi - temp.reserveFoi;
    }
    else
        temp.foi += foi + m_caracteristiques.regenFoi * foi;

    if (temp.foi > (temp.maxFoi - temp.reserveFoi) * 2)
        temp.foi = (temp.maxFoi - temp.reserveFoi) * 2;

    m_personnage.setCaracteristique(temp);

    m_caracteristiques.foi = temp.foi;
    m_caracteristiques.reserveFoi = temp.reserveFoi;
}

void Hero::setMonstreVise(int monstre)
{
    m_monstreVise=monstre;
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
int Hero::getMonstreVise()
{
    return m_monstreVise;
}
#endif

