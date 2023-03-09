#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include "geometry.hpp"

struct node{
    int x;
    int y;
    node *next;
};

struct Snake;


struct Snake *createSnake(int x, int y, enum Direction direction);

void destroySnake(Snake *snake);

enum Direction getDirection(Snake *snake);

void setDirection(Snake *snake, enum Direction direction);

node *getHead(Snake *snake);

void setHead(Snake *snake, node *head);

void addHead(Snake *snake, node *head);

node *getTail(Snake *snake);

void removeTail(Snake *snake);

#endif