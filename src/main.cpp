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
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");

    std::ifstream file("data/best_score.txt");
    file >> bestScore;
    file.close();
    
    switch(menu(window)){
        case PLAY:
            playGame(window);
            break;
        case QUIT:
            break;
    }

    window.close();
    return 0;
}