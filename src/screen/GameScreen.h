#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

#include "screen/Screen.h"
#include "element/Snake.h"
#include "element/Fruit.h"
#include "element/Grid.h"
#include "element/Button.h"

namespace sfSnake
{
    class GameScreen : public Screen
    {
    public:
        GameScreen();

        void handleInput(sf::RenderWindow &window) override;
        void update(sf::Time delta) override;
        void render(sf::RenderWindow &window) override;

        void generateFruit();
        void generateFruit_();
    private:
        Snake snake_;
        std::deque<Fruit> fruit_;
        Grid grid_;

        sf::Clock fruitUpdateClock_; // 添加一个时钟成员变量
        sf::Time fruitUpdateInterval_ = sf::seconds(15); // 添加一个时间间隔成员变量

        Button pauseButton_;


        sf::Text survivalTimeText_; // 用于显示生存时间
        sf::Clock survivalClock_; // 计时器，用于计算生存时间

        sf::Text score_;
        sf::Text highestScoreText_;
    };
}