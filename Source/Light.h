#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

struct Wall
{
    Wall (sf::Vector2f p1,sf::Vector2f p2)
    {
        pt1=p1;
        pt2=p2;
    }

    // Pt1 et Pt2 sont les deux extrémités du mur
    sf::Vector2f pt1;
    sf::Vector2f pt2;

    // collision permet de savoir si on a déjà compter la collision avec une extrémité du mur
    bool collision;
};

class Light
{
    public :

    // Constructeur et destructeur
    Light();
    Light(sf::Vector2f position, float intensity, float radius, int quality, sf::Color color);
    ~Light();

    // Afficher la lumière
    void Draw(sf::RenderWindow *App);

    // Calculer la lumière
    void Generate(std::vector <Wall> m_wall);

    // Ajouter un triangle à la lumière, en effet, les lumières sont composée de triangles
    void AddTriangle(sf::Vector2f pt1,sf::Vector2f pt2, int minimum_wall,std::vector <Wall> m_wall);

    // Tester voir si un point "pt" se trouve dans le triangle [(0;0),"pt1","pt2"]
    bool CollisionWithPoint(sf::Vector2f pt,sf::Vector2f pt1,sf::Vector2f pt2);

    // Tester voir si une ligne ["l1","l1"] traverse le triangle [(0;0),"pt1","pt2"]
    bool CollisionWithLine(sf::Vector2f l1, sf::Vector2f l2,sf::Vector2f pt1,sf::Vector2f pt2);

    // Changer différents attributs de la lumière
    void SetIntensity(float);
    void SetRadius(float);
    void SetQuality(int);
    void SetColor(sf::Color);
    void SetPosition(sf::Vector2f);


    // Retourner différents attributs de la lumière
    float GetIntensity();
    float GetRadius();
    int GetQuality();
    sf::Color GetColor();
    sf::Vector2f GetPosition();

    // Une petite bool pour savoir si la lumière est allumée ou éteinte
    bool m_actif;

    private :
    //Position à l'écran
    sf::Vector2f m_position;
    //Intensité, gère la transparence ( entre 0 et 255 )
    float m_intensity;
    //Rayon de la lumière
    float m_radius;
    //Couleur de la lumière
    sf::Color m_color;
    //Qualité de la lumière, c'est à dire le nombre de triangles par défaut qui la compose.
    int m_quality;

    //Tableau dynamique de Shape, ce sont ces shapes de type triangle qui compose la lumière
    std::vector <sf::Shape> m_shape;
};


