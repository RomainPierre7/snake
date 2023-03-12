#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <fstream>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"
#include "view.hpp"

int bestScore = 0;
int previousScore = 0;

int main(){
    if (!sf::Shader::isAvailable()) {
        std::cout << "Error: SFML library doesn't install on your system" << std::endl;
        std::cout << "Please download it on https://www.sfml-dev.org/ and install it" << std::endl;
        return 1;
    }

    srand(time(NULL));

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int HEIGHT = desktop.height - 200;
    sf::RenderWindow window(sf::VideoMode(HEIGHT, HEIGHT), "Snake");

    std::ifstream file("data/best_score.txt");
    file >> bestScore;
    file.close();
    
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
        case QUIT:
            break;
    }

    window.close();
    return 0;
}