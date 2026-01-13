#include "Breakout.h"
#include <cmath>


// Constructor for the game sets the values
Breakout::Breakout(std::string title, int width, int height, int targetfps) 
    : GameEngine(title, width, height, targetfps)
{
    paddle = nullptr;
    ball = nullptr;
    livesText = nullptr;
    lives = 3;
    score = 0;
    ballLaunched = false;
    
    // Initialize pointers and game status
    // Load sounds at construction
    AudioManager::loadSound("ballbounce", constants::ballbounce_str);

    AudioManager::loadSound("destroybrick", constants::destroybrick_str);
    
    initGame();
}


// Destructor removes and cleans up Sprites
Breakout::~Breakout()
{
    // Create game objects and scene content
    removeSprite(paddle);
    removeSprite(ball);
    removeSprite(livesText);
    

    // Remove all bricks
    for (size_t i = 0; i < bricks.size(); i++) 
    // Remove HUD and game objects from engine
    {
        removeSprite(bricks[i]);
    }
}

    // Remove all bricks
// Sets up the game with paddle, ball and bricks
void Breakout::initGame()
{
    // Variables for paddle position and size
    float paddleWidth = 100;
    float paddleHeight = 20;


    // Paddle position
    float paddleX = constants::gScreenWidth / 2 - paddleWidth / 2;
    float paddleY = constants::gScreenHeight - 50;


    // Dynamically create paddle and ball
    paddle = new MovingSprite(getRenderer(), constants::paddle_str, paddleWidth, paddleHeight, paddleX, paddleY);
    addSprite(paddle);
    
    float ballSize = 10;
    float ballX = constants::gScreenWidth / 2 - ballSize / 2;
    float ballY = constants::gScreenHeight - 80;

    // Dynamically create ball
    ball = new MovingSprite(getRenderer(), constants::ball_str, ballSize, ballSize, ballX, ballY);
    addSprite(ball);
    

    // Create and display lives text
    livesText = new TextSprite(getRenderer(), constants::arial_font, 24, "Lives: " + std::to_string(lives), {255, 255, 255, 255}, 10, 10);
    addSprite(livesText);
    
    // Create bricks to destroy
    int totalBricksWidth = BRICK_COLS * (BRICK_WIDTH + BRICK_PADDING);
    int bricksStartX = (constants::gScreenWidth - totalBricksWidth) / 2;
    int bricksStartY = 50;
    

    // Loop to create rows and columns of bricks
    for (int row = 0; row < BRICK_ROWS; row++) {
        std::string brickImage;
        
        if (row == 0) {
            brickImage = constants::brick_red_str;
        } else if (row == 1) 
        {
            brickImage = constants::brick_orange_str;
        } else if (row == 2) 
        {
            brickImage = constants::brick_yellow_str;
        } else if (row == 3) 
        {
            brickImage = constants::brick_green_str;
        } else 
        {
            brickImage = constants::brick_blue_str;
        }
        
        for (int col = 0; col < BRICK_COLS; col++) 
        {
            // Calculate position for each brick
            float brickX = bricksStartX + col * (BRICK_WIDTH + BRICK_PADDING);
            float brickY = bricksStartY + row * (BRICK_HEIGHT + BRICK_PADDING);
            
            // Dynamically create brick
            Sprite* brick = new Sprite(getRenderer(), brickImage, BRICK_WIDTH, BRICK_HEIGHT, brickX, brickY);
            bricks.push_back(brick);
            addSprite(brick);
        }
    }
    

    // Reset ball status
    ballLaunched = false;
}


// Update method that handles input and game updates
void Breakout::update(double deltaTime)
{
    GameEngine::update(deltaTime);
    

    // Handle paddle input and ball launch
    bool leftPressed = InputManager::getKey(Key::Left) || InputManager::getKey(Key::A);
    bool rightPressed = InputManager::getKey(Key::Right) || InputManager::getKey(Key::D);
    bool spacePressed = InputManager::getKey(Key::Space);
    

    // Update paddle movement based on input
    if (leftPressed) 
    {
        paddle->setXVelocity(-PADDLE_SPEED);
    } else if (rightPressed) 
    {
        paddle->setXVelocity(PADDLE_SPEED);
    } else 
    {
        paddle->setXVelocity(0);
    }
    

    // Constrain paddle within screen
    if (paddle->getX() < 0) 
    {
        paddle->addPosition(-paddle->getX(), 0);
    }
    if (paddle->getX() + paddle->getWidth() > constants::gScreenWidth) 
    {
        float overX = (paddle->getX() + paddle->getWidth()) - constants::gScreenWidth;
        paddle->addPosition(-overX, 0);
    }

    // Launch ball if spacebar is pressed
    if (spacePressed && !ballLaunched) 
    {

        ballLaunched = true;

        ball->setXVelocity(BALL_SPEED * 0.5f);
        ball->setYVelocity(-BALL_SPEED);

    }
    
    gameUpdate(deltaTime);
}

void Breakout::gameUpdate(double deltaTime)
{
    if (!ballLaunched) 
    {

        // Ball follows paddle before launch
        float ballX = paddle->getX() + paddle->getWidth() / 2 - ball->getWidth() / 2;
        float ballY = paddle->getY() - ball->getHeight() - 10;
        float deltaX = ballX - ball->getX();
        float deltaY = ballY - ball->getY();

        ball->addPosition(deltaX, deltaY);
    }
    
    handleCollisions();
}
// Handles collisions between ball and walls, paddle and bricks
void Breakout::handleCollisions()
{
    // Wall collision
    if (ball->getX() <= 0 && ball->getXVelocity() < 0) 
    {


        ball->addPosition(-ball->getX(), 0); 
        ball->setXVelocity(-ball->getXVelocity());
        AudioManager::playSound("ballbounce");
    } 
    // Right wall
    if (ball->getX() + ball->getWidth() >= constants::gScreenWidth && ball->getXVelocity() > 0) 
    {
        float overlapX = (ball->getX() + ball->getWidth()) - constants::gScreenWidth;
        ball->addPosition(-overlapX, 0); 
        ball->setXVelocity(-ball->getXVelocity());
        AudioManager::playSound("ballbounce");
    }
    // Top wall
    if (ball->getY() <= 0 && ball->getYVelocity() < 0)
    {
        ball->addPosition(0, -ball->getY()); 
        ball->setYVelocity(-ball->getYVelocity());
        AudioManager::playSound("ballbounce");
    }
    // Bottom wall
    if (ball->getY() >= constants::gScreenHeight)
    {
        lives--;
        livesText->setText(getRenderer(), "Lives: " + std::to_string(lives));
        resetBall();
        
        if (lives <= 0) 
        {
            lives = 3;
            score = 0;
            initGame();
        }
    }
    

    // Collision with paddle
    bool ballMovingDown = ball->getYVelocity() > 0;
    if (checkCollision(*ball, *paddle) && ballMovingDown)
    {
        AudioManager::playSound("ballbounce");
        
        float ballCenterX = ball->getX() + ball->getWidth() / 2;
        float paddleCenterX = paddle->getX() + paddle->getWidth() / 2;
        float hitPosition = (ballCenterX - paddleCenterX) / (paddle->getWidth() / 2);
        
        ball->setXVelocity(BALL_SPEED * hitPosition);
        ball->setYVelocity(-BALL_SPEED);
        
        float desiredY = paddle->getY() - ball->getHeight();
        float deltaY = desiredY - ball->getY();
        ball->addPosition(0, deltaY);
        
        float minSpeed = 50.0f;


        if (std::abs(ball->getXVelocity()) < minSpeed) 
        {
            if (hitPosition >= 0) 
            {
                ball->setXVelocity(minSpeed);
            } else 
            {
                ball->setXVelocity(-minSpeed);
            }
        }
    }
    
    checkBrickCollisions();
}

// Checks collisions between the ball and bricks
void Breakout::checkBrickCollisions()
{
    for (size_t i = 0; i < bricks.size(); i++) 
    {
        if (checkCollision(*ball, *bricks[i])) 
        {
            AudioManager::playSound("destroybrick");
            
            removeSprite(bricks[i]);
            bricks.erase(bricks.begin() + i);
            
            ball->setYVelocity(-ball->getYVelocity());
            score += 10;
            
            if (bricks.size() == 0) 
            {
                resetBall();
                initGame();
            }
            
            break;
        }
    }
}

// Resets the ball's position and velocity
void Breakout::resetBall()
{
    ballLaunched = false;
    ball->setXVelocity(0);
    ball->setYVelocity(0);
    
    float ballX = paddle->getX() + paddle->getWidth() / 2 - ball->getWidth() / 2;
    float ballY = paddle->getY() - ball->getHeight() - 10;
    float deltaX = ballX - ball->getX();
    float deltaY = ballY - ball->getY();
    
    ball->addPosition(deltaX, deltaY);
}
