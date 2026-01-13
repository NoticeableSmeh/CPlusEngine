#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H


#include "MovingSprite.h"
#include <vector>


class AnimatedSprite : public MovingSprite
{
    public:

    // Animated sprite with spritesheet parameters 
    AnimatedSprite(Renderer& ren, std::string path, float w, float h, float x, float y, 
        int frameWidth, int frameHeight, int numFrames, float frameDelay); 


    AnimatedSprite(const AnimatedSprite&) = delete;

    AnimatedSprite& operator=(const AnimatedSprite&) = delete;

    ~AnimatedSprite() override;

    // Update movement and step animation based on frameDelay
    void tick(float deltaTime) override;

    // Start animation
    void play();

    // Pause animation
    void pause();

    // Stop and reset animation
    void stop();

    // Set time delay between frames 
    void setFrameDelay(float delay);

    private: 
    int currentFrame;   // Current frame index
    int numFrames;      // Number of frames in spritesheet
    float frameDelay;   // Time between frames
    float frameTimer;   // Accumulated time since last frame change
    bool isPlaying;     // Whether animation is active

    int frameWidth;     // Width per frame in spritesheet
    int frameHeight;    // Height per frame in spritesheet

    void updateFrame(); // Calculate and set source rectangle for current frame

    
};

#endif