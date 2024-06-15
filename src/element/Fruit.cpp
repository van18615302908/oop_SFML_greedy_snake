#include <SFML/Graphics.hpp> // 包含SFML图形库的头文件

#include "Fruit.h" // 包含水果类的头文件
#include "Game.h" // 包含游戏类的头文件

using namespace sfSnake; // 使用sfSnake命名空间

Fruit::Fruit(sf::Vector2f position, sf::Color color, int score) // 水果类的构造函数，根据给定的位置、颜色和分数初始化水果对象
        : score_(score) // 初始化分数成员变量
{
    shape_.setRadius(Game::GlobalVideoMode.width / 256.0f); // 设置水果形状的半径为屏幕宽度的1/256
    setOriginMiddle(shape_); // 设置水果形状的原点为中心点

    if (score == -1) // 如果分数为-1，则生成炸弹
    {
        shape_.setFillColor(sf::Color::Black); // 设置炸弹的颜色为黑色
        shape_.setPointCount(8); // 将炸弹设置为八边形（或其他你想要的形状）
        shape_.setRadius(Game::GlobalVideoMode.width / 128.0f); // 设置炸弹的半径为屏幕宽度的1/128（比水果大一些）
    }
    else if (score == -2)
    {
        shape_.setFillColor(sf::Color::Red); // 设置炸弹的颜色为红色
        shape_.setPointCount(16); // 将炸弹设置为八边形（或其他你想要的形状）
        shape_.setRadius(Game::GlobalVideoMode.width / 64.0f); // 设置炸弹的半径为屏幕宽度的1/64（比水果大一些）
    }
    else if(score == -3)
    {
        shape_.setFillColor(sf::Color::Green); // 设置炸弹的颜色为绿色
        shape_.setPointCount(16); // 将炸弹设置为八边形（或其他你想要的形状）
        shape_.setRadius(Game::GlobalVideoMode.width / 64.0f); // 设置炸弹的半径为屏幕宽度的1/64（比水果大一些）
    }
    else // 否则生成普通水果
    {
        shape_.setFillColor(color); // 设置水果形状的颜色
    }

    shape_.setPosition(position); // 设置水果形状的位置
}

void Fruit::render(sf::RenderWindow &window) // 渲染水果对象到窗口
{
    window.draw(shape_); // 在窗口上绘制水果形状
}
