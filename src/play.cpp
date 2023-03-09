#include <iostream>
#include <array>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"

void newFood(std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map){
    int x = rand() % GRID_WIDTH;
    int y = rand() % GRID_HEIGHT;
    while (map[x][y] != EMPTY){
        x = rand() % GRID_WIDTH;
        y = rand() % GRID_HEIGHT;
    }
    map[x][y] = FOOD;
}

bool gameOver(struct Snake *snake, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map){
    node *head = getHead(snake);
    if (head->x < 0 || head->x >= GRID_WIDTH || head->y < 0 || head->y >= GRID_HEIGHT)
        return true;
    if (map[head->x][head->y] == SNAKE)
        return true;
    return false;
}

int play(struct Snake *snake, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map){
    node *head = getHead(snake);
    node *newHead = new node;
    newHead->x = head->x;
    newHead->y = head->y;
    switch (getDirection(snake)){
        case UP:
            newHead->y--;
            break;
        case DOWN:
            newHead->y++;
            break;
        case LEFT:
            newHead->x--;
            break;
        case RIGHT:
            newHead->x++;
            break;
    }
    addHead(snake, newHead);
    if (gameOver(snake, map)) return GAME_OVER;
    if (map[newHead->x][newHead->y] != FOOD){
        node *tail = getTail(snake);
        map[tail->x][tail->y] = EMPTY;
        removeTail(snake);
    } else {
        newFood(map);
    }
    map[newHead->x][newHead->y] = SNAKE;
    return GAME_CONTINUE;
}