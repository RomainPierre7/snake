#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

enum Direction{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#define GRID_WIDTH 15
#define GRID_HEIGHT 15

#define EMPTY 0
#define SNAKE 1
#define FOOD 2

#define QUIT 0
#define PLAY 1
#define AUTO 2
#define AUTO_FAST 3
#define DEMO 4
#define RESET 5

#define GAME_OVER 0
#define GAME_CONTINUE 1

extern int bestScore;
extern int previousScore;

#endif