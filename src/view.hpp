#ifndef __VIEW_HPP__
#define __VIEW_HPP__

#include <SFML/Graphics.hpp>
#include <array>
#include "geometry.hpp"
#include "snake.hpp"

void updateView(sf::RenderWindow &window, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map);

void pauseGame(sf::RenderWindow &window);

int menu(sf::RenderWindow &window);

#endif