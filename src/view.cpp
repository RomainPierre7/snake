#include "view.hpp"
#include "play.hpp"
#include "demo.hpp"

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
    if (!font.loadFromFile("data/arial.ttf"))
        return;
    sf::Text text;
    text.setFont(font);
    text.setString("Paused, press Enter to resume\n\nPress Esc to quit to menu");
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
                if (event.key.code == sf::Keyboard::Escape){
                    switch(menu(window)){
                        case PLAY:
                            playGame(window);
                            break;
                        case AUTO:
                            autoGame(window, false);
                            break;
                        case AUTO_FAST:
                            autoGame(window, true);
                            break;
                        case DEMO:
                            demoGame(window, true);
                            break;
                        case RESET:
                            resetBestScore(window);
                            break;
                        case QUIT:
                            break;
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

int menu(sf::RenderWindow &window){
    sf::Font font;
    if (!font.loadFromFile("data/arial.ttf"))
        return -1;
    sf::Text text;
    text.setFont(font);
    text.setString("Snake\n\n\npress 'Enter' to play\n\npress 'A' to auto play\npress 'F' to auto play in fast mode\npress 'D' to launch demo mode (50x50, fast bot)\n\npress 'Escape' to quit\npress 'R' to reset best score\n\n\nBest score: " + std::to_string(bestScore) + "\nPrevious score: " + std::to_string(previousScore));
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width / 2, window.getSize().y / 2 - text.getGlobalBounds().height / 2);
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                return QUIT;

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    window.close();
                    return QUIT;
                }
                if (event.key.code == sf::Keyboard::Enter){
                    return PLAY;
                }
                if (event.key.code == sf::Keyboard::A){
                    return AUTO;
                }
                if (event.key.code == sf::Keyboard::F){
                    return AUTO_FAST;
                }
                if (event.key.code == sf::Keyboard::D){
                    return DEMO;
                }
                if (event.key.code == sf::Keyboard::R){
                    return RESET;
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }
    return QUIT;
}