#include "ball.h"

Ball::Ball(float radius, sf::Vector2f center)
{
    circleShape.setRadius(radius - 3);
    circleShape.setOutlineThickness(3);
    circleShape.setOutlineColor(sf::Color::Black);
    circleShape.setFillColor(sf::Color(255,165,0));
}

void Ball::Draw(sf::RenderWindow* win)
{
    win->draw(circleShape);
}


void Ball::move(float x,float y)
{
    circleShape.move(x,y);
}

void Ball::setPosition(int x, int y)
{
    circleShape.setPosition(x, y);
}

sf::Vector2f Ball::getPosition()
{
    return circleShape.getPosition();
}
