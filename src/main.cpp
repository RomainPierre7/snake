#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <fstream>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"
#include "view.hpp"
#include "demo.hpp"

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
    int SIDE = desktop.height - 200;
    int caseWidth = SIDE / GRID_WIDTH;
    sf::RenderWindow window(sf::VideoMode(caseWidth * GRID_WIDTH, caseWidth * GRID_WIDTH), "Snake");
    printf("%d, %d\n", window.getSize().x, window.getSize().y);

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
        case DEMO:
            demoGame(window, true);
            break;
        case RESET:
            resetBestScore(window);
            break;
        case QUIT:
            break;
    }

    window.close();
    return 0;
}