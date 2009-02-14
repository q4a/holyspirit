
#include "Light.h"



// Wall_Entity est une variable qui permet de repr�senter dans le programme un mur
struct Wall_Entity
{
    Wall_Entity (int id)
    {
        m_ID=id;
    }

    int ID() { return m_ID; }

    private:

    int m_ID;
};

// Light_Entity est une variable qui permet de repr�senter dans le programme une lumi�re
struct Light_Entity
{
    Light_Entity (){m_Dynamic=false,m_ID=0;}
    Light_Entity (int id,bool d)
    {
        m_ID=id;
        m_Dynamic=d;
    }

    int ID() { return m_ID; }
    bool Dynamic() { return m_Dynamic; }

    private:

    int m_ID;
    bool m_Dynamic;
};



class Light_Manager
{
    public :
    // Constructeur et destructeur
    Light_Manager();
    ~Light_Manager();


    //Diff�rents moyen d'ajouter une lumi�re dynamique, soit on l'ajoute sans aucune valeur par d�faut, soit on lui donne une lumi�re par d�faut, soit on lui donne ses valeurs "� la main"
    Light_Entity Add_Dynamic_Light();
    Light_Entity Add_Dynamic_Light(Light);
    Light_Entity Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    //Diff�rents moyen d'ajouter une lumi�re dynamique, soit on lui donne une lumi�re par d�faut, soit on lui donne ses valeurs "� la main"
    //On ne peut ajouter une lumi�re static sans rien, �a ne servirait � rien car elle ne peut �tre modifi�e par la suite
    Light_Entity Add_Static_Light(Light);
    Light_Entity Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    // Ajouter un mur
    void AddWall(sf::Vector2f pt1,sf::Vector2f pt2);

    // D�sactiver une lumi�re ou supprimer un mur
    bool Delete_Light(Light_Entity);
    bool Delete_Wall(Wall_Entity);

    // Calculer toutes les lumi�res dynamiques
    void Generate();

    // Afficher toutes les lumi�res � l'�cran
    void Draw(sf::RenderWindow *App);

    // Diff�rentes m�thodes pour modifier les attributs d'une lumi�re, ou les r�cup�rer. Il faut � chaque fois envoyer une Light_Entity en param�tre pour
    // savoir de quelle lumi�re on parle/

    void SetPosition(Light_Entity, sf::Vector2f );
    void SetQuality(Light_Entity, int );
    void SetRadius(Light_Entity, int );
    void SetColor(Light_Entity, sf::Color );
    void SetIntensity(Light_Entity, int);

    float GetIntensity(Light_Entity);
    float GetRadius(Light_Entity);
    int GetQuality(Light_Entity);
    sf::Color GetColor(Light_Entity);
    sf::Vector2f GetPosition(Light_Entity);

    private :
    // Les tableaux de murs, lumi�res statiques et dynamiques
    std::vector <Wall> m_wall;
    std::vector <Light> m_StaticLight;
    std::vector <Light> m_DynamicLight;
};

