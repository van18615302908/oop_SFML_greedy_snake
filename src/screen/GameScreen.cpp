#include <SFML/Graphics.hpp>

#include <random>
#include <memory>
#include <iostream>

#include "screen/GameScreen.h"
#include "screen/GameOverScreen.h"
#include "screen/PauseScreen.h"
#include "screen/HelpScreen.h"

using namespace sfSnake;

GameScreen::GameScreen()
    : snake_(), grid_(), pauseButton_()
{
    pauseButton_.update("assets/image/pauseUI.png", 1 / 16.0f);
    pauseButton_.setPosition(
        Game::GlobalVideoMode.width / 15.0 * 14.0,
        Game::GlobalVideoMode.width / 15.0);

    score_.setFont(Game::GlobalFont);
    score_.setString(sf::String(L"当前分数:") + std::to_string(snake_.getScore()));
    score_.setCharacterSize(Game::GlobalVideoMode.width / 25.0f);
    score_.setFillColor(Game::Color::Yellow);
    setOriginMiddle(score_);
    score_.setPosition(
        Game::GlobalVideoMode.width / 2.0f,
        Game::GlobalVideoMode.width * 0.05f);

    // 最高分文本
    highestScoreText_.setFont(Game::GlobalFont);
    highestScoreText_.setString(sf::String(L"最高分数:") + std::to_string(sfSnake::Snake::highestScore));
    highestScoreText_.setCharacterSize(Game::GlobalVideoMode.width / 25.0f);
    highestScoreText_.setFillColor(Game::Color::Yellow);
    setOriginMiddle(highestScoreText_);
    highestScoreText_.setPosition(
            Game::GlobalVideoMode.width / 2.0f,
            Game::GlobalVideoMode.width * 0.10f);


    // 初始化survivalTimeText_
    survivalTimeText_.setFont(Game::GlobalFont);
    survivalTimeText_.setString(sf::String(L"生存时间: 0 秒") );

    survivalTimeText_.setCharacterSize(Game::GlobalVideoMode.width / 25.0f);
    survivalTimeText_.setFillColor(Game::Color::Yellow);
    setOriginMiddle(survivalTimeText_);
    survivalTimeText_.setPosition(
            Game::GlobalVideoMode.width * 0.85f,
            Game::GlobalVideoMode.height * 0.95f); // 设置在屏幕右下角
}

void GameScreen::handleInput(sf::RenderWindow &window)
{
    snake_.handleInput(window);

    auto mousePosition = sf::Mouse::getPosition(window);

    pauseButton_.focused(false);

    if (pauseButton_.contain(mousePosition))
    {
        pauseButton_.focused(true);
        if (
            !Game::mouseButtonLocked &&
            sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            Game::mouseButtonCDtime = sf::Time::Zero;
            Game::mouseButtonLocked = true;
            Game::TmpGameScreen = Game::MainScreen;
            Game::MainScreen = std::make_shared<PauseScreen>();
            return;
        }
    }
}


void GameScreen::update(sf::Time delta)
{
    while (fruit_.size() < 10)
        generateFruit();

    snake_.update(delta);
    snake_.checkFruitCollisions(fruit_);

    // 更新最高分
    if (snake_.getScore() > sfSnake::Snake::highestScore) {
        sfSnake::Snake::highestScore = snake_.getScore();
    }
    // 更新生存时间
    sf::Time survivalTime = survivalClock_.getElapsedTime();
    int seconds = static_cast<int>(survivalTime.asSeconds());
    survivalTimeText_.setString( sf::String(L"生存时间:") + std::to_string(seconds) + sf::String(L"秒") );

    // 检查是否需要更新水果
    if (fruitUpdateClock_.getElapsedTime() >= fruitUpdateInterval_)
    {
        fruit_.clear(); // 清空现有的水果
        if(seconds %30 != 0){
            while (fruit_.size() < 10)
                generateFruit(); // 重新生成水果
        }


        fruitUpdateClock_.restart(); // 重置计时器
    }

    //奖励关卡，30秒生成一次超多水果，并且不会有炸弹
    if(seconds %30 == 0 && seconds != 0){
        fruit_.clear(); // 清空现有的水果
        while (fruit_.size() < 40)
            generateFruit_(); // 重新生成水果
    }

    highestScoreText_.setString(sf::String(L"最高分数:") + std::to_string(sfSnake::Snake::highestScore));

    if (snake_.hitSelf())
    {
        Game::MainScreen = std::make_shared<GameOverScreen>(snake_.getScore());
    }

    score_.setString(sf::String(L"当前分数:") + std::to_string(snake_.getScore()));
}

void GameScreen::render(sf::RenderWindow &window)
{
    if (Game::GridVisibility)
        grid_.render(window);
    snake_.render(window);
    for (auto fruit : fruit_)
        fruit.render(window);
    pauseButton_.render(window);
    window.draw(score_);
    window.draw(highestScoreText_); // 绘制最高分
    window.draw(survivalTimeText_);


}

void GameScreen::generateFruit()
{
    static std::default_random_engine engine(time(NULL));
    static std::default_random_engine colorEngine(time(NULL));

    static std::uniform_int_distribution<int> xPos(
        Game::GlobalVideoMode.width / 15.0f,
        Game::GlobalVideoMode.width -
            Game::GlobalVideoMode.width / 10.0f);

    static std::uniform_int_distribution<int> yPos(
        Game::GlobalVideoMode.width / 10.0f,
        Game::GlobalVideoMode.height -
            Game::GlobalVideoMode.width / 15.0f);

    static std::uniform_int_distribution<int> fruitColor(0, 11);

    switch (fruitColor(colorEngine))
    {
    case 0: // black
        fruit_.push_back(Fruit(
            sf::Vector2f(xPos(engine), yPos(engine)),
            Game::Color::Fruit[0],
            0));
        break;
    case 1: // brown
        fruit_.push_back(Fruit(
            sf::Vector2f(xPos(engine), yPos(engine)),
            Game::Color::Fruit[1],
            0));
        break;
    case 2:
    case 3:
    case 4: // red
        fruit_.push_back(Fruit(
            sf::Vector2f(xPos(engine), yPos(engine)),
            Game::Color::Fruit[2],
            3));
        break;
    case 5:
    case 6: // blue
        fruit_.push_back(Fruit(
            sf::Vector2f(xPos(engine), yPos(engine)),
            Game::Color::Fruit[3],
            2));
        break;
    case 7:
    case 8: // green
        fruit_.push_back(Fruit(
            sf::Vector2f(xPos(engine), yPos(engine)),
            Game::Color::Fruit[4],
            1));
        break;

    case 11: // green_BOOM
        fruit_.push_back(Fruit(
                sf::Vector2f(xPos(engine), yPos(engine)),
                Game::Color::Fruit[2],
                -3));
            break;

    case 9: // red_BOOM
        fruit_.push_back(Fruit(
                sf::Vector2f(xPos(engine), yPos(engine)),
                Game::Color::Fruit[2],
                -2));
            break;

    case 10: // black_bomb
        fruit_.push_back(Fruit(
                sf::Vector2f(xPos(engine), yPos(engine)),
                Game::Color::Fruit[2],
                -1));
            break;
    default:
        break;
    }
}
void GameScreen::generateFruit_()
{
    static std::default_random_engine engine(time(NULL));
    static std::default_random_engine colorEngine(time(NULL));

    static std::uniform_int_distribution<int> xPos(
            Game::GlobalVideoMode.width / 15.0f,
            Game::GlobalVideoMode.width -
            Game::GlobalVideoMode.width / 10.0f);

    static std::uniform_int_distribution<int> yPos(
            Game::GlobalVideoMode.width / 10.0f,
            Game::GlobalVideoMode.height -
            Game::GlobalVideoMode.width / 15.0f);

    static std::uniform_int_distribution<int> fruitColor(0, 8);

    switch (fruitColor(colorEngine))
    {
        case 0: // black
            fruit_.push_back(Fruit(
                    sf::Vector2f(xPos(engine), yPos(engine)),
                    Game::Color::Fruit[0],
                    0));
            break;
        case 1: // brown
            fruit_.push_back(Fruit(
                    sf::Vector2f(xPos(engine), yPos(engine)),
                    Game::Color::Fruit[1],
                    0));
            break;
        case 2:
        case 3:
        case 4: // red
            fruit_.push_back(Fruit(
                    sf::Vector2f(xPos(engine), yPos(engine)),
                    Game::Color::Fruit[2],
                    3));
            break;
        case 5:
        case 6: // blue
            fruit_.push_back(Fruit(
                    sf::Vector2f(xPos(engine), yPos(engine)),
                    Game::Color::Fruit[3],
                    2));
            break;
        case 7:
        case 8: // green
            fruit_.push_back(Fruit(
                    sf::Vector2f(xPos(engine), yPos(engine)),
                    Game::Color::Fruit[4],
                    1));
            break;


        default:
            break;
    }
}