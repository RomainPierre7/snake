#include "snake.hpp"
#include "geometry.hpp"

struct Snake{
    enum Direction direction;
    node *head;
};

struct Snake *createSnake(int x, int y, enum Direction direction){
    struct Snake *snake = new Snake;
    snake->direction = direction;
    snake->head = new node;
    snake->head->x = x;
    snake->head->y = y;
    snake->head->next = nullptr;
    return snake;
}

void destroySnake(Snake *snake){
    node *tmp = snake->head;
    while (tmp != nullptr){
        node *next = tmp->next;
        delete tmp;
        tmp = next;
    }
    delete snake;
}

enum Direction getDirection(Snake *snake){
    return snake->direction;
}

void setDirection(Snake *snake, enum Direction direction){
    snake->direction = direction;
}

node *getHead(Snake *snake){
    return snake->head;
}

void setHead(Snake *snake, node *head){
    snake->head = head;
}

void addHead(Snake *snake, node *head){
    head->next = snake->head;
    snake->head = head;
}

node *getTail(Snake *snake){
    node *tail = snake->head;
    while (tail->next != nullptr)
        tail = tail->next;
    return tail;
}

void removeTail(Snake *snake){
    node *tail = snake->head;
    while (tail->next->next != nullptr)
        tail = tail->next;
    tail->next = nullptr;
}