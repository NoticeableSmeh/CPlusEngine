#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include "Sprite.h"

class MovingSprite : public Sprite 
{
    public:
    // Constructors and destructor
    MovingSprite(Renderer& ren, std::string path, float w, float h, float x, 
    float y, float srcX, float srcY, float srcW, float srcH);

    MovingSprite(Renderer& ren, std::string path, float width, float height);

    MovingSprite(Renderer& ren, std::string path, float width, 
        float height, float xPosition, float yPosition);

    ~MovingSprite() override;

    // No copy constructor or assignment
    MovingSprite(const MovingSprite&) = delete;
    MovingSprite& operator=(const MovingSprite&) = delete;
    
    void render(Renderer& ren) override;

    void tick(float deltaTime) override;
    
    // Change velocity directly 
    void changeVelocity(float xValue, float yValue);

    // Velocity getters/setters
    float getXVelocity();

    float getYVelocity();
    
    void setXVelocity(float value);
    
    void setYVelocity(float value);
    
    void addXVelocity(float value);
    
    void addYVelocity(float value);
    
    void addVelocity(float xValue, float yValue);

    void subtractXVelocity(float value);
    
    void subtractYVelocity(float value);
    
    void subtractVelocity(float xValue, float yValue);
    

    // Physics methods
    void setGravity(float gravity);
    float getGravity() const { return gravityForce; }
    
    // Elasticity for bouncing
    void setElasticity(float elasticity);
    float getElasticity() const { return elasticity; }
    
    // Mass for physics calculations
    void setMass(float mass);
    float getMass() const { return mass; }
    
    // Enable or disable physics
    void applyPhysics(bool enable) { physicsEnabled = enable; }
    bool hasPhysics() const { return physicsEnabled; }

    private:
    float xVelocity, yVelocity; // Velocity in pixels per second
    
    float gravityForce;        // Gravity strength
    float elasticity;          // Bounce factor (0-1)
    float mass;                // Weight for physics
    bool physicsEnabled;       // Whether physics should be applied

};


#endif