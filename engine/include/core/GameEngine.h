#include <iostream>
#include <SDL3/SDL.h>             
#include <SDL3_image/SDL_image.h> 
#include <SDL3_ttf/SDL_ttf.h>  
#include <vector>
#include <algorithm>
#include <functional>  
#include <map>         

#include "Constants.h"
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Scene.h"  

//Sprites
#include "Sprite.h"
#include "MovingSprite.h"
#include "AnimatedSprite.h"
#include "TextSprite.h"
#include "TextInputField.h"

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class GameEngine
{
public:
// Constructor and destructor
    GameEngine(std::string title, int width, int height, int targetfps);

    GameEngine(const GameEngine&) = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    ~GameEngine();

    // Main method to run the game
    void run();

    // Initializes SDL and creates window and renderer
    bool initialize(std::string title, int width, int height, int targetfps);

    // Handles input and updates
    void input();

    // Update method that can be overridden by subclasses
    virtual void update(double deltaTime);
    
    // Game-specific update method that can be overridden
    virtual void gameUpdate(double deltaTime){}
    

    // Gets the renderer and event queue
    Renderer& getRenderer() {return gameRenderer; }

    // Gets event queue
    const std::vector<SDL_Event>& getEvents() const { return eventQueue; }

    // Rendering method
    void render();

    // Shuts down SDL, cleans up memory
    void closeSDL();

    // Sprite handling and collisions
    void addSprite(Sprite* sprite);

    // Removes sprite from game
    void removeSprite(Sprite* sprite);

    bool checkCollision(const Sprite& a, const Sprite& b);
    // Pixel perfect collision between two sprites

    bool checkPixelPerfectCollision(const Sprite& a, const Sprite& b);
    // Handle physics collision between two MovingSprites

    void handlePhysicsCollision(MovingSprite& a, MovingSprite& b);
    // Add scene

    void addScene(Scene* scene);
    // Sets the active scene
    void setActiveScene(const std::string& sceneName);

    // Gets the active scene
    Scene* getActiveScene() { return activeScene; }

    // Gets a scene by name
    Scene* getScene(const std::string& sceneName);

    // Register key callback
    void registerKeyCallback(Key key, std::function<void()> callback);

    // Unregister key callback
    void unregisterKeyCallback(Key key);
    

    // Member function to register member function callbacks with Templates
    template<typename T>
    void registerKeyCallback(Key key, T* object, void (T::*memberFunc)())
    {
        keyCallbacks[key] = [object, memberFunc]() {
            (object->*memberFunc)();
        };
    }

private: 
    Renderer gameRenderer;   // Renderer for drawing sprites/scenes
    Window gameWindow;       // SDL window that the engine uses
    
    std::vector<Sprite*> sprites; // All sprites that the engine updates/draws
    
    std::vector<Scene*> scenes;   // Registered scenes
    Scene* activeScene;           // Current active scene

    int targetFPS;        // Target frames per second
    Uint32 frameStart;    // Start time for current frame (ms)
    Uint32 frameTime;     // Time spent on current frame (ms)
    Uint32 frameDelay;    // Delay to maintain targetFPS (ms)

    Uint64 lastCounter;   // Last high-resolution counter value
    double deltaTime;     // Time delta in seconds per iteration

    std::map<Key, std::function<void()>> keyCallbacks; // Shortcuts -> callback
    void processKeyCallbacks();                         // Execute registered callbacks

    std::vector<SDL_Event> eventQueue; // Queue of incoming SDL events

    bool quit = false;    // Flag to exit game loop
};

#endif