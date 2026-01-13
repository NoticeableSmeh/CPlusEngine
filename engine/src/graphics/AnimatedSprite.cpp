#include "AnimatedSprite.h"
#include "Renderer.h"

AnimatedSprite::AnimatedSprite(Renderer& ren, std::string path, 
                               float w, float h, float x, float y,
                               int frameWidth, int frameHeight,
                               int numFrames, float frameDelay)
    : MovingSprite(ren, path, w, h, x, y),
      currentFrame(0),
      numFrames(numFrames),
      frameDelay(frameDelay),
      frameTimer(0.0f),
      isPlaying(true),
      frameWidth(frameWidth),
      frameHeight(frameHeight)
{
    updateFrame();
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::tick(float deltaTime)
{
    // Update movement via base class, and step animation if needed
    MovingSprite::tick(deltaTime);
    
    if (isPlaying)
    {
        frameTimer += deltaTime;
        
        if (frameTimer >= frameDelay)
        {
            frameTimer = 0.0f;
            currentFrame++;
            
            if (currentFrame >= numFrames)
            {
                currentFrame = 0;
            }
            
            updateFrame();
        }
    }
}

void AnimatedSprite::updateFrame()
{
    // Set source rectangle in spritesheet based on current frame index
    float srcX = currentFrame * frameWidth;
    float srcY = 0;
    
    setSourceRect(srcX, srcY, frameWidth, frameHeight);
}

void AnimatedSprite::play()
{
    isPlaying = true;
}

void AnimatedSprite::pause()
{
    isPlaying = false;
}

void AnimatedSprite::stop()
{
    // Stop and reset to first frame
    isPlaying = false;
    currentFrame = 0;
    frameTimer = 0.0f;
    updateFrame();
}

void AnimatedSprite::setFrameDelay(float delay)
{
    // Change animation tempo (seconds per frame)
    frameDelay = delay;
}