#include <iostream>
#include <fstream>
#include <array>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"
#include "view.hpp"

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

int play(struct Snake *snake, std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>& map, int &score){
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
        score++;
    }
    map[newHead->x][newHead->y] = SNAKE;
    return GAME_CONTINUE;
}

void playGame(sf::RenderWindow &window){
    std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> map = {};
    struct Snake *snake = createSnake(0, 0, DOWN);
    map[0][0] = SNAKE;
    newFood(map);

    int score = 0;

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

        sf::sleep(sf::milliseconds(120));
        
        if (play(snake, map, score) == GAME_OVER){
            break;
        }
        window.setTitle("Snake - Score: " + std::to_string(score));

        window.clear();
        updateView(window, map);
        window.display();
    }
    destroySnake(snake);
    
    if (score > bestScore){
        bestScore = score;
        std::ofstream file("data/best_score.txt");
        file << bestScore;
        file.close();
    }
    previousScore = score;

    switch(menu(window)){
        case PLAY:
            playGame(window);
            break;
        case QUIT:
            break;
    }
}