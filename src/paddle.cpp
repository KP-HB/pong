#include "paddle.h"

Paddle::Paddle(sf::Vector2f size, sf::Vector2f center)
{
    sf::Vector2f paddleSize(25, 100);
    rectShape.setSize(paddleSize - sf::Vector2f(3, 3));
    rectShape.setOutlineThickness(3);
    rectShape.setOutlineColor(sf::Color(0,0,0));
    rectShape.setFillColor(sf::Color(255,255,255));
    score=0;
}

void Paddle::Draw(sf::RenderWindow* win)
{
    win->draw(rectShape);
}

Paddle::~Paddle()
{

}

void Paddle::incrementScore()
{
    score++;
}

int Paddle::getScore()
{
    return score;
}

void Paddle::setPosition(int x, int y)
{
    rectShape.setPosition(x, y);
}

void Paddle::setOrigin(float x, float y)
{
    rectShape.setOrigin(x,y);
}

void Paddle::move(float x,float y)
{
    rectShape.move(x,y);
}


sf::Vector2f Paddle::getPosition()
{
    return rectShape.getPosition();
}
