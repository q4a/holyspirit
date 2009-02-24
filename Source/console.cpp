#include "console.h"
#include "constantes.h"
#include "Globale.h"
#include <iostream>
#include <fstream>

using namespace std;

Console::~Console()
{
    m_textes.clear();
    m_erreur.clear();
}

void Console::Afficher(sf::RenderWindow* ecran)
{
    if(!configuration->desactivate_console)
    {
        int min=m_textes.size()-27+m_defilement;
        if(min<0)
        min=0;
        for(int i=min;i<(int)m_textes.size()+m_defilement;i++)
        {
            if(i>=0&&i<(int)m_erreur.size())
            {
                sf::String texte;
                texte.SetText(m_textes[i].c_str());
                texte.SetSize(12.f);
                texte.SetY((i-m_textes.size()+27-m_defilement)*15+60);

                if(m_erreur[i])
                    texte.SetColor(sf::Color(255,0,0,255));

                moteurGraphique->AjouterTexte(&texte,19);
            }

        }
    }
}
void Console::Ajouter(std::string texte,bool erreur)
{
    if(!configuration->desactivate_console)
    {
        m_textes.push_back(texte);
        m_erreur.push_back(erreur);
        Rapport();
    }
}

void Console::Ajouter(int nombre,bool erreur)
{
    if(!configuration->desactivate_console)
    {
        char temp[10];
        sprintf(temp,"%i",nombre);
        m_textes.push_back(temp);
        m_erreur.push_back(erreur);
        Rapport();
    }
}

int Console::getTaille(){return m_textes.size();}

void Console::defiler(bool direction)
{
    if(direction==0)
    {
        m_defilement--;
        if(m_defilement<(int)-m_textes.size())
            m_defilement=-m_textes.size()+1;
    }
    if(direction==1)
    {
        m_defilement++;
        if(m_defilement>0)
            m_defilement=0;
    }
}

void Console::Rapport()
{
    ofstream fichier("Log.txt", ios::out | ios::trunc);
    if(fichier)  // si l'ouverture a réussi
    {
        for(int i=0;i<(int)m_textes.size();i++)
        {
            if(m_erreur[i])
                fichier<<"ERREUR : ";
            else
                fichier<<"         ";

            fichier<<m_textes[i]<<'\n';
        }
        fichier.close();
    }
}
