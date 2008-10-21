#ifndef MONSTREH
#define MONSTREH

#include "Personnage.h"
#include "Constantes.h"

class Modele_Monstre : public Modele_Personnage
{
    public:
    bool Charger(std::string chemin);
    std::vector<ModeleObjet> getObjets();

    std::string m_chemin;


    private:
    std::vector<ModeleObjet> m_objets;
};


class Monstre : public Personnage
{
    public:
    Monstre();
    void Charger(int numero,Modele_Monstre *modele);
    void testerVision(coordonnee positionHero);
    int getModele();
    coordonnee getDepart();
    void setDepart();

    bool getVu();

    private:
    bool m_vu;
    int m_modele;
    coordonnee m_depart;
};


#endif

