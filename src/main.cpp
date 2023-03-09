#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"
#include "view.hpp"


int main(){
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");

    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> map = {};
    struct Snake *snake = createSnake(0, 0, DOWN);
    map[0][0] = SNAKE;
    newFood(map);

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
                    pauseGame(window);

                if (event.key.code == sf::Keyboard::Up)
                    if (getDirection(snake) != DOWN)
                        setDirection(snake, UP);
                if (event.key.code == sf::Keyboard::Down)
                    if (getDirection(snake) != UP)
                        setDirection(snake, DOWN);
                if (event.key.code == sf::Keyboard::Left)
                    if (getDirection(snake) != RIGHT)
                        setDirection(snake, LEFT);
                if (event.key.code == sf::Keyboard::Right)
                    if (getDirection(snake) != LEFT)
                        setDirection(snake, RIGHT);
            }
        }

        sf::sleep(sf::milliseconds(100));
        
        if (play(snake, map) == GAME_OVER){
            std::cout << "Game Over!" << std::endl;
            break;
        }

        window.clear();
        updateView(window, map);
        window.display();
    }

    destroySnake(snake);
    return 0;
}