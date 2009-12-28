#include <iostream>

using namespace std;

int main()
{
    sf::RenderWindow App(sf::VideoMode(512, 384, 32), "Bejeweled");

    Bejeweled my_beje;

    my_beje.Generate();

    while (App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        App.Clear();

        my_beje.Update();
        App.Draw(sf::sprite(my_beje.GetImage()));

        App.Display();
    }

    return EXIT_SUCCESS;
}
