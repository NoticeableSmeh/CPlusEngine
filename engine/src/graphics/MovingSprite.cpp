#include "SDL3/SDL.h"
#include <SDL3_image/SDL_image.h> 

#include "MovingSprite.h"
#include "Renderer.h"

// For spritesheets, srcX, srcY = the position you want to start with on the spreedsheet from the top left
// srcW, srcH is for marking the area in which the sprite is covering, 
// Takes the pixels from srcX, srcY to srcW, srcH marking that area like a rectangle selection tool
MovingSprite::MovingSprite(Renderer& ren, std::string path, float w, float h, float x, float y, 
    float srcX, float srcY, float srcW, float srcH) 
: MovingSprite(ren, path, w, h, x, y)
{
    // Cut out/mark source rectangle for current frame
    sourceRect = new SDL_FRect{srcX, srcY, srcW, srcH};
}

MovingSprite::MovingSprite(Renderer& ren, std::string path, float width, float height)
{   
    // Initialize base data and physics
    this->height = height;
    this->width = width;
    this->xPosition = 0;     
    this->yPosition = 0;      
    this->sourceRect = nullptr;  
    this->xVelocity = 0;
    this->yVelocity = 0;
    this->gravityForce = 0.0f;
    this->elasticity = 0.8f;
    this->mass = 1.0f;
    this->physicsEnabled = false;

    p_sprite = IMG_LoadTexture(ren.returnNativeRenderer(), path.c_str());

    if(!p_sprite){
        std::cout << "MovingSprite Failed to load texture: " << path << " - " << SDL_GetError() << std::endl;
    } else {
        std::cout << "MovingSprite Loaded texture: " << path << std::endl;
    }
}

MovingSprite::MovingSprite(Renderer& ren, std::string path, float width, 
        float height, float xPosition, float yPosition)
{
    // Initialize positioned sprite and physics
    this->height = height;
    this->width = width;
    this->xPosition = xPosition;
    this->yPosition = yPosition;
    this->sourceRect = nullptr; 
    this->xVelocity = 0;
    this->yVelocity = 0;
    this->gravityForce = 0.0f;
    this->elasticity = 0.8f;
    this->mass = 1.0f;
    this->physicsEnabled = false;

    p_sprite = IMG_LoadTexture(ren.returnNativeRenderer(), path.c_str());

    if(!p_sprite)
    {
        std::cout << "MovingSprite Failed to load texture: " << path << " - " << SDL_GetError() << std::endl;
    } else {
        std::cout << "MovingSprite Loaded texture: " << path << std::endl;
    }      
}


MovingSprite::~MovingSprite()
{

}

void MovingSprite::render(Renderer& ren) 
{
    // Draw sprite with optional source rectangle
    SDL_FRect dstRect{xPosition, yPosition, width, height};
    SDL_RenderTexture(ren.returnNativeRenderer(), p_sprite, sourceRect, &dstRect);
}

void MovingSprite::tick(float deltaTime)
{
    // Apply gravity if active and update position
    if (physicsEnabled && gravityForce != 0.0f)
    {
        yVelocity += gravityForce * deltaTime;
    }
    
    xPosition += xVelocity * deltaTime;
    yPosition += yVelocity * deltaTime;
}

void MovingSprite::changeVelocity(float xValue, float yValue)
{
    // Replace velocity with new components
    this->xVelocity = xValue;
    this->yVelocity = yValue;
}


float MovingSprite::getXVelocity()
{
    return xVelocity;
}

float MovingSprite::getYVelocity()
{
    return yVelocity;
}

void MovingSprite::setXVelocity(float value)
{
    xVelocity = value;
}

void MovingSprite::setYVelocity(float value)
{
    yVelocity = value;
}

void MovingSprite::addXVelocity(float value)
{
    xVelocity += value;
}

void MovingSprite::addYVelocity(float value)
{
    yVelocity += value;
}

void MovingSprite::addVelocity(float xValue, float yValue)
{
    xVelocity += xValue;
    yVelocity += yValue;
}

void MovingSprite::subtractXVelocity(float value)
{
    xVelocity -= value;
}

void MovingSprite::subtractYVelocity(float value)
{
    yVelocity -= value;
}

void MovingSprite::subtractVelocity(float xValue, float yValue)
{
    xVelocity -= xValue;
    yVelocity -= yValue;
}

void MovingSprite::setGravity(float gravity)
{
    // Set gravity acceleration 
    gravityForce = gravity;
}

void MovingSprite::setElasticity(float elast)
{
    // Min/max value
    elasticity = elast;
    if (elasticity < 0.0f) elasticity = 0.0f;
    if (elasticity > 1.0f) elasticity = 1.0f;
}

void MovingSprite::setMass(float m)
{
    // Set mass 
    mass = m;
    if (mass <= 0.0f) mass = 0.1f;
}
