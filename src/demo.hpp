#ifndef __DEMO_HPP__
#define __DEMO_HPP__

#include <SFML/Graphics.hpp>
#include "geometry.hpp"

void updateViewDemo(sf::RenderWindow &window, std::array<std::array<int, 50>, 50>& map);

void newFoodDemo(std::array<std::array<int, 50>, 50>& map);

bool gameOverDemo(struct Snake *snake, std::array<std::array<int, 50>, 50>& map);

int playDemo(struct Snake *snake, std::array<std::array<int, 50>, 50>& map, int &score);

void demoGame(sf::RenderWindow &window, bool fast);

#endif