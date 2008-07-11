///**************************************************\\\
||| Dernière modification : Le 06/02/08 par Gregouar |||
///**************************************************///




#ifndef EVENEMENTH
#define EVENEMENTH

#include "constantes.h"

class Evenement
{
	public:
	Evenement();
	Evenement(int numero);
	~Evenement();
	void setEvenement(int numeroEvenement);
	void AjouterInformation(int information); // Ajouter une information à l'événement ( ex : pour un changement de map, le numéro de la prochaine map, la position du héro, etc... )
	int getType();
	int getInformation(int numeroInfo);
	int getNombreInformation(){return m_information.size();}
	void deleteInformations();

	Evenement operator=(const Evenement &evenement);

	protected:
	int m_numero;
	std::vector <int> m_information;
};

#endif

