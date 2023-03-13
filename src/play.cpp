#include <iostream>
#include <fstream>
#include <array>
#include "geometry.hpp"
#include "snake.hpp"
#include "play.hpp"
#include "view.hpp"
#include "demo.hpp"

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
        
        if ((play(snake, map, score) == GAME_OVER) || (score >= GRID_HEIGHT * GRID_WIDTH - 1)){
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
        case DEMO:
            demoGame(window, true);
            break;
        case RESET:
            resetBestScore();
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

        std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT> distance = {};
        for (int i = 0; i < GRID_WIDTH; i++){
            for (int j = 0; j < GRID_HEIGHT; j++){
                distance[i][j] = -1;
            }
        }
        distance[x_food][y_food] = 0;

        int index = 0;
        while ((distance[x_head][y_head] == -1) && (index < GRID_WIDTH * GRID_HEIGHT)){
            for (int i = 0; i < GRID_WIDTH; i++){
                for (int j = 0; j < GRID_HEIGHT; j++){
                    if (distance[i][j] == index){
                        if (i + 1 < GRID_WIDTH && distance[i + 1][j] == -1 && ((map[i + 1][j] != SNAKE) || (i + 1 == x_head && j == y_head)))
                            distance[i + 1][j] = index + 1;
                        if (i - 1 >= 0 && distance[i - 1][j] == -1 && ((map[i - 1][j] != SNAKE) || (i - 1 == x_head && j == y_head)))
                            distance[i - 1][j] = index + 1;
                        if (j + 1 < GRID_HEIGHT && distance[i][j + 1] == -1 && ((map[i][j + 1] != SNAKE) || (i == x_head && j + 1 == y_head)))
                            distance[i][j + 1] = index + 1;
                        if (j - 1 >= 0 && distance[i][j - 1] == -1 && ((map[i][j - 1] != SNAKE) || (i == x_head && j - 1 == y_head)))
                            distance[i][j - 1] = index + 1;
                    }
                    
                }
            }
            index++;
        }

        if (index < GRID_WIDTH * GRID_HEIGHT){
            if (x_head + 1 <= GRID_WIDTH - 1 && distance[x_head + 1][y_head] == distance[x_head][y_head] - 1 && getDirection(snake) != LEFT && getDirection(snake) != RIGHT && distance[x_head + 1][y_head] != -1)
                    setDirection(snake, RIGHT);
            else if (x_head - 1 >= 0 && distance[x_head - 1][y_head] == distance[x_head][y_head] - 1 && getDirection(snake) != LEFT && getDirection(snake) != RIGHT && distance[x_head - 1][y_head] != -1)
                    setDirection(snake, LEFT);
            else if (y_head + 1 <= GRID_HEIGHT - 1 && distance[x_head][y_head + 1] == distance[x_head][y_head] - 1 && getDirection(snake) != UP && getDirection(snake) != DOWN && distance[x_head][y_head + 1] != -1)
                    setDirection(snake, DOWN);
            else if (y_head - 1 >= 0 && distance[x_head][y_head - 1] == distance[x_head][y_head] - 1 && getDirection(snake) != UP && getDirection(snake) != DOWN && distance[x_head][y_head - 1] != -1)
                    setDirection(snake, UP);
        } else {
            if (getDirection(snake) == LEFT || getDirection(snake) == RIGHT){
                if (y_head + 1 < GRID_HEIGHT && map[x_head][y_head + 1] != SNAKE)
                    setDirection(snake, DOWN);
                else if (y_head - 1 >= 0 && map[x_head][y_head - 1] != SNAKE)
                    setDirection(snake, UP);
            } else {
                if (x_head + 1 < GRID_WIDTH && map[x_head + 1][y_head] != SNAKE)
                    setDirection(snake, RIGHT);
                else if (x_head - 1 >= 0 && map[x_head - 1][y_head] != SNAKE)
                    setDirection(snake, LEFT);
            }
        }

        if (!fast) sf::sleep(sf::milliseconds(120));
        else sf::sleep(sf::milliseconds(10));
        
        if ((play(snake, map, score) == GAME_OVER) || (score >= GRID_HEIGHT * GRID_WIDTH - 1)){
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
        case DEMO:
            demoGame(window, true);
            break;
        case RESET:
            resetBestScore();
            break;
        case QUIT:
            break;
    }
}

void resetBestScore(){
    std::ofstream file("data/best_score.txt");
    file << 0;
    file.close();
}