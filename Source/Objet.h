
#include<iostream>

class Objet
{
    public:
    Objet();
    Objet(std::string nom,int rarete);

    std::string getNom();
    int getRarete();

    private:
    std::string m_nom;
    int m_rarete;
};

