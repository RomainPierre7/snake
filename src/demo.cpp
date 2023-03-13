#include <iostream>
#include <fstream>
#include <array>
#include "geometry.hpp"
#include "snake.hpp"
#include "demo.hpp"
#include "view.hpp"
#include "demo.hpp"
#include "view.hpp"
#include "play.hpp"

void updateViewDemo(sf::RenderWindow &window, std::array<std::array<int, 50>, 50>& map){
    int caseWidth = window.getSize().x / 50;
    sf::RectangleShape rectangle(sf::Vector2f(caseWidth, caseWidth));
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    for (int i = 0; i < 50; i++){
        for (int j = 0; j < 50; j++){
            rectangle.setPosition(i * caseWidth, j * caseWidth);
            if (map[i][j] == EMPTY)
                rectangle.setFillColor(sf::Color::White);
            else if (map[i][j] == SNAKE)
                rectangle.setFillColor(sf::Color::Green);
            else if (map[i][j] == FOOD)
                rectangle.setFillColor(sf::Color::Red);
            window.draw(rectangle);
        }
    }
}

void newFoodDemo(std::array<std::array<int, 50>, 50>& map){
    int x = rand() % 50;
    int y = rand() % 50;
    while (map[x][y] != EMPTY){
        x = rand() % 50;
        y = rand() % 50;
    }
    map[x][y] = FOOD;
}

bool gameOverDemo(struct Snake *snake, std::array<std::array<int, 50>, 50>& map){
    node *head = getHead(snake);
    if (head->x < 0 || head->x >= 50 || head->y < 0 || head->y >= 50)
        return true;
    if (map[head->x][head->y] == SNAKE)
        return true;
    return false;
}

int playDemo(struct Snake *snake, std::array<std::array<int, 50>, 50>& map, int &score){
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
    if (gameOverDemo(snake, map)) return GAME_OVER;
    if (map[newHead->x][newHead->y] != FOOD){
        node *tail = getTail(snake);
        map[tail->x][tail->y] = EMPTY;
        removeTail(snake);
    } else {
        newFoodDemo(map);
        score++;
    }
    map[newHead->x][newHead->y] = SNAKE;
    return GAME_CONTINUE;
}

void demoGame(sf::RenderWindow &window, bool fast){
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int SIDE = desktop.height - 200;
    int caseWidth = SIDE / 50;
    window.setSize(sf::Vector2u(caseWidth * 50 + 40, caseWidth * 50 + 40));
    printf("%d, %d\n", window.getSize().x, window.getSize().y);

    std::array<std::array<int, 50>, 50> map = {};
    struct Snake *snake = createSnake(0, 0, DOWN);
    map[0][0] = SNAKE;
    newFoodDemo(map);

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
        for (int i = 0; i < 50; i++){
            for (int j = 0; j < 50; j++){
                if (map[i][j] == FOOD){
                    x_food = i;
                    y_food = j;
                }
            }
        }

        node *head = getHead(snake);
        int x_head = head->x;
        int y_head = head->y;

        std::array<std::array<int, 50>, 50> distance = {};
        for (int i = 0; i < 50; i++){
            for (int j = 0; j < 50; j++){
                distance[i][j] = -1;
            }
        }
        distance[x_food][y_food] = 0;

        int index = 0;
        while ((distance[x_head][y_head] == -1) && (index < 50 * 50)){
            for (int i = 0; i < 50; i++){
                for (int j = 0; j < 50; j++){
                    if (distance[i][j] == index){
                        if (i + 1 < 50 && distance[i + 1][j] == -1 && ((map[i + 1][j] != SNAKE) || (i + 1 == x_head && j == y_head)))
                            distance[i + 1][j] = index + 1;
                        if (i - 1 >= 0 && distance[i - 1][j] == -1 && ((map[i - 1][j] != SNAKE) || (i - 1 == x_head && j == y_head)))
                            distance[i - 1][j] = index + 1;
                        if (j + 1 < 50 && distance[i][j + 1] == -1 && ((map[i][j + 1] != SNAKE) || (i == x_head && j + 1 == y_head)))
                            distance[i][j + 1] = index + 1;
                        if (j - 1 >= 0 && distance[i][j - 1] == -1 && ((map[i][j - 1] != SNAKE) || (i == x_head && j - 1 == y_head)))
                            distance[i][j - 1] = index + 1;
                    }
                    
                }
            }
            index++;
        }

        if (index < 50 * 50){
            if (x_head + 1 <= 50 - 1 && distance[x_head + 1][y_head] == distance[x_head][y_head] - 1 && getDirection(snake) != LEFT && getDirection(snake) != RIGHT && distance[x_head + 1][y_head] != -1)
                    setDirection(snake, RIGHT);
            else if (x_head - 1 >= 0 && distance[x_head - 1][y_head] == distance[x_head][y_head] - 1 && getDirection(snake) != LEFT && getDirection(snake) != RIGHT && distance[x_head - 1][y_head] != -1)
                    setDirection(snake, LEFT);
            else if (y_head + 1 <= 50 - 1 && distance[x_head][y_head + 1] == distance[x_head][y_head] - 1 && getDirection(snake) != UP && getDirection(snake) != DOWN && distance[x_head][y_head + 1] != -1)
                    setDirection(snake, DOWN);
            else if (y_head - 1 >= 0 && distance[x_head][y_head - 1] == distance[x_head][y_head] - 1 && getDirection(snake) != UP && getDirection(snake) != DOWN && distance[x_head][y_head - 1] != -1)
                    setDirection(snake, UP);
        } else {
            if (getDirection(snake) == LEFT || getDirection(snake) == RIGHT){
                if (y_head + 1 < 50 && map[x_head][y_head + 1] != SNAKE)
                    setDirection(snake, DOWN);
                else if (y_head - 1 >= 0 && map[x_head][y_head - 1] != SNAKE)
                    setDirection(snake, UP);
            } else {
                if (x_head + 1 < 50 && map[x_head + 1][y_head] != SNAKE)
                    setDirection(snake, RIGHT);
                else if (x_head - 1 >= 0 && map[x_head - 1][y_head] != SNAKE)
                    setDirection(snake, LEFT);
            }
        }

        if (!fast) sf::sleep(sf::milliseconds(120));
        else sf::sleep(sf::milliseconds(10));
        
        if ((playDemo(snake, map, score) == GAME_OVER) || (score >= 50 * 50 - 1)){
            break;
        }
        window.setTitle("Snake - Score: " + std::to_string(score));

        window.clear();
        updateViewDemo(window, map);
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
        case QUIT:
            break;
    }
}