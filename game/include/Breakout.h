#ifndef BREAKOUT_H
#define BREAKOUT_H

#include "GameEngine.h"

class Breakout : public GameEngine
{
public:
    // Constructor and destructor
    Breakout(std::string title, int width, int height, int targetfps);
    ~Breakout();
    
    // No copy constructor or assignment
    Breakout(const Breakout&) = delete;
    Breakout& operator=(const Breakout&) = delete;

    // Game-specific update method
    void gameUpdate(double deltaTime) override;
    // Update method that handles input and game updates
    void update(double deltaTime) override;
    
private:
    // Game-specific methods
    void initGame();
    // Handles collisions between ball and walls, paddle and bricks
    void handleCollisions();
    // Checks collisions between ball and bricks
    void checkBrickCollisions();
    // Resets ball position and velocity
    void resetBall();
    
    MovingSprite* paddle;            // Player's paddle
    MovingSprite* ball;              // The ball
    std::vector<Sprite*> bricks;     // Bricks to break
    TextSprite* livesText;           // HUD: lives text
    
    int lives;       // Number of lives left
    int score;       // Score
    bool ballLaunched; // Whether the ball has been launched
    
    // Constant values for the game, this is for the game so don't want them in Constants.h
    static constexpr float PADDLE_SPEED = 400.0f;
    static constexpr float BALL_SPEED = 300.0f;
    static constexpr int BRICK_ROWS = 5;
    static constexpr int BRICK_COLS = 10;
    static constexpr int BRICK_WIDTH = 60;
    static constexpr int BRICK_HEIGHT = 20;
    static constexpr int BRICK_PADDING = 5;
};

#endif
