#ifndef BALL_H
#define BALL_H
#include <SFML/Graphics.hpp>

class Ball
{
    public:
        Ball(float radius, sf::Vector2f center);
        void setPosition(int x, int y);
        void move(float x, float y);
        void Draw(sf::RenderWindow* win);
        sf::Vector2f getPosition();
    private:
        sf::CircleShape circleShape;

};

#endif // BALL_H
