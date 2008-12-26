

#ifndef MODELMOTEURPARTICULES
#define MODELMOTEURPARTICULES

#include "constantes.h"

struct ModeleParticule
{
    coordonnee positionImage;

    int min,max;
};

class ModeleParticuleSysteme
{
    public:
    ModeleParticuleSysteme();
    ModeleParticuleSysteme(std::string chemin);
    ~ModeleParticuleSysteme();
    void Charger(std::string chemin);

    //private:
    int m_image,m_son;
    std::string m_chemin;
    std::vector <ModeleParticule> m_particules;
};
#endif
