
#ifndef _CONTEXTEH
#define _CONTEXTEH

class Jeu;

class Contexte
{
    public:
	virtual void Utiliser(Jeu *jeu) = 0;
};

#endif
