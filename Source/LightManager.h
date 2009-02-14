
#include "Light.h"



// Wall_Entity est une variable qui permet de représenter dans le programme un mur
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

// Light_Entity est une variable qui permet de représenter dans le programme une lumière
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


    //Différents moyen d'ajouter une lumière dynamique, soit on l'ajoute sans aucune valeur par défaut, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    Light_Entity Add_Dynamic_Light();
    Light_Entity Add_Dynamic_Light(Light);
    Light_Entity Add_Dynamic_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    //Différents moyen d'ajouter une lumière dynamique, soit on lui donne une lumière par défaut, soit on lui donne ses valeurs "à la main"
    //On ne peut ajouter une lumière static sans rien, ça ne servirait à rien car elle ne peut être modifiée par la suite
    Light_Entity Add_Static_Light(Light);
    Light_Entity Add_Static_Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);

    // Ajouter un mur
    void AddWall(sf::Vector2f pt1,sf::Vector2f pt2);

    // Désactiver une lumière ou supprimer un mur
    bool Delete_Light(Light_Entity);
    bool Delete_Wall(Wall_Entity);

    // Calculer toutes les lumières dynamiques
    void Generate();

    // Afficher toutes les lumières à l'écran
    void Draw(sf::RenderWindow *App);

    // Différentes méthodes pour modifier les attributs d'une lumière, ou les récupérer. Il faut à chaque fois envoyer une Light_Entity en paramètre pour
    // savoir de quelle lumière on parle/

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
    // Les tableaux de murs, lumières statiques et dynamiques
    std::vector <Wall> m_wall;
    std::vector <Light> m_StaticLight;
    std::vector <Light> m_DynamicLight;
};

