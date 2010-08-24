

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



#include "objet.h"

#include "../globale.h"
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;

sf::Color GetItemColor(int rarete)
{
    if (rarete==NORMAL)
        return (sf::Color(224,224,224,255));
    if (rarete==BONNEFACTURE)
        return (sf::Color(160,0,160,255));
    if (rarete==BENI)
        return (sf::Color(0,128,255,255));
    if (rarete==SACRE)
        return (sf::Color(255,255,128,255));
    if (rarete==SANCTIFIE)
        return (sf::Color(0,255,192,255));
    if (rarete==DIVIN)
        return (sf::Color(255,164,32,255));
    if (rarete==INFERNAL)
        return (sf::Color(224,0,0,255));
    if (rarete==CRAFT)
        return (sf::Color(128,64,0,255));
    if (rarete==QUETE)
        return (sf::Color(96,255,96,255));

    return (sf::Color(224,224,224,255));
}


void ChargerMiracleBenediction(benediction &bene, Miracle &miracle, bool &m_useMiracle)
{
    Caracteristique caract;

    if (bene.type==DEGATS_SUPP)
    {
        caract.degatsMin[0] = bene.info2;
        caract.degatsMax[0] = bene.info3;

        if(bene.info1 == PHYSIQUE)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/PhysicEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/PhysicEffect.miracle.hs",caract,0);
        }

        if(bene.info1 == FEU)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/FireEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/FireEffect.miracle.hs",caract,0);
        }
        if(bene.info1 == FOI)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/HolyEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/HolyEffect.miracle.hs",caract,0);
        }
        if(bene.info1 == CORROSION)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/PoisonEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/PoisonEffect.miracle.hs",caract,0);
        }
    }

    if (bene.type==DEGATS_TEMPS_SUPP)
    {
        caract.degatsMin[0] = bene.info2;
        caract.degatsMax[0] = bene.info2;
        caract.armure[0] = bene.info3;

        if(bene.info1 == PHYSIQUE)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/PhysicTimeEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/PhysicTimeEffect.miracle.hs",caract,0);
        }
        if(bene.info1 == FEU)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/FireTimeEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/FireTimeEffect.miracle.hs",caract,0);
        }
        if(bene.info1 == FOI)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/HolyTimeEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/HolyTimeEffect.miracle.hs",caract,0);
        }
        if(bene.info1 == CORROSION)
        {
            if (!m_useMiracle)
                miracle.Charger("Data/Items/Miracles/PoisonTimeEffect.miracle.hs",caract,0),m_useMiracle=true;
            else
                miracle.Concatenencer("Data/Items/Miracles/PoisonTimeEffect.miracle.hs",caract,0);
        }
    }
}

Objet::Objet()
{
    m_nom="Item";
    m_chemin="";
    m_rarete=0;
    m_equipe=-1;

    m_position.x=0;
    m_position.y=0;

    m_armure=0;
    m_degatsMin=0;
    m_degatsMax=0;


    m_requirement.force=0;
    m_requirement.dexterite=0;
    m_requirement.charisme=0;
    m_requirement.vitalite=0;
    m_requirement.piete=0;
    m_capaciteBenediction = 0;

    m_prix=0;

    ai=0,aa=0,dii=0,dia=0,dai=0,daa=0,bi=0,ba=0;
    m_shoot_weapon=0;

    m_useMiracle = false;
    m_alpha = 0;

    m_dejaTrie = false;
    m_nbr_bless = 0;

    m_time = 0;
}

Objet::Objet(std::string nom, int rarete)
{
    m_nom=nom;
    m_rarete=rarete;
    m_image=0;
    m_chemin="";
    m_equipe=-1;
    m_capaciteBenediction=0;

    m_useMiracle = false;
    m_alpha = 0;
    m_nbr_bless = 0;
    m_time = 0;
}

Objet::~Objet()
{
    m_description.clear();
    m_benedictions.clear();
    m_emplacement.clear();
    m_emplacementImpossible.clear();
    m_emplacementImageHero.clear();
    m_cheminImageHero.clear();
}

const std::string &Objet::getChemin()
{
    return m_chemin;
}
const std::string &Objet::getNom()
{
    return m_nom;
}
int Objet::getRarete()
{
    return m_rarete;
}
int Objet::getImage()
{
    return m_image;
};
coordonnee Objet::getPosition()
{
    return m_position;
};
coordonnee Objet::getTaille()
{
    return m_taille;
};
coordonnee Objet::getPositionImage()
{
    return m_positionImage;
};

void Objet::setRarete(int chance)
{
    m_rarete=chance;
}
void Objet::setPosition(int x, int y)
{
    m_position.x=x,m_position.y=y;
}



void Objet::Sauvegarder(std::ofstream *fichier)
{
    char caractere,espace='_';
    caractere='*';
    fichier->write((char*)&caractere, sizeof(char));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='e';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_equipe, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));


    caractere='r';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_rarete, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='x';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_position.x, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));
    caractere='y';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_position.y, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='d';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='i';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_degatsMin, sizeof(int));
    // fichier->write((char*)&espace, sizeof(char));

    caractere='d';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='a';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_degatsMax, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='a';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_armure, sizeof(int));
    //fichier->write((char*)&espace, sizeof(char));


    caractere='m';
    fichier->write((char*)&caractere, sizeof(char));
    int n=m_chemin.size()+1;
    fichier->write((char *)&n, sizeof(int));
    fichier->write(m_chemin.c_str(), n);
    // fichier->write((char*)&espace, sizeof(char));

    for (int i=0;i<(int)m_benedictions.size();i++)
    {
        caractere='b';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].type, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='i';
        fichier->write((char*)&caractere, sizeof(char));
        caractere='1';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].info1, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='i';
        fichier->write((char*)&caractere, sizeof(char));
        caractere='2';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].info2, sizeof(int));

        caractere='i';
        fichier->write((char*)&caractere, sizeof(char));
        caractere='3';
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&m_benedictions[i].info2, sizeof(int));
        //fichier->write((char*)&espace, sizeof(char));

        caractere='$';
        //fichier->write((char*)&espace, sizeof(char));
        fichier->write((char*)&caractere, sizeof(char));
        fichier->write((char*)&espace, sizeof(char));
    }

    caractere='$';
    // fichier->write((char*)&espace, sizeof(char));
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&espace, sizeof(char));

    if (configuration->debug)
        console->Ajouter("/Sauvegarde de "+m_chemin);
}

void Objet::SauvegarderTexte(std::ofstream *fichier)
{
    *fichier<<" o ";
    *fichier<<" e"<<m_equipe;
    *fichier<<" r"<<m_rarete;

    *fichier<<" g"<<m_prix;

    *fichier<<" di"<<m_degatsMin;
    *fichier<<" da"<<m_degatsMax;
    *fichier<<" a"<<m_armure;
    *fichier<<" v"<<m_vie;
    *fichier<<" s"<<m_nbr_bless;

    *fichier<<" x"<<m_position.x;
    *fichier<<" y"<<m_position.y;

    *fichier<<" t"<<m_dejaTrie;

    *fichier<<" m"<<m_chemin;

    for (int i=0;i<(int)m_benedictions.size();i++)
    {
        *fichier<<" b"<<m_benedictions[i].type;

        *fichier<<" i1"<<m_benedictions[i].info1;
        *fichier<<" i2"<<m_benedictions[i].info2;
        *fichier<<" i3"<<m_benedictions[i].info3;

        *fichier<<" $ ";
    }

    for (int i=0;i<(int)m_miracles_benedictions.size();i++)
        *fichier<<" c"<<m_miracles_benedictions[i].m_chemin;

    *fichier<<" $ ";

    if (configuration->debug)
        console->Ajouter("/Sauvegarde de "+m_chemin);
}

void Objet::ChargerChemin(std::ifstream *fichier)
{
    char caractere;
    do
    {
        fichier->get(caractere);
        if (caractere=='m')
            *fichier>>m_chemin;

        if (fichier->eof())
        {
            throw "Impossible de charger l'objet";
        }
    }
    while (caractere!='$');
}

void Objet::ChargerTexte(std::ifstream *fichier, const Caracteristique &caract, bool NePasAjouterBenedictions)
{
    m_rarete=0,m_equipe=-1;
    m_degatsMin=0,m_degatsMax=0,m_armure=0;
    m_position.y=0;
    m_position.x=0;

    char caractere;

    do
    {
        fichier->get(caractere);
        if (caractere=='e')
            *fichier>>m_equipe;

        else if (caractere=='r')
            *fichier>>m_rarete;

        else if (caractere=='x')
            *fichier>>m_position.x;
        else if (caractere=='y')
            *fichier>>m_position.y;

        else if (caractere=='d')
        {
            fichier->get(caractere);
            if (caractere=='i')
                *fichier>>m_degatsMin;
            else if (caractere=='a')
                *fichier>>m_degatsMax;
        }
        else if (caractere=='a')
            *fichier>>m_armure;
        else if (caractere=='v')
            *fichier>>m_vie;
        else if (caractere=='s')
            *fichier>>m_nbr_bless;
        else if (caractere=='l')
        {
            sf::Color m_color;
            fichier->get(caractere);
            if (caractere=='r')
                *fichier>>m_color.r;
            else if (caractere=='g')
                *fichier>>m_color.g;
            else if (caractere=='b')
                *fichier>>m_color.b;
        }

        else if (caractere=='t')
            *fichier>>m_dejaTrie;
        else if (caractere=='m')
            *fichier>>m_chemin;
        else if (caractere=='g')
            *fichier>>m_prix;
        else if (caractere=='c')
        {
            std::string temp;
            *fichier>>temp;
            if(!NePasAjouterBenedictions)
            {
                m_miracles_benedictions.push_back(Miracle (temp, caract, 0));
                if(m_useMiracle)
                    m_miracle.Concatenencer(temp,caract,0);
                else
                    m_miracle.Charger(temp,caract,0);

                m_chemin_miracles.push_back(temp);
                m_useMiracle = true;
            }
        }
        else if (caractere=='b')
        {

            int type=0,info1=0,info2=0,info3=0;

            *fichier>>type;

            do
            {
                fichier->get(caractere);
                if (caractere=='i')
                {
                    fichier->get(caractere);
                    if (caractere=='1')
                        *fichier>>info1;
                    if (caractere=='2')
                        *fichier>>info2;
                    if (caractere=='3')
                        *fichier>>info3;
                }
                if (fichier->eof())
                {
                    throw "Impossible de charger l'objet";
                }
            }
            while (caractere!='$');

            if (!NePasAjouterBenedictions)
            {
                m_benedictions.push_back(benediction ());
                m_benedictions.back().type=type;
                m_benedictions.back().info1=info1;
                m_benedictions.back().info2=info2;
                m_benedictions.back().info3=info3;

                if (m_type!=LITANIE)
                    ChargerMiracleBenediction(m_benedictions.back(),m_miracle,m_useMiracle);
            }
            caractere='_';
        }

        if (fichier->eof())
        {
            throw "Impossible de charger l'objet";
        }
    }
    while (caractere!='$');

}


void Objet::Charger(const std::string &chemin, const Caracteristique &caract,bool NePasAjouterBenedictions)
{
    console->Ajouter("",0);
    console->Ajouter("Chargement de l'objet : "+chemin,0);

    cDAT reader;
    reader.Read(configuration->chemin_items);

    ifstream *fichier=reader.GetInfos(chemin);

    if (fichier)
    {
        char caractere;
        ifstream fichier2;

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 't' :
                        *fichier>>m_type;
                        break;
                    case 'e' :
                        int temp;
                        *fichier>>temp;
                        m_emplacement.push_back(temp);
                        break;
                    case 'i' :
                        *fichier>>temp;
                        m_emplacementImpossible.push_back(temp);
                        break;
                    case 'c' :
                        m_IDClasse.push_back("");
                        m_nom_IDClasse.push_back(0);
                        *fichier>>m_IDClasse.back();

                        fichier2.open(m_IDClasse.back().c_str(),ios::in);
                        if(fichier2)
                        {
                            char caractere2;
                            do
                            {
                                fichier2.get(caractere2);
                                if (caractere2=='*')
                                    fichier2>>m_nom_IDClasse.back();
                                if (fichier2.eof())
                                    caractere2='$';
                            }
                            while (caractere2!='$');
                        }
                        fichier2.close();
                        break;

                    case 's' :
                        *fichier>>m_shoot_weapon;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'g' :
                        *fichier>>m_prix;
                        break;

                    case 's' :
                        *fichier>>m_requirement.force;
                        break;
                    case 'd' :
                        *fichier>>m_requirement.dexterite;
                        break;
                    case 'c' :
                        *fichier>>m_requirement.charisme;
                        break;
                    case 'p' :
                        *fichier>>m_requirement.piete;
                        break;
                    case 'v' :
                        *fichier>>m_requirement.vitalite;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        m_chemin=chemin;
        std::string description;

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                int no;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'n' :
                            *fichier>>no;
                            m_nom       = configuration->getText(2,no);
                            break;
                        case 'd' :
                            *fichier>>no;
                            description = configuration->getText(2,no);
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        for (int i=0;i<(int)m_nom.size();i++)
            if (m_nom[i]=='_')
                m_nom[i]=' ';

        for (int i=0;i<(int)description.size();i++)
            if (description[i]=='_')
                description[i]=' ';

        std::string::size_type stTemp = description.find('\\');

        while (stTemp !=  std::string::npos)
        {
            m_description.push_back(description.substr(0, stTemp)+'\n');
            description = description.substr(stTemp + 1);
            stTemp = description.find('\\');
        }
        m_description.push_back(description.substr(0, stTemp)+'\n');


        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                //getline(fichier, temp);
                *fichier>>temp;

                m_image=moteurGraphique->AjouterImage(temp,-1);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;

                m_son=moteurSons->AjouterBuffer(temp);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {

                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'e' :
                        int temp;
                        *fichier>>temp;
                        m_emplacementImageHero.push_back(temp);
                        break;
                    case 'm' :
                        string temp2;
                        *fichier>>temp2;
                        m_cheminImageHero.push_back(temp2);
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');




        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        *fichier>>m_positionImage.x;
                        break;
                    case 'y' :
                        *fichier>>m_positionImage.y;
                        break;
                    case 'w' :
                        *fichier>>m_positionImage.w;
                        break;
                    case 'h' :
                        *fichier>>m_positionImage.h;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'x' :
                        *fichier>>m_taille.x;
                        break;
                    case 'y' :
                        *fichier>>m_taille.y;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');


        do
        {

            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'r' :
                        *fichier>>m_rarete;
                        break;
                    case 'b' :
                        *fichier>>m_capaciteBenediction;
                        break;
                    case 's' :
                        *fichier>>m_chemin_set;
                        break;
                    case 't' :
                        *fichier>>m_time;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        m_set = Set ();
        if(!m_chemin_set.empty())
            m_set.Charger(m_chemin_set, caract);

        this->ChargerCaracteristiques(fichier);


        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;
                m_miracle = Miracle ();
                m_miracle.Charger(temp, caract, 0);
                m_chemin_miracles.push_back(temp);
                m_useMiracle=true;
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {

            fichier->get(caractere);
            if (caractere=='*')
            {
                int b=-1,ia=-1,ib=-1, ic=-1;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                    case 'b' :
                        *fichier>>b;
                        break;
                    case 'i' :
                        fichier->get(caractere);
                        if (caractere=='1')
                            *fichier>>ia;
                        if (caractere=='2')
                            *fichier>>ib;
                        if (caractere=='3')
                            *fichier>>ic;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                if (!NePasAjouterBenedictions)
                {
                    m_benedictions.push_back(benediction ());
                    m_benedictions.back().type=b;
                    m_benedictions.back().info1=ia;
                    m_benedictions.back().info2=ib;
                    m_benedictions.back().info3=ic;

                    if (m_type!=LITANIE)
                        ChargerMiracleBenediction(m_benedictions.back(),m_miracle,m_useMiracle);
                }

                fichier->get(caractere);
            }

            if (fichier->eof())
            {
                caractere='$';
            }

        }
        while (caractere!='$');

        fichier->close();
    }
    else
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);

    delete fichier;
}

void Set::Charger(std::string chemin, Caracteristique caract)
{
    m_chemin = chemin;

    cDAT reader;
    reader.Read(configuration->chemin_items);

    ifstream *fichier=reader.GetInfos(chemin);

    if (fichier)
    {
        char caractere;

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                int no;
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'n' :
                            *fichier>>no;
                            m_nom       = configuration->getText(2,no);
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                m_items.push_back("");
                *fichier>>m_items.back();
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                m_benedictions.push_back(std::vector<benediction> ());
                m_chemin_miracles.push_back(std::vector<std::string> ());
                m_miracle.push_back(Miracle ());
                m_useMiracle.push_back(false);

                do
                {
                    fichier->get(caractere);
                    if (caractere=='*')
                    {
                        string temp;
                        *fichier>>temp;
                        m_miracle.back().Charger(temp, caract, 0);
                        m_chemin_miracles.back().push_back(temp);
                        m_useMiracle.back()=true;
                    }
                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                        caractere='$';
                    }
                }
                while (caractere!='$');


                do
                {

                    fichier->get(caractere);
                    if (caractere=='*')
                    {
                        int b=-1,ia=-1,ib=-1, ic=-1;
                        do
                        {
                            fichier->get(caractere);
                            switch (caractere)
                            {
                            case 'b' :
                                *fichier>>b;
                                break;
                            case 'i' :
                                fichier->get(caractere);
                                if (caractere=='1')
                                    *fichier>>ia;
                                if (caractere=='2')
                                    *fichier>>ib;
                                if (caractere=='3')
                                    *fichier>>ic;
                                break;
                            }

                            if (fichier->eof())
                            {
                                console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                                caractere='$';
                            }

                        }
                        while (caractere!='$');


                        m_benedictions.back().push_back(benediction ());
                        m_benedictions.back().back().type=b;
                        m_benedictions.back().back().info1=ia;
                        m_benedictions.back().back().info2=ib;
                        m_benedictions.back().back().info3=ic;

                        bool useMir = m_useMiracle.back();
                        ChargerMiracleBenediction(m_benedictions.back().back(),m_miracle.back(),useMir);
                        m_useMiracle.back() = useMir;

                      /*  if (m_benedictions.back().back().type==DEGATS_SUPP)
                        {
                            if(ia == FEU)
                            {
                                m_chemin_miracles.back().push_back("Data/Items/Miracles/FireEffect.miracle.hs");
                                if (!m_useMiracle.back())
                                    m_miracle.back().Charger("Data/Items/Miracles/FireEffect.miracle.hs",caract,0),m_useMiracle.back()=true;
                                else
                                    m_miracle.back().Concatenencer("Data/Items/Miracles/FireEffect.miracle.hs",caract,0);
                            }
                            if(ia == FOI)
                            {
                                m_chemin_miracles.back().push_back("Data/Items/Miracles/HolyEffect.miracle.hs");
                                if (!m_useMiracle.back())
                                    m_miracle.back().Charger("Data/Items/Miracles/HolyEffect.miracle.hs",caract,0),m_useMiracle.back()=true;
                                else
                                    m_miracle.back().Concatenencer("Data/Items/Miracles/HolyEffect.miracle.hs",caract,0);
                            }
                            if(ia == CORROSION)
                            {
                                m_chemin_miracles.back().push_back("Data/Items/Miracles/PoisonEffect.miracle.hs");
                                if (!m_useMiracle.back())
                                    m_miracle.back().Charger("Data/Items/Miracles/PoisonEffect.miracle.hs",caract,0),m_useMiracle.back()=true;
                                else
                                    m_miracle.back().Concatenencer("Data/Items/Miracles/PoisonEffect.miracle.hs",caract,0);
                            }
                        }*/

                        fichier->get(caractere);
                    }

                    if (fichier->eof())
                    {
                        caractere='$';
                    }
                }
                while (caractere!='$');
                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+chemin+" \" Invalide",1);
                caractere='$';
            }
        }
        while (caractere!='$');


        fichier->close();
    }
    else
        console->Ajouter("Impossible d'ouvrir : "+chemin,1);

    delete fichier;
}

void Objet::ChargerMiracle(const Caracteristique &caract)
{
    m_miracle = Miracle();

    m_useMiracle = false;

    for(unsigned i = 0 ; i < m_chemin_miracles.size() ; ++i)
    {
        if(m_useMiracle)
            m_miracle.Concatenencer(m_chemin_miracles[i], caract, 0);
        else
            m_miracle.Charger(m_chemin_miracles[i], caract, 0);

        m_useMiracle = true;
    }

    for(unsigned i = 0 ; i < m_benedictions.size() ; ++i)
        ChargerMiracleBenediction(m_benedictions[i],m_miracle,m_useMiracle);
}

void Objet::Generer(int bonus)
{
    if(va - vi + 1 != 0)
        m_vie=(rand() % (va - vi + 1)) + vi;
    else
        m_vie = 0;

    if(aa - ai + 1 != 0)
        m_armure=(rand() % (aa - ai + 1)) + ai;
    else
        m_armure = 0;

    if(dia - dii + 1 != 0)
        m_degatsMin=(rand() % (dia - dii + 1)) + dii;
    else
        m_degatsMin = 0;

    if(daa - dai + 1 != 0)
        m_degatsMax=(rand() % (daa - dai + 1)) + dai;
    else
        m_degatsMax = 0;

    if(ba > bi)
        m_nbr_bless = (rand() % (ba - bi + 1)) + bi;
    else
        m_nbr_bless = 0;

    if (m_type != CONSOMMABLE && m_capaciteBenediction > 0)
        if (m_rarete<DIVIN || m_type == LITANIE)
        {
            int nbrBene=0;
            int rarete=NORMAL;

            if(bonus != 0)
            {
                int random=rand()%10000 / bonus;
                if (random<=1500)
                    rarete=BONNEFACTURE;

                if (random<=300)
                    rarete=BENI;

                if (random<=30)
                    rarete=SACRE;

                if (random<3)
                    rarete=SANCTIFIE;

                if (rarete<m_rarete)
                    rarete=m_rarete;
            }

            if(m_type != LITANIE)
            {
                m_rarete=rarete;

                if (m_rarete==BONNEFACTURE)
                    nbrBene=1;

                if (m_rarete==BENI)
                    nbrBene=rand()%(4-2)+2;

                if (m_rarete==SACRE)
                    nbrBene=rand()%(9-5)+5;

                if (m_rarete==SANCTIFIE)
                    nbrBene=rand()%(15-10)+10;
            }
            else
                nbrBene = 1;

            int accru = 0;

            for (int i=0;i<nbrBene;i++)
            {
                benediction temp;
                temp.type=rand()%NOMBRE_BENEDICTION;

                temp.info1=0;
                temp.info2=0;
                temp.info3=0;

                if (temp.type==POINTS_SUPP)
                {
                    if(rand()%100 < 50)
                        temp.info1 = PT_VIE;
                    else
                        temp.info1 = PT_FOI;

                    temp.info2=rand()%(m_capaciteBenediction*15 - (int)(m_capaciteBenediction*5) + 1)+m_capaciteBenediction*5;
                    //m_prix += (int)((float)prixor*0.5*(temp.info2)/(m_capaciteBenediction*10 - m_capaciteBenediction*3));
                }
                else if (temp.type==EFFICACITE_ACCRUE&&(m_type==ARME||m_type==ARMURE||m_type==LITANIE))
                {
                    temp.info1=(int)(rand()%(m_capaciteBenediction*10 - (int)((float)m_capaciteBenediction*2.5) + 1)+(float)m_capaciteBenediction*2.5);
                    //m_prix += (int)((float)prixor*0.5*(temp.info1)/(m_capaciteBenediction*10 - m_capaciteBenediction*2.5));
                }
                else if (temp.type==DEGATS_SUPP&&(m_type==ARME||m_type==LITANIE))
                {
                    int random = rand()%100;
                    if(random < 25)
                        temp.info1 = PHYSIQUE;
                    if(random >= 25 && random < 50)
                        temp.info1 = FEU;
                    if(random >= 50 && random < 75)
                        temp.info1 = FOI;
                    if(random >= 75)
                        temp.info1 = CORROSION;

                    temp.info2=(int)(rand()%((int)((float)m_capaciteBenediction*1.5) - (int)((float)m_capaciteBenediction*0.5) + 1)+(float)m_capaciteBenediction*0.5);
                    temp.info3=(int)(rand()%((int)((float)m_capaciteBenediction*2.5) - (int)((float)m_capaciteBenediction*1.5) + 1)+(float)m_capaciteBenediction*1.5);
                   // m_prix += (int)((float)prixor*0.5*(temp.info2+temp.info3)/(m_capaciteBenediction*2 - m_capaciteBenediction*0.5));
                }
                else if (temp.type==DEGATS_TEMPS_SUPP&&(m_type==ARME||m_type==LITANIE))
                {
                    int random = rand()%100;
                    if(random < 25)
                        temp.info1 = PHYSIQUE;
                    if(random >= 25 && random < 50)
                        temp.info1 = FEU;
                    if(random >= 50 && random < 75)
                        temp.info1 = FOI;
                    if(random >= 75)
                        temp.info1 = CORROSION;

                    temp.info3=(int)(rand()%((int)((float)m_capaciteBenediction*1.5) - (int)((float)m_capaciteBenediction*0.5) + 1)+(float)m_capaciteBenediction*0.5);
                    temp.info2=(int)(rand()%((int)((float)m_capaciteBenediction*2.5) - (int)((float)m_capaciteBenediction*1.5) + 1)+(float)m_capaciteBenediction*1.5);
                   // m_prix += (int)((float)prixor*0.5*(temp.info2+temp.info3)/(m_capaciteBenediction*2 - m_capaciteBenediction*0.5));
                }
                else if (temp.type==ARMURE_SUPP&&(m_type==ARMURE||m_type==LITANIE))
                {
                    int random = rand()%100;
                    if(random < 25)
                        temp.info1 = PHYSIQUE;
                    if(random >= 25 && random < 50)
                        temp.info1 = FEU;
                    if(random >= 50 && random < 75)
                        temp.info1 = FOI;
                    if(random >= 75)
                        temp.info1 = CORROSION;

                    temp.info2=(int)(rand()%((int)((float)m_capaciteBenediction*10) - (int)((float)m_capaciteBenediction*5) + 1)+(float)m_capaciteBenediction*5);
                    //m_prix += (int)((float)prixor*0.1*(temp.info2)/(m_capaciteBenediction*2 - m_capaciteBenediction*0.5));
                }
                else if (temp.type==CARACT_SUPP)
                {
                    int random = rand()%100;
                    if(random < 20)
                        temp.info1 = FORCE;
                    if(random >= 20 && random < 40)
                        temp.info1 = DEXTERITE;
                    if(random >= 40 && random < 60)
                        temp.info1 = VITALITE;
                    if(random >= 60 && random < 80)
                        temp.info1 = PIETE;
                    if(random >= 80)
                        temp.info1 = CHARISME;

                    temp.info2=(int)(rand()%(m_capaciteBenediction - (int)((float)m_capaciteBenediction*0.25) + 1)+(float)m_capaciteBenediction*0.25);
                    if(temp.info2 <= 0)
                        temp.info2 = 1;
                    //m_prix += (int)((float)prixor*0.5*(temp.info2)/(m_capaciteBenediction - m_capaciteBenediction*0.25));
                }
                else
                {
                    temp.info1=(int)(rand()%(m_capaciteBenediction - (int)((float)m_capaciteBenediction*0.25) + 1)+(float)m_capaciteBenediction*0.25);
                    //m_prix += (int)((float)prixor*0.5*(temp.info1)/(m_capaciteBenediction - m_capaciteBenediction*0.25));
                }

                if ((temp.type==EFFICACITE_ACCRUE&&!(m_type==ARME||m_type==ARMURE||m_type==LITANIE))||
                    (temp.type==DEGATS_SUPP&&(m_type!=ARME&&m_type!=LITANIE))||
                    (temp.type==DEGATS_TEMPS_SUPP&&(m_type!=ARME&&m_type!=LITANIE))||
                    (temp.type==ARMURE_SUPP&&(m_type!=ARMURE&&m_type!=LITANIE)))
                    i--;
                else
                    accru += AddBenediction(temp);
            }

            m_prix = 0;

            if(aa - ai != 0)
                m_prix += ((m_armure + accru * m_armure / 100) * 2);
            if(dia - dii != 0)
                m_prix += ((m_degatsMin + accru * m_degatsMin / 100) * 10);
            if(daa - dai != 0)
                m_prix += ((m_degatsMax + accru * m_degatsMax / 100) * 10);

            if(m_prix == 0)
                m_prix = 200;

            for (int j=0;j<(int)m_benedictions.size();j++)
            {
                if(m_benedictions[j].type == DEGATS_SUPP)
                    m_prix += (int)((float)50*(m_benedictions[j].info2+m_benedictions[j].info3));
                if(m_benedictions[j].type == DEGATS_TEMPS_SUPP)
                    m_prix += (int)((float)10*(m_benedictions[j].info2*m_benedictions[j].info3));
                if(m_benedictions[j].type == ARMURE_SUPP)
                    m_prix += (int)((float)10*(m_benedictions[j].info2));
            }

            float multiplicateur_car = 1;

            for (int j=0;j<(int)m_benedictions.size();j++)
            {
                if(m_benedictions[j].type == CARACT_SUPP)
                    multiplicateur_car += (float)m_benedictions[j].info2 * 0.5;
                if(m_benedictions[j].type == POINTS_SUPP)
                    m_prix += (int)((float)m_benedictions[j].info2 * 0.1);
            }

            m_prix = (int)((float)m_prix * multiplicateur_car);
        }
}

int Objet::AddBenediction(benediction temp)
{
    bool ajouter = true;
    int accru = 0;

    for (int j=0;j<(int)m_benedictions.size();j++)
        if (m_benedictions[j].type==temp.type)
        {
            if(m_benedictions[j].info1==temp.info1 && temp.type != EFFICACITE_ACCRUE)
                m_benedictions[j].info2+=temp.info2,m_benedictions[j].info3+=temp.info3,ajouter=false;
            if(temp.type == EFFICACITE_ACCRUE)
                m_benedictions[j].info1+=temp.info1,ajouter=false,accru=m_benedictions[j].info1;
        }


    if (ajouter)
    {
        m_benedictions.push_back(benediction ());
        m_benedictions.back()=temp;

        if(m_type!=LITANIE)
            ChargerMiracleBenediction(temp,m_miracle,m_useMiracle);
    }

    return (accru);
}

void Objet::ChargerCaracteristiques(std::ifstream *fichier)
{
    if (m_type==ARME || m_type==GOLEM)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'd' :
                            fichier->get(caractere);
                            if (caractere=='i')
                            {
                                fichier->get(caractere);
                                if (caractere=='i')
                                    *fichier>>dii;
                                if (caractere=='a')
                                    *fichier>>dia;
                            }
                            else if (caractere=='a')
                            {
                                fichier->get(caractere);
                                if (caractere=='i')
                                    *fichier>>dai;
                                if (caractere=='a')
                                    *fichier>>daa;
                            }
                            break;
                        case 'b' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>bi;
                            if (caractere=='a')
                                *fichier>>ba;
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');
                fichier->get(caractere);



            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }

    if (m_type==ARMURE || m_type==GOLEM)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'a' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>ai;
                            if (caractere=='a')
                                *fichier>>aa;
                            break;

                        case 'b' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>bi;
                            if (caractere=='a')
                                *fichier>>ba;
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }

    if (m_type==GOLEM)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'v' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>vi;
                            if (caractere=='a')
                                *fichier>>va;
                            break;

                        case 'b' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>bi;
                            if (caractere=='a')
                                *fichier>>ba;
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }

    if (m_type==JEWELERY)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'b' :
                            fichier->get(caractere);
                            if (caractere=='i')
                                *fichier>>bi;
                            if (caractere=='a')
                                *fichier>>ba;
                            break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }

    if (m_type==SCHEMA)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                m_craft_ingredients.push_back(Ingredient ());
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'm' :
                            (*fichier)>>m_craft_ingredients.back().nom;
                        break;

                        case 'n' :
                            (*fichier)>>m_craft_ingredients.back().nombre;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                Objet temp;
                Caracteristique temp2;
                temp.Charger(m_craft_ingredients.back().nom, temp2, true);
                m_craft_ingredients.back().text_nom = temp.m_nom;

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 'm' :
                            (*fichier)>>m_craft_result;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }

    if (m_type==LITANIE)
    {
        char caractere;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                do
                {
                    fichier->get(caractere);
                    switch (caractere)
                    {
                        case 't' :
                            m_conditions.push_back(ConditionLitanie ());
                            m_conditions.back().type = L_TYPE;
                            (*fichier)>>m_conditions.back().valeur;
                        break;
                        case 'e' :
                            m_conditions.push_back(ConditionLitanie ());
                            m_conditions.back().type = L_EMPLACEMENT;
                            (*fichier)>>m_conditions.back().valeur;
                        break;
                        case 'm' :
                            m_conditions.push_back(ConditionLitanie ());
                            m_conditions.back().type = L_NOM;
                            (*fichier)>>m_conditions.back().valeur_string;
                        break;
                    }

                    if (fichier->eof())
                    {
                        console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                        caractere='$';
                    }

                }
                while (caractere!='$');

                fichier->get(caractere);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');
    }
}


sf::Text Objet::AjouterCaracteristiqueAfficher(coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color)
{
    sf::Text string;

    string.SetFont(moteurGraphique->m_font);

    string.SetColor(color);

    string.SetCharacterSize(14);
    string.SetString(chaine);

    if (tailleCadran->x<(int)string.GetRect().Width)
        tailleCadran->x=(int)string.GetRect().Width;

    decalage->y+=(int)string.GetRect().Height+2;

    return string;
}

std::string getTextBenediction(const benediction &bene)
{
    std::ostringstream buf;
    if(bene.type == EFFICACITE_ACCRUE)
    {
        buf<<configuration->getText(1,0);
        buf<<" "<<bene.info1<<" %";
    }
    if(bene.type == CARACT_SUPP)
    {
        if(bene.info1 == FORCE)
            buf<<configuration->getText(1,1);
        if(bene.info1 == DEXTERITE)
            buf<<configuration->getText(1,2);
        if(bene.info1 == VITALITE)
            buf<<configuration->getText(1,3);
        if(bene.info1 == PIETE)
            buf<<configuration->getText(1,4);
        if(bene.info1 == CHARISME)
            buf<<configuration->getText(1,5);
        buf<<" "<<bene.info2;
    }
    if(bene.type == POINTS_SUPP)
    {
        if(bene.info1 == PT_VIE)
            buf<<configuration->getText(1,6);
        if(bene.info1 == PT_FOI)
            buf<<configuration->getText(1,7);
        buf<<" "<<bene.info2;
    }
    if(bene.type == DEGATS_SUPP)
    {
        buf<<configuration->getText(1,8 + bene.info1);
        buf<<" "<<bene.info2<<" - "<<bene.info3;
    }
    if(bene.type == DEGATS_TEMPS_SUPP)
    {
        buf<<configuration->getText(1,16 + bene.info1);
        buf<<" "<<bene.info2<<" "<<configuration->getText(1,20)<<" "<<bene.info3<<" s";
    }
    if(bene.type == ARMURE_SUPP)
    {
        buf<<configuration->getText(1,12 + bene.info1);
        buf<<" "<<bene.info2;
    }
    return buf.str();
}

void Objet::Afficher(coordonnee position)
{
    sf::Sprite sprite;
    sprite.SetImage(*moteurGraphique->getImage(m_image));
    sprite.SetSubRect(IntRect(m_positionImage.x,
                              m_positionImage.y,
                              m_positionImage.w,
                              m_positionImage.h));
    if(m_hauteur < 1)
        sprite.SetScale(0.8f,0.4f);
    else
        sprite.SetScale(0.6f,0.6f);

    sprite.SetOrigin(   m_positionImage.w * 0.5,
                        m_positionImage.h * 0.5);

    sprite.SetX(position.x-32+m_position.x*32+16-(m_positionImage.w*0.8f)/2);
    sprite.SetY(position.y+m_position.y*32 - m_hauteur);

    sprite.SetRotation(m_rotation);

    if(m_hauteur > 16)
        moteurGraphique->AjouterCommande(&sprite,10,1);
    else
        moteurGraphique->AjouterCommande(&sprite,8,1);
}

int Objet::AfficherCaracteristiques(coordonnee position,Caracteristique caract, std::vector<Objet> *items, std::string nom_classe,float modPrix,bool compare,bool decalageDroite, bool orientationHaut, bool coffre)
{
    std::vector <sf::Text> temp;

    sf::Sprite sprite;
    sf::Text string;

    coordonnee tailleCadran,decalage(-10,0);

    if (m_equipe>=0&&compare)
    {
        /*temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,configuration->getText(0,18).c_str()));
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,"---------------"));
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));*/
    }
    else if (compare)
    {
        if(coffre)
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,configuration->getText(0,63).c_str()));
        else if (decalageDroite)
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,configuration->getText(0,19).c_str()));
        else
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,configuration->getText(0,20).c_str()));
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,"---------------"));
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));
    }

    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,m_nom.c_str()));
    temp.back().SetColor(GetItemColor(m_rarete));
    temp.back().SetStyle(4);
    temp.back().SetCharacterSize(14);
    if(!m_chemin_set.empty())
    {
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,m_set.m_nom.c_str()));
        temp.back().SetColor(GetItemColor(m_rarete));

        m_set.m_nombre = -1;

        for(unsigned i = 0 ; i < m_set.m_items.size() ; ++i)
        {
            Objet tempobj;
            tempobj.Charger(m_set.m_items[i], caract);
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,tempobj.m_nom.c_str()));
            temp.back().SetStyle(2);

            bool ok = false;
            if(items != NULL)
                for(unsigned j = 0 ; j < items->size() ; ++j)
                    if((*items)[j].m_chemin == m_set.m_items[i] && (*items)[j].m_equipe >= 0)
                        ok = true;
            if(ok)
                temp.back().SetColor(GetItemColor(tempobj.m_rarete)), m_set.m_nombre++;
            else
                temp.back().SetColor(sf::Color(128,128,128));
        }
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));
    }


    for (int i=0;i<(int)m_description.size();i++)
    {
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,m_description[i].c_str()));
        temp.back().SetStyle(2);
    }

    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));

    if(!m_nom_IDClasse.empty())
    {
        bool mauvais = true;
        for(unsigned i = 0 ; i < m_nom_IDClasse.size() ; ++i)
        {
            std::string buf;
            buf = configuration->getText(0,52) + configuration->getText(3, m_nom_IDClasse[i]);
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.c_str()));
            if(m_IDClasse[i] == nom_classe)
                mauvais = false;
        }

        if(mauvais)
            for(unsigned i = 1 ; i <= m_nom_IDClasse.size() ; ++i)
                temp[temp.size() - i].SetColor(sf::Color(192,0,0));

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));
    }

    if(m_time > 0)
    {
        int sec = m_time%60;
        int min = (int)(m_time/60);

        std::ostringstream buf;
        buf<<configuration->getText(0,69);

        if(min > 0)
            buf<<min<<" "<<configuration->getText(0,71)<<" ";
        if(sec > 0)
            buf<<sec<<" "<<configuration->getText(0,70);
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
    }


    if(!m_craft_ingredients.empty())
    {
        for (int i=0;i<(int)m_craft_ingredients.size();i++)
        {
            std::ostringstream buf;
            buf<<"- "<<m_craft_ingredients[i].nombre<<" "<<m_craft_ingredients[i].text_nom;
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
            //temp.back().SetStyle(2);
        }

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));
    }


    float multiplieurEfficacite=100;

    for (int i=0;i<(int)m_benedictions.size();i++)
        if (m_benedictions[i].type==EFFICACITE_ACCRUE)
            multiplieurEfficacite+=m_benedictions[i].info1;


    if(m_type == ARME)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,21)<<(int)(m_degatsMin*multiplieurEfficacite/100)<<" - "<<(int)(m_degatsMax*multiplieurEfficacite/100);

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (multiplieurEfficacite!=100)
            temp.back().SetColor(sf::Color(0,128,255));
    }

    if(m_type == ARMURE)
    {
        if(m_armure > 0)
        {
            std::ostringstream buf;
            buf<<configuration->getText(0,22)<<(int)(m_armure*multiplieurEfficacite/100);

            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));

            if (multiplieurEfficacite!=100)
                temp.back().SetColor(sf::Color(0,128,255));
        }
    }

    if(m_type == GOLEM)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,3)<<" : "<<m_gol_caract.maxVie;
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));

        buf.str("");
        buf<<configuration->getText(0,21)<<m_gol_caract.degatsMin[0]<<" - "<<m_gol_caract.degatsMax[0];
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));

        buf.str("");
        buf<<configuration->getText(0,22)<<m_gol_caract.armure[0];
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
    }


    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran," "));


    if (m_requirement.force>0)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,23)<<m_requirement.force;

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (caract.force<m_requirement.force)
            temp.back().SetColor(sf::Color(192,0,0));
    }
    if (m_requirement.dexterite>0)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,24)<<m_requirement.dexterite;

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (caract.dexterite<m_requirement.dexterite)
            temp.back().SetColor(sf::Color(192,0,0));
    }
    if (m_requirement.charisme>0)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,25)<<m_requirement.charisme;

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (caract.charisme<m_requirement.charisme)
            temp.back().SetColor(sf::Color(192,0,0));
    }
    if (m_requirement.vitalite>0)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,26)<<m_requirement.vitalite;

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (caract.vitalite<m_requirement.vitalite)
            temp.back().SetColor(sf::Color(192,0,0));
    }
    if (m_requirement.piete>0)
    {
        std::ostringstream buf;
        buf<<configuration->getText(0,27)<<m_requirement.piete;

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
        if (caract.piete<m_requirement.piete)
            temp.back().SetColor(sf::Color(192,0,0));
    }

    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));

    for (int i=0;i<(int)m_benedictions.size();i++)
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,getTextBenediction(m_benedictions[i]).c_str(),sf::Color(0,128,255)));

    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));

    for(unsigned i = 0 ; i < m_miracles_benedictions.size() ; ++i)
    {
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,m_miracles_benedictions[i].m_nom.c_str(),sf::Color(128,64,0)));
        for(unsigned j =  0 ; j < m_miracles_benedictions[i].m_description.size() ; ++j)
        {
            temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,m_miracles_benedictions[i].m_description[j].c_str(),sf::Color(128,64,0)));
            temp.back().SetStyle(2);
        }

    }

    if(!m_set.m_chemin.empty())
    {
        for(int k = 0 ; k < m_set.m_nombre ; ++k)
            for (int i=0;i<(int)m_set.m_benedictions[k].size();i++)
                temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,getTextBenediction(m_set.m_benedictions[k][i]).c_str(),GetItemColor(5)));

        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));
    }

    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));

    for(int i = 0 ; i < m_nbr_bless ; ++i)
    {
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,configuration->getText(0,60).c_str(),sf::Color(128,128,128)));
        temp.back().SetStyle(2);
    }


    temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,""));


    {
        std::ostringstream buf;
        buf<<configuration->getText(0,28)<<(int)((float)m_prix*modPrix);

        if(modPrix != 1)
            buf<<" ("<< m_prix <<")";
        temp.push_back(AjouterCaracteristiqueAfficher(&decalage,&tailleCadran,buf.str().c_str()));
    }

    if (position.x-tailleCadran.x-10<position.x&&decalageDroite)
        position.x=position.x+tailleCadran.x-10;

    if (position.x-tailleCadran.x-10<0)
        position.x=tailleCadran.x+10;

    if (position.y-decalage.y-10<0 && orientationHaut)
        position.y=decalage.y+10;

    if (position.y+decalage.y+20>configuration->Resolution.h && !orientationHaut)
        position.y=configuration->Resolution.h-decalage.y-20;

    if (position.x+decalage.x+20>configuration->Resolution.w)
        position.x=configuration->Resolution.w-decalage.x-20;


    if(orientationHaut)
    {
        int decalY=0;
        int decalY2=20;
        for (int i=0;i<(int)temp.size();i++)
            decalY2+=(int)temp[i].GetRect().Height+2;

        position.y -= decalY2;

        for (int i=0;i<(int)temp.size();i++)
        {
            temp[i].SetY((position.y+decalY+10));
            temp[i].SetX(position.x+(tailleCadran.x-(int)temp[i].GetRect().Width)*0.5-tailleCadran.x);

            decalY+=(int)temp[i].GetRect().Height+2;

            moteurGraphique->AjouterTexte(&temp[i],19);
        }
    }
    else
    {
        int decalY=0;
        for (int i=0;i<(int)temp.size();i++)
        {
            temp[i].SetY((position.y+decalY+10));
            temp[i].SetX(position.x+(tailleCadran.x-(int)temp[i].GetRect().Width)*0.5-tailleCadran.x);

            decalY+=(int)temp[i].GetRect().Height+2;

            moteurGraphique->AjouterTexte(&temp[i],19);
        }
    }
    tailleCadran.y=decalage.y;

    tailleCadran.y+=20;
    tailleCadran.x+=20;

    sprite.SetImage(*moteurGraphique->getImage(0));
    /*if (surbrillance)
        sprite.SetColor(sf::Color(16,16,16,224));
    else*/
        sprite.SetColor(sf::Color(0,0,0,224));

    sprite.SetY(position.y);
    sprite.SetX(position.x-tailleCadran.x+10);
    sprite.Resize(tailleCadran.x,tailleCadran.y);
    moteurGraphique->AjouterCommande(&sprite,19,0);

    sf::Sprite sprite2;

    sprite2.SetImage(*moteurGraphique->getImage(moteurGraphique->m_img_corner));
    sprite2.Resize(16,16);
    sprite2.SetColor(sf::Color(255,255,255,255));
    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x-tailleCadran.x+10-2);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    sprite2.SetY(position.y-2);
    sprite2.SetX(position.x+10+2);
    sprite2.SetRotation(270);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x+10+2);
    sprite2.SetRotation(180);
    moteurGraphique->AjouterCommande(&sprite2,19,0);

    sprite2.SetY(position.y+tailleCadran.y+2);
    sprite2.SetX(position.x+-tailleCadran.x+10-2);
    sprite2.SetRotation(90);
    moteurGraphique->AjouterCommande(&sprite2,19,0);


    temp.clear();

    if (decalageDroite)
        return (position.x+20);

    return (position.x-tailleCadran.x);
}

void Objet::CalculerGolem()
{
    m_gol_caract.maxVie                 = m_vie;

    m_gol_caract.force                  = 0;
    m_gol_caract.dexterite              = 0;
    m_gol_caract.vitalite               = 0;
    m_gol_caract.piete                  = 0;
    m_gol_caract.charisme               = 0;
    m_gol_caract.degatsMin[PHYSIQUE]    = 0;
    m_gol_caract.degatsMax[PHYSIQUE]    = 0;
    m_gol_caract.armure[PHYSIQUE]       = 0;

    int multiplieurEfficacite = 100;

    for(unsigned i = 0 ; i < m_benedictions.size() ; ++i)
    {
         if (m_benedictions[i].type==ARMURE_SUPP)
                m_gol_caract.armure[m_benedictions[i].info1]+=m_benedictions[i].info2;
         if (m_benedictions[i].type==CARACT_SUPP)
         {
            if(m_benedictions[i].info1 == FORCE)
                m_gol_caract.force    += m_benedictions[i].info2;
            if(m_benedictions[i].info1 == DEXTERITE)
                m_gol_caract.dexterite+= m_benedictions[i].info2;
            if(m_benedictions[i].info1 == VITALITE)
                m_gol_caract.vitalite += m_benedictions[i].info2;
            if(m_benedictions[i].info1 == PIETE)
                m_gol_caract.piete    += m_benedictions[i].info2;
            if(m_benedictions[i].info1 == CHARISME)
                m_gol_caract.charisme += m_benedictions[i].info2;
         }
         if (m_benedictions[i].type==POINTS_SUPP)
         {
            if(m_benedictions[i].info1 == PT_VIE)
                m_gol_caract.maxVie   += m_benedictions[i].info2;
            if(m_benedictions[i].info1 == PT_FOI)
                m_gol_caract.maxFoi   += m_benedictions[i].info2;
         }
         if (m_benedictions[i].type==EFFICACITE_ACCRUE)
                multiplieurEfficacite += m_benedictions[i].info1;
    }

    m_gol_caract.maxVie += m_gol_caract.vitalite * 25;
    m_gol_caract.maxFoi += m_gol_caract.piete * 25;

    m_gol_caract.vie = m_gol_caract.maxVie;
    m_gol_caract.foi = m_gol_caract.maxFoi;

    m_gol_caract.degatsMin[PHYSIQUE]    += m_degatsMin  * multiplieurEfficacite * (m_gol_caract.force * 10 + 100) / 10000;
    m_gol_caract.degatsMax[PHYSIQUE]    += m_degatsMax  * multiplieurEfficacite * (m_gol_caract.force * 10 + 100) / 10000;
    m_gol_caract.armure[PHYSIQUE]       += m_armure     * multiplieurEfficacite * (m_gol_caract.dexterite * 10 + 100) / 10000;
}


bool Objet::Utilisable(const Caracteristique &caract,std::string IDClasse)
{
    if (m_requirement.force<=caract.force)
        if (m_requirement.dexterite<=caract.dexterite)
            if (m_requirement.charisme<=caract.charisme)
                if (m_requirement.vitalite<=caract.vitalite)
                    if (m_requirement.piete<=caract.piete)
                    {
                        for (int i=0;i<(int)m_IDClasse.size();i++)
                            if (m_IDClasse[i]==IDClasse)
                                return true;
                        if (m_IDClasse.empty())
                            return true;
                    }
    return false;
}

void Objet::JouerSon()
{
    moteurSons->JouerSon(m_son,coordonnee (-1,-1,-1,-1));
}


void Objet::setChanceTrouver(float chance)
{
    m_chanceTrouver=chance;
}

float Objet::getChanceTrouver()
{
    return m_chanceTrouver;
}
int Objet::getPrix()
{
    return m_prix;
}

