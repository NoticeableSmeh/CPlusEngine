#include <iostream>
#include <SDL3/SDL.h>


#ifndef SPRITE_H
#define SPRITE_H

class Renderer;

class Sprite
{
    public:

    // Default constructor
    Sprite();

    // Constructor without position, no spritesheet
    Sprite(Renderer& ren, std::string path, float width, float height);

    // Constructor with position, no spritesheet
    Sprite(Renderer& ren, std::string path, float width, 
        float height, float xPosition, float yPosition);

    // Spritesheet: srcX/srcY = start position from top left corner
    // srcW/srcH = clipping area 
    Sprite(Renderer& ren, std::string path, float w, float h, float x, float y, float srcX, float srcY, float srcW, float srcH);

    // Destructor
    virtual ~Sprite();

    // Prohibit value semantics (no copying)
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;
    
    // Render sprite
    virtual void render(Renderer& ren);

    // Load sprite after construction, no spritesheet
    void load(Renderer& ren, std::string path, float x, float y, float width, float height);

    // Update per frame
    virtual void tick(float deltaTime);
    
    // Set source rectangle (spritesheet cutout)
    void setSourceRect(float x, float y, float w, float h);

    void setSourceRect(const SDL_FRect& rect);

    // Get underlying SDL texture
    SDL_Texture* returnNativeSprite() const;

    // Get position and size
    float getX();

    float getY();

    float getWidth();

    float getHeight();

    // Bounding rect for simple collision detection
    SDL_FRect getBoundingRect() const;
    
    // Move sprite by delta
    void addPosition(float dx, float dy);

    private:

    

    protected: 
    float width, height;    // Size in pixels
    float xPosition, yPosition; // Position on screen
    SDL_FRect* sourceRect;  // Source rectangle for spritesheet 
    SDL_Texture* p_sprite;  // SDL texture representing the sprite image
    
};



#endif