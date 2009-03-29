#include <SFML/System.hpp>
#include <iostream>

void ThreadFunction(void* UserData)
{
    // Afficher quelque chose...
    for (int i = 0; i < 10; ++i)
        std::cout << "Je suis le thread numero 1" << std::endl;
}

int main()
{
    // Création d'un thread avec notre fonction
    sf::Thread Thread(&ThreadFunction);

    // Lancement du thread !
    Thread.Launch();

    // Afficher quelque chose...
    for (int i = 0; i < 10; ++i)
        std::cout << "Je suis le thread principal" << std::endl;

    return EXIT_SUCCESS;
}
