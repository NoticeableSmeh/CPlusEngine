#include "SDL3/SDL.h"
#include <SDL3_image/SDL_image.h> 

#include "Sprite.h"
#include "Renderer.h"

Sprite::Sprite(Renderer& ren, std::string path, float width, float height)
{   
    // Initialize base size and default position
    this->height = height;
    this->width = width;
    this->sourceRect = nullptr;
    this->xPosition = 0;
    this->yPosition = 0;

    // Load texture from file
    p_sprite = IMG_LoadTexture(ren.returnNativeRenderer(), path.c_str());

    if(!p_sprite){
        std::cout << "Failed to load texture: " << path << " - " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Loaded texture: " << path << std::endl;
    }
}


Sprite::Sprite(Renderer& ren, std::string path, float width, float height, 
    float xPosition, float yPosition)
{
    // Initialize with given position
    this->height = height;
    this->width = width;
    this->xPosition = xPosition;
    this->yPosition = yPosition;
    this->sourceRect = nullptr;

    // Load texture from file
    p_sprite = IMG_LoadTexture(ren.returnNativeRenderer(), path.c_str());

    if(!p_sprite)
    {
        std::cout << "Failed to load texture: " << path << " - " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Loaded texture: " << path << std::endl;
    }
}

Sprite::Sprite()
{
    // Empty sprite; requires loading before render
    p_sprite = nullptr;
    sourceRect = nullptr;
    width = 0;
    height = 0;
    xPosition = 0;
    yPosition = 0;
}

Sprite::Sprite(Renderer& ren, std::string path, float w, float h, float x, float y, 
    float srcX, float srcY, float srcW, float srcH) 
: Sprite(ren, path, w, h, x, y)
{
    // Spritesheet source rectangle (cutout)
    sourceRect = new SDL_FRect{srcX, srcY, srcW, srcH};
}

Sprite::~Sprite()
{
    if(p_sprite)
    {
        // Free texture
        SDL_DestroyTexture(p_sprite);
    }
    
    delete sourceRect;
}

void Sprite::setSourceRect(float x, float y, float w, float h) {
    // Create source rectangle if missing and update values
    if (!sourceRect) sourceRect = new SDL_FRect();
    sourceRect->x = x;
    sourceRect->y = y;
    sourceRect->w = w;
    sourceRect->h = h;
}

void Sprite::setSourceRect(const SDL_FRect& rect) {
    // Copy source rectangle
    if (!sourceRect) sourceRect = new SDL_FRect();
    *sourceRect = rect;
}

SDL_Texture* Sprite::returnNativeSprite() const
{
    return p_sprite;
}

float Sprite::getHeight()
{
    return Sprite::height;
}

float Sprite::getWidth()
{
    return Sprite::width;
}

float Sprite::getX()
{
    return Sprite::xPosition;
}

float Sprite::getY()
{
    return Sprite::yPosition;
}

SDL_FRect Sprite::getBoundingRect() const
{
    // Rectangle for simple collision
    return SDL_FRect{xPosition, yPosition, width, height};
}
void Sprite::addPosition(float dx, float dy)
{
    // Move sprite by delta
    xPosition += dx;
    yPosition += dy;
}
void Sprite::render(Renderer& ren) {
    // Draw sprite; use source rectangle if set
    SDL_FRect dstRect{xPosition, yPosition, width, height};
    SDL_RenderTexture(ren.returnNativeRenderer(), p_sprite, sourceRect, &dstRect);  
}

void Sprite::load(Renderer& ren, std::string path, float x, float y, float width, float height)
{
    // Load texture and update size/position
    this->height = height;
    this->width = width;
    this->xPosition = x;
    this->yPosition = y;
    
    p_sprite = IMG_LoadTexture(ren.returnNativeRenderer(), path.c_str());        
}

void Sprite::tick(float deltaTime) 
{
    
}
