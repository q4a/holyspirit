
#include "Objet.h"

Objet::Objet()
{
    m_nom="Un objet merveilleux";
    m_rarete=0;
}

Objet::Objet(std::string nom, int rarete)
{
    m_nom=nom;
    m_rarete=rarete;
}

std::string Objet::getNom(){return m_nom;}
int Objet::getRarete(){return m_rarete;}
