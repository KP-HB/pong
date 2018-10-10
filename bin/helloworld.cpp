
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ball.h"
#include "paddle.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

template <typename T>
//function to convert a non-string variable to a string.
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}


int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int Width = 800;
    const int Height = 600;
    const float paddleSpeed = 400.f;
    const int pointsForWin = 11;
    float ballRadius = 10.f;
    float startSpeed = 400.f;
    float maxSpeed = 800.f;
    float cpuSpeed = 0.f;
    float ballAngle = 0.f; // to be changed later
    sf::Vector2f paddleSize(25, 100);
    sf::Vector2f Origin(paddleSize / 2.f);
    sf::Vector2f center(ballRadius / 2, ballRadius / 2);

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(Width, Height, 32), "Space Pong",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    //Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("../resources/BOUNCE.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);

    sf::SoundBuffer playerMiss;
    if (!playerMiss.loadFromFile("../resources/sjam-defense.wav"))
        return EXIT_FAILURE;
    sf::Sound p2(playerMiss);

    sf::SoundBuffer cpuMiss;
    if (!cpuMiss.loadFromFile("../resources/sjam-uhoh.wav"))
        return EXIT_FAILURE;
    sf::Sound p1(cpuMiss);

    sf::SoundBuffer swish;
    if (!swish.loadFromFile("../resources/Swish.wav"))
        return EXIT_FAILURE;
    sf::Sound hoop (swish);

    sf::Music music;
    if(!music.openFromFile("../resources/sjam-inst.wav"))
       return EXIT_FAILURE;
    music.getLoop();
    music.setVolume(25);
    music.play();

    //load texture for the background
    sf::Texture texture;
    if (!texture.loadFromFile("../resources/sj.png"))
        return EXIT_FAILURE;
    sf::Sprite background(texture);

    // Create the paddles
    Paddle leftPaddle(paddleSize, Origin), rightPaddle(paddleSize, Origin);
    leftPaddle.setOrigin(paddleSize.x / 2, paddleSize.y / 2);
    rightPaddle.setOrigin(paddleSize.x / 2, paddleSize.y / 2);

    // Create the ball
    Ball ball(ballRadius, center);

    // Create Center Line
    sf::RectangleShape divider_line(sf::Vector2f(1, 600));
    divider_line.setPosition(sf::Vector2f(400, 0));


    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("../resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseScreen;
    pauseScreen.setFont(font);
    pauseScreen.setStyle(sf::Text::Bold);
    pauseScreen.setCharacterSize(40);
    pauseScreen.setPosition(170.f, 150.f);
    pauseScreen.setFillColor(sf::Color::White);
    pauseScreen.setString("Welcome to Space pong!\nPress space to start the game");

    // Set the Score
    sf::Text p1Score, cpuScore;
    p1Score.setFont(font);
    p1Score.setFillColor(sf::Color(255,255,0));
    p1Score.setString("0");
    p1Score.setCharacterSize(54);
    p1Score.setStyle(sf::Text::Bold);
    cpuScore = p1Score;
    p1Score.setPosition(100,100);
    cpuScore.setPosition(window.getSize().x - 100,100);

    // Define the paddle properties to help cpu
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.1f);

    sf::Clock clock;
    bool isPlaying = false;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if(leftPaddle.getScore() != pointsForWin || rightPaddle.getScore() != pointsForWin){
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
                {
                    if (!isPlaying)
                    {
                        // (re)start the game
                        isPlaying = true;
                        clock.restart();

                        // Reset the position of the paddles and ball
                        leftPaddle.setPosition(10 + paddleSize.x / 2, Height / 2);
                        rightPaddle.setPosition(Width - 10 - paddleSize.x / 2, Height / 2);
                        ball.setPosition(Width / 2, Height / 2);

                        // Reset the ball angle
                        do
                        {
                            // Make sure the ball initial angle is not too much vertical
                            ballAngle = (std::rand() % 360) * 2 * pi / 360;
                        }
                        while (std::abs(std::cos(ballAngle)) < 0.7f);
                    }
                }
            }
            // resetting game
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R))
                {
                    if (!isPlaying || isPlaying)
                    {
                        // (re)start the game
                        isPlaying = true;
                        clock.restart();

                        // Reset the position of the paddles and ball
                        leftPaddle.setPosition(10 + paddleSize.x / 2, Height / 2);
                        rightPaddle.setPosition(((Width - 10) - paddleSize.x / 2), Height / 2);
                        ball.setPosition(Width / 2, Height / 2);

                        // Reset Score
                        leftPaddle.score = 0;
                        p1Score.setString(to_string(leftPaddle.getScore()));
                        window.draw(p1Score);
                        rightPaddle.score = 0;
                        cpuScore.setString(to_string(leftPaddle.getScore()));
                        window.draw(cpuScore);
                        music.play();

                        // Reset the ball angle
                        do
                        {
                            // Make sure the ball initial angle is not too much vertical
                            ballAngle = (std::rand() % 360) * 2 * pi / 360;
                        }
                        while (std::abs(std::cos(ballAngle)) < 0.7f);
                    }
                }
        }

        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
               (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               (leftPaddle.getPosition().y + paddleSize.y / 2 < Height - 5.f))
            {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }

            // Move the computer's paddle
            if (((cpuSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((cpuSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < Height - 5.f)))
            {
                rightPaddle.move(0.f, cpuSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
                    cpuSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
                    cpuSpeed = -paddleSpeed;
                else
                    cpuSpeed = 0.f;
            }

            // Move the ball
            float factor = startSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen and give points
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                rightPaddle.incrementScore();
                cpuScore.setString(to_string(rightPaddle.getScore()));
                hoop.play();
                if(rightPaddle.score != pointsForWin){
                    pauseScreen.setString("CPU Scored! \nSpacebar for next round or\nescape to exit");
                    startSpeed = 400.f;
                }
                //final point cpu wins
                if(rightPaddle.score == pointsForWin){
                    pauseScreen.setString("CPU Won! \nR for new game or\nescape to exit");
                    music.stop();
                    p2.play();
                    leftPaddle.score = 0;
                    p1Score.setString(to_string(leftPaddle.getScore()));
                    window.draw(p1Score);
                    rightPaddle.score = 0;
                    cpuScore.setString(to_string(leftPaddle.getScore()));
                    window.draw(cpuScore);
                }
            }

            if (ball.getPosition().x + ballRadius > Width)
            {
                isPlaying = false;
                leftPaddle.incrementScore();
                p1Score.setString(to_string(leftPaddle.getScore()));
                hoop.play();
                if(leftPaddle.score != pointsForWin){
                    pauseScreen.setString("You Scored! \nSpacebar for next round  or\nescape to exit");
                    startSpeed = 400.f;
                }
                //final point, you win
                if(leftPaddle.score == pointsForWin){
                    pauseScreen.setString("You Won! \nR for new game or\nescape to exit");
                    music.stop();
                    p1.play();
                    leftPaddle.score = 0;
                    p1Score.setString(to_string(leftPaddle.getScore()));
                    window.draw(p1Score);
                    rightPaddle.score = 0;
                    cpuScore.setString(to_string(leftPaddle.getScore()));
                    window.draw(cpuScore);
                }
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > Height)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, Height - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                if (startSpeed < maxSpeed)
                {
                    startSpeed= startSpeed + 50.f;
                }
                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                if (startSpeed < maxSpeed)
                {
                    startSpeed= startSpeed + 50.f;
                }
                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }

        // Clear the window
        window.clear(sf::Color(0,0,0));
        window.draw(background);

        if (isPlaying)
        {
            // Draw the paddles ball, and score
            leftPaddle.Draw(&window);
            rightPaddle.Draw(&window);
            ball.Draw(&window);
            window.draw(p1Score);
            window.draw(cpuScore);
            window.draw(divider_line);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseScreen);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
