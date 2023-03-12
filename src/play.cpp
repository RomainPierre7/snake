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
        case AUTO:
            autoGame(window, false);
            break;
        case AUTO_FAST:
            autoGame(window, true);
            break;
        case QUIT:
            break;
    }
}

void autoGame(sf::RenderWindow &window, bool fast){
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
                if (event.key.code == sf::Keyboard::Escape)
                    pauseGame(window);
        }

        int x_food = 0;
        int y_food = 0;
        for (int i = 0; i < GRID_WIDTH; i++){
            for (int j = 0; j < GRID_HEIGHT; j++){
                if (map[i][j] == FOOD){
                    x_food = i;
                    y_food = j;
                }
            }
        }

        node *head = getHead(snake);
        int x_head = head->x;
        int y_head = head->y;

        if (x_head < x_food && getDirection(snake) != LEFT && getDirection(snake) != RIGHT){
            if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1)){
                setDirection(snake, RIGHT);
            } else if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1)){
                setDirection(snake, DOWN);
            } else if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0)){
                setDirection(snake, UP);
            } else if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0)){
                setDirection(snake, LEFT);
            }
        } else if (x_head > x_food && getDirection(snake) != LEFT && getDirection(snake) != RIGHT){
            if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0)){
                setDirection(snake, LEFT);
            } else if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1)){
                setDirection(snake, DOWN);
            } else if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0)){
                setDirection(snake, UP);
            } else if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1)){
                setDirection(snake, RIGHT);
            }
        } else if (y_head < y_food && getDirection(snake) != UP && getDirection(snake) != DOWN){
            if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1)){
                setDirection(snake, DOWN);
            } else if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1)){
                setDirection(snake, RIGHT);
            } else if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0)){
                setDirection(snake, LEFT);
            } else if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0)){
                setDirection(snake, UP);
            }
        } else if (y_head > y_food && getDirection(snake) != UP && getDirection(snake) != DOWN){
            if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0)){
                setDirection(snake, UP);
            } else if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1)){
                setDirection(snake, RIGHT);
            } else if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0)){
                setDirection(snake, LEFT);
            } else if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1)){
                setDirection(snake, DOWN);
            }
        } else {
            if (getDirection(snake) == UP){
                if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0))
                    setDirection(snake, UP);
                else if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1))
                    setDirection(snake, RIGHT);
                else if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0))
                    setDirection(snake, LEFT);
            } else if (getDirection(snake) == DOWN){
                if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1))
                    setDirection(snake, DOWN);
                else if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1))
                    setDirection(snake, RIGHT);
                else if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0))
                    setDirection(snake, LEFT);
            } else if (getDirection(snake) == LEFT){
                if ((map[x_head - 1][y_head] != SNAKE) && (x_head > 0))
                    setDirection(snake, LEFT);
                else if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0))
                    setDirection(snake, UP);
                else if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1))
                    setDirection(snake, DOWN);
            } else if (getDirection(snake) == RIGHT){
                if ((map[x_head + 1][y_head] != SNAKE) && (x_head < GRID_WIDTH - 1))
                    setDirection(snake, RIGHT);
                else if ((map[x_head][y_head - 1] != SNAKE) && (y_head > 0))
                    setDirection(snake, UP);
                else if ((map[x_head][y_head + 1] != SNAKE) && (y_head < GRID_HEIGHT - 1))
                    setDirection(snake, DOWN);
            }
        }

        if (!fast) sf::sleep(sf::milliseconds(120));
        else sf::sleep(sf::milliseconds(10));
        
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
        case AUTO:
            autoGame(window, false);
            break;
        case AUTO_FAST:
            autoGame(window, true);
            break;
        case QUIT:
            break;
    }
}