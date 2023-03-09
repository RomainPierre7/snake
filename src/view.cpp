#include "view.hpp"

void updateView(sf::RenderWindow &window, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map){
    int caseWidth = window.getSize().x / GRID_WIDTH;
    sf::RectangleShape rectangle(sf::Vector2f(caseWidth, caseWidth));
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    for (int i = 0; i < GRID_WIDTH; i++){
        for (int j = 0; j < GRID_HEIGHT; j++){
            rectangle.setPosition(i * caseWidth, j * caseWidth);
            if (map[i][j] == EMPTY)
                rectangle.setFillColor(sf::Color::White);
            else if (map[i][j] == SNAKE)
                rectangle.setFillColor(sf::Color::Green);
            else if (map[i][j] == FOOD)
                rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
        }
    }
}

void pauseGame(sf::RenderWindow &window){
    sf::Font font;
    if (!font.loadFromFile("src/arial.ttf"))
        return;
    sf::Text text;
    text.setFont(font);
    text.setString("Paused, press Enter to continue");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y / 2 - text.getGlobalBounds().height / 2);
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Enter){
                    for (int i = 3; i > 0; i--){
                        window.clear();
                        text.setString("Resume in " + std::to_string(i) + " seconds");
                        window.draw(text);
                        window.display();
                        sf::sleep(sf::milliseconds(1000));
                    }
                    return;
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
}