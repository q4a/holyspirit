
#include "Personnage.h"
#include "Constantes.h"

class Modele_Monstre : public Modele_Personnage
{
    public:
    bool Charger(std::string chemin);
};


class Monstre : public Personnage
{
    public:
    Monstre();
    void Charger(int numero,Modele_Monstre modele);
    int getModele();

    private:
    bool m_vu;
    int m_modele;
};



