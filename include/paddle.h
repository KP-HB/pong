#ifndef PADDLE_H
#define PADDLE_H
#include <SFML/Graphics.hpp>

class Paddle
{
    public:
        Paddle(sf::Vector2f size, sf::Vector2f center);
        ~Paddle();
        float paddleSpeed();
        float cpuSpeed();
        int getScore();
        void Draw(sf::RenderWindow* win);
        void incrementScore();
        void setPosition(int x, int y);
        void setOrigin(float x, float y);
        void move(float x,float y);
        int score;
        sf::Vector2f getPosition();
        sf::Vector2f paddleSize(int x, int y);

    private:
        sf::RectangleShape rectShape;
};
#endif
