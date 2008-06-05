#include "console.h"

void Console::Afficher(sf::RenderWindow* ecran)
{
    int min=m_textes.size()-27;
    if(min<0)
    min=0;
    for(int i=min;i<m_textes.size();i++)
    {
        sf::String texte;
        texte.SetText(m_textes[i].c_str());
        texte.SetSize(12.f);
        texte.SetTop((i-m_textes.size()+27)*15+40);

        if(m_erreur[i])
            texte.SetColor(sf::Color(255,0,0,255));

        ecran->Draw(texte);

    }
}
void Console::Ajouter(std::string texte,bool erreur)
{
    m_textes.push_back(texte);
    m_erreur.push_back(erreur);
}

void Console::AjouterNombre(int nombre,bool erreur)
{
    char temp[10];
    sprintf(temp,"%ld",nombre);
    m_textes.push_back(temp);
    m_erreur.push_back(erreur);
}


