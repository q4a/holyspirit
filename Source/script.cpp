#include <fstream>
#include "script.h"

#include "globale.h"

using namespace std;


Script::Script()
{
}


Script::Script(std::string chemin)
{
    Charger(chemin);
}

void Script::AjouterCondition(ifstream *fichier)
{
    int noCondition=0;

    Instruction instructionBuffer;
    instructionBuffer.nom="if";
    m_instructions.push_back(instructionBuffer);

    noCondition=m_instructions.size()-1;

    bool OK=true;

    while(!fichier->eof()&&OK)
    {
        int temp;
        temp=Lire(fichier);

        if(temp==-1)
            OK=false;
        else
            m_instructions.at(noCondition).valeurs.push_back(temp);
    }
}

int Script::Lire(ifstream *fichier)
{
    int retour=0;

    string temp;

    *fichier>>temp;
    if(temp=="if")
        retour=m_instructions.size(),AjouterCondition(fichier);
    else if(temp=="end")
        retour=-1;
    else if(temp=="then")
        retour=-2;
    else if(temp=="else")
        retour=-3;
    else
    {
        retour=m_instructions.size();

        Instruction instructionBuffer;
        instructionBuffer.nom=temp;

        m_instructions.push_back(instructionBuffer);

        char caractere;
        fichier->get(caractere);
        if(caractere=='*')
        {
            int valeur;
            *fichier>>valeur;
            m_instructions[m_instructions.size()-1].valeurs.push_back(valeur);
        }

        fichier->get(caractere);
        if(caractere=='*')
        {
            int valeur;
            *fichier>>valeur;
            m_instructions[m_instructions.size()-1].valeurs.push_back(valeur);
        }
    }

    return retour;
}

void Script::Charger(std::string chemin)
{
    ifstream fichier;
    fichier.open(chemin.c_str(), ios::in);

    if(fichier)
    {
        string temp;
        while(!fichier.eof()&&temp!="main")
        {
            fichier>>temp;
            Instruction instructionBuffer;
            instructionBuffer.nom="main";
            m_instructions.push_back(instructionBuffer);
        }

        bool OK=true;
        while(!fichier.eof()&&OK==true)
        {
            int temp=Lire(&fichier);

            if(temp==-1)
                OK=false;
            else
                m_instructions[0].valeurs.push_back(temp);

        }
    }

    fichier.close();

    char chaine[255];

    sprintf(chaine,"Chargement du script : \"%s\"",chemin.c_str());
    console.Ajouter(chaine);
}
