

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
        return (sf::Color(96,0,96,255));
    if (rarete==BENI)
        return (sf::Color(0,64,128,255));
    if (rarete==SACRE)
        return (sf::Color(255,255,128,255));
    if (rarete==SANCTIFIE)
        return (sf::Color(128,255,255,255));
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

    ai=0,aa=0,dii=0,dia=0,dai=0,daa=0;
    m_shoot_weapon=0;

    m_useMiracle = false;
    m_alpha = 0;

    m_dejaTrie = false;
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


    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='r';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.r, sizeof(Uint8));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='g';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.g, sizeof(Uint8));
    //fichier->write((char*)&espace, sizeof(char));

    caractere='l';
    fichier->write((char*)&caractere, sizeof(char));
    caractere='b';
    fichier->write((char*)&caractere, sizeof(char));
    fichier->write((char*)&m_color.b, sizeof(Uint8));
    // fichier->write((char*)&espace, sizeof(char));

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

    *fichier<<" lr"<<m_color.r;
    *fichier<<" lg"<<m_color.g;
    *fichier<<" lb"<<m_color.b;

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

        else if (caractere=='l')
        {
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


                if (m_benedictions.back().type==DEGATS_SUPP)
                {
                    if(info1 == FEU)
                    {
                        m_chemin_miracles.push_back("Data/Items/FireEffect/FireEffect.miracle.hs");
                        if (!m_useMiracle)
                            m_miracle.Charger("Data/Items/FireEffect/FireEffect.miracle.hs",caract,0),m_useMiracle=true;
                        else
                            m_miracle.Concatenencer("Data/Items/FireEffect/FireEffect.miracle.hs",caract,0);
                    }
                    if(info1 == FOI)
                    {
                        m_chemin_miracles.push_back("Data/Items/HolyEffect/HolyEffect.miracle.hs");
                        if (!m_useMiracle)
                            m_miracle.Charger("Data/Items/HolyEffect/HolyEffect.miracle.hs",caract,0),m_useMiracle=true;
                        else
                            m_miracle.Concatenencer("Data/Items/HolyEffect/HolyEffect.miracle.hs",caract,0);
                    }
                    if(info1 == CORROSION)
                    {
                        m_chemin_miracles.push_back("Data/Items/PoisonEffect/PoisonEffect.miracle.hs");
                        if (!m_useMiracle)
                            m_miracle.Charger("Data/Items/PoisonEffect/PoisonEffect.miracle.hs",caract,0),m_useMiracle=true;
                        else
                            m_miracle.Concatenencer("Data/Items/PoisonEffect/PoisonEffect.miracle.hs",caract,0);
                    }
                }
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
    //fichier.open(chemin.c_str(), ios::in);
    if (fichier)
    {
        std::string buf;
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
                        *fichier>>buf;
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

        /*do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>m_nom;
                for (int i=0;i<(int)m_nom.size();i++)
                    if (m_nom[i]=='_')
                        m_nom[i]=' ';
                //getline(fichier, m_nom);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');

        std::string description;
        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                *fichier>>description;
                for (int i=0;i<(int)description.size();i++)
                    if (description[i]=='_')
                        description[i]=' ';
                //getline(fichier, description);
            }
            if (fichier->eof())
            {
                console->Ajouter("Erreur : Objet \" "+m_chemin+" \" Invalide",1);
                caractere='$';
            }

        }
        while (caractere!='$');*/

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
                        std::string temp;
                        *fichier>>temp;
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

        this->ChargerCaracteristiques(fichier);


        do
        {
            fichier->get(caractere);
            if (caractere=='*')
            {
                string temp;
                *fichier>>temp;
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

                    if (m_benedictions.back().type==DEGATS_SUPP)
                    {
                        if(ia == FEU)
                        {
                            m_chemin_miracles.push_back("Data/Items/FireEffect/FireEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/FireEffect/FireEffect.miracle.hs",caract,0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/FireEffect/FireEffect.miracle.hs",caract,0);
                        }
                        if(ia == FOI)
                        {
                            m_chemin_miracles.push_back("Data/Items/HolyEffect/HolyEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/HolyEffect/HolyEffect.miracle.hs",caract,0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/HolyEffect/HolyEffect.miracle.hs",caract,0);
                        }
                        if(ia == CORROSION)
                        {
                            m_chemin_miracles.push_back("Data/Items/PoisonEffect/PoisonEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/PoisonEffect/PoisonEffect.miracle.hs",caract,0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/PoisonEffect/PoisonEffect.miracle.hs",caract,0);
                        }
                    }

                   /* if (m_benedictions.back().type==DEGATS_FEU)
                    {
                        m_chemin_miracles.push_back("Data/Items/FireEffect/FireEffect.miracle.hs");
                        if (!m_useMiracle)
                            m_miracle.Charger("Data/Items/FireEffect/FireEffect.miracle.hs", caract, 0),m_useMiracle=true;
                        else
                            m_miracle.Concatenencer("Data/Items/FireEffect/FireEffect.miracle.hs", caract, 0);
                    }
                    if (m_benedictions.back().type==DEGATS_FOI)
                    {
                        m_chemin_miracles.push_back("Data/Items/HolyEffect/HolyEffect.miracle.hs");
                        if (!m_useMiracle)
                            m_miracle.Charger("Data/Items/HolyEffect/HolyEffect.miracle.hs", caract, 0),m_useMiracle=true;
                        else
                            m_miracle.Concatenencer("Data/Items/HolyEffect/HolyEffect.miracle.hs", caract, 0);
                    }*/
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

void Objet::ChargerMiracle(const Caracteristique &caract)
{
    m_miracle = Miracle();

    for(unsigned i = 0 ; i < m_chemin_miracles.size() ; ++i)
    {
        if(i > 0)
            m_miracle.Concatenencer(m_chemin_miracles[i], caract, 0);
        else
            m_miracle.Charger(m_chemin_miracles[i], caract, 0);
    }
}

void Objet::Generer(int bonus)
{
    m_armure=(rand() % (aa - ai + 1)) + ai;
    m_degatsMin=(rand() % (dia - dii + 1)) + dii;
    m_degatsMax=(rand() % (daa - dai + 1)) + dai;
    m_color.r=255;
    m_color.g=255;
    m_color.b=255;
    m_color.a=255;

    if (m_type != CONSOMMABLE)
        if (m_rarete<DIVIN)
        {
            int nbrBene=0;
            int rarete=NORMAL;

            if(bonus != 0)
            {
                int random=rand()%10000 / bonus;
                if (random<=900)
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

            m_rarete=rarete;

            if (m_rarete==BONNEFACTURE)
                nbrBene=1;

            if (m_rarete==BENI)
                nbrBene=rand()%(4-2)+2;

            if (m_rarete==SACRE)
                nbrBene=rand()%(9-5)+5;

            if (m_rarete==SANCTIFIE)
                nbrBene=rand()%(15-10)+10;

            int accru = 0;

            for (int i=0;i<nbrBene;i++)
            {
                bool ajouter=true;

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
                else if (temp.type==EFFICACITE_ACCRUE&&(m_type==ARME||m_type==ARMURE))
                {
                    temp.info1=(int)(rand()%(m_capaciteBenediction*10 - (int)((float)m_capaciteBenediction*2.5) + 1)+(float)m_capaciteBenediction*2.5);
                    //m_prix += (int)((float)prixor*0.5*(temp.info1)/(m_capaciteBenediction*10 - m_capaciteBenediction*2.5));
                }
                else if (temp.type==DEGATS_SUPP&&m_type==ARME)
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
                else if (temp.type==ARMURE_SUPP&&m_type==ARMURE)
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

                if ((temp.type==EFFICACITE_ACCRUE&&!(m_type==ARME||m_type==ARMURE))||
                    (temp.type==DEGATS_SUPP&&m_type!=ARME)||
                    (temp.type==ARMURE_SUPP&&m_type!=ARMURE))
                    ajouter=false,i--;

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
                    Caracteristique temp2;
                    m_benedictions.push_back(benediction ());
                    m_benedictions.back()=temp;

                    if (temp.type==DEGATS_SUPP)
                    {
                        if(temp.info1 == FEU)
                        {
                            m_chemin_miracles.push_back("Data/Items/FireEffect/FireEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/FireEffect/FireEffect.miracle.hs", temp2, 0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/FireEffect/FireEffect.miracle.hs", temp2, 0);
                        }
                        if(temp.info1 == FOI)
                        {
                            m_chemin_miracles.push_back("Data/Items/HolyEffect/HolyEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/HolyEffect/HolyEffect.miracle.hs", temp2, 0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/HolyEffect/HolyEffect.miracle.hs", temp2, 0);
                        }
                        if(temp.info1 == CORROSION)
                        {
                            m_chemin_miracles.push_back("Data/Items/PoisonEffect/PoisonEffect.miracle.hs");
                            if (!m_useMiracle)
                                m_miracle.Charger("Data/Items/PoisonEffect/PoisonEffect.miracle.hs", temp2, 0),m_useMiracle=true;
                            else
                                m_miracle.Concatenencer("Data/Items/PoisonEffect/PoisonEffect.miracle.hs", temp2, 0);
                        }
                    }

                    if (temp.type==ARMURE_SUPP || temp.type==DEGATS_SUPP)
                    {
                        if(temp.info1 == FEU)
                        {
                            m_color.r=255;
                            m_color.g=64;
                            m_color.b=64;
                        }
                        if(temp.info1 == FOI)
                        {
                            m_color.r=64;
                            m_color.g=64;
                            m_color.b=255;
                        }
                        if(temp.info1 == CORROSION)
                        {
                            m_color.r=64;
                            m_color.g=255;
                            m_color.b=64;
                        }
                    }
                }
            }

            m_prix = 0;

            if(aa - ai != 0)
                m_prix += ((m_armure + accru * m_armure / 100) * 2/*- aa + ai)/(aa - ai)/2*/);
            if(dia - dii != 0)
                m_prix += ((m_degatsMin + accru * m_degatsMin / 100) * 10 /*- dia + dii)/(dia - dii)/4*/);
            if(daa - dai != 0)
                m_prix += ((m_degatsMax + accru * m_degatsMax / 100) * 10 /*- daa + dai)/(daa - dai)/4*/);

            if(m_prix == 0)
                m_prix = 200;

            for (int j=0;j<(int)m_benedictions.size();j++)
            {
                if(m_benedictions[j].type == DEGATS_SUPP)
                    m_prix += (int)((float)50*(m_benedictions[j].info2+m_benedictions[j].info3)/*/(m_capaciteBenediction*2 - m_capaciteBenediction*0.5)*/);
                if(m_benedictions[j].type == ARMURE_SUPP)
                    m_prix += (int)((float)10*(m_benedictions[j].info2)/*/(m_capaciteBenediction*2 - m_capaciteBenediction*0.5)*/);
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

void Objet::ChargerCaracteristiques(std::ifstream *fichier)
{
    if (m_type==ARME)
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

    if (m_type==ARMURE)
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
}


sf::Text Objet::AjouterCaracteristiqueAfficher(coordonnee position,coordonnee *decalage,coordonnee *tailleCadran, const char *chaine,sf::Color color)
{
    sf::Text string;
    return string;
}


int Objet::AfficherCaracteristiques(coordonnee position,Caracteristique caract,float modPrix,bool compare,bool decalageDroite, bool surbrillance)
{

}

bool Objet::Utilisable(const Caracteristique &caract,int IDClasse)
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

