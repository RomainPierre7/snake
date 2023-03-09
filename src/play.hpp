#ifndef __PLAY_HPP__
#define __PLAY_HPP__

#include <array>
#include "snake.hpp"

#define GAME_OVER 0
#define GAME_CONTINUE 1

void newFood(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map);

int play(struct Snake *snake, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map);

bool gameOver(struct Snake *snake, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map);

#endif