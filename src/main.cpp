#include <SFML/Graphics.hpp>

int main(){
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.width, desktop.height), "Snake");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear();

        window.display();
    }
    return 0;
}