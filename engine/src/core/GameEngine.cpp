#include "GameEngine.h"
#include <cmath>

SDL_Event e;

GameEngine::GameEngine(std::string title, int width, int height, int targetFPS)
    : activeScene(nullptr)  
{
    // Initialize SDL, window, renderer and audio
    initialize(title, width, height, targetFPS);
    // High-resolution timer start for deltaTime calculation
    lastCounter = SDL_GetPerformanceCounter();
}

void GameEngine::run()
{
    // Main loop = collect events, update, render, maintain FPS
    SDL_zero(e);
    while (quit == false)
    {
        frameStart = SDL_GetTicks();

        Uint64 currentCounter = SDL_GetPerformanceCounter();
        Uint64 counterDiff = currentCounter - lastCounter;
        double frequency = static_cast<double>(SDL_GetPerformanceFrequency());
        deltaTime = static_cast<double>(counterDiff) / frequency;
        lastCounter = currentCounter;

        // Collect SDL events for later processing
        eventQueue.clear();
        while (SDL_PollEvent(&e))
        {
            eventQueue.push_back(e);
        }

        gameUpdate(deltaTime);
        input();
        processKeyCallbacks();
        update(deltaTime);
        render();
        
        // FPS regulation via delay
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    closeSDL();
}

void GameEngine::input()
{
    // Handle global quit events
    for (const auto& event : eventQueue)
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            quit = true;
        }
    }
    
    // Update InputManager with all events
    InputManager::update(eventQueue);

    if (InputManager::shouldQuit())
    {
        quit = true;
    }
}

void GameEngine::processKeyCallbacks()
{
    // Execute registered callbacks on key down
    for (auto& pair : keyCallbacks)
    {
        Key key = pair.first;
        std::function<void()>& callback = pair.second;
        
        if (InputManager::getKeyDown(key))
        {
            callback();
        }
    }
}

void GameEngine::registerKeyCallback(Key key, std::function<void()> callback)
{
    // Link key to callback function
    keyCallbacks[key] = callback;
}

void GameEngine::unregisterKeyCallback(Key key)
{
    // Remove link for key
    keyCallbacks.erase(key);
}

bool GameEngine::initialize(std::string title, int width, int height, int targetFPS)
{
    this->targetFPS = targetFPS;
    frameDelay = 1000 / targetFPS;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == false)
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }
    
    if (TTF_Init() == false)
    {
        SDL_Log("TTF could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }
    
    if (!AudioManager::initialize())
    {
        SDL_Log("AudioManager could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    // Create window and linked renderer
    gameWindow.initializeWindow(title, width, height);
    gameRenderer.initializeRenderer(gameWindow);

    return true;
}

void GameEngine::update(double deltaTime)
{
    // Update old sprites (before scenes were added)
    for (auto &s : sprites)
    {
        s->tick(deltaTime);
    }
    
    // Update active scene if set
    if (activeScene)
    {
        activeScene->update(deltaTime);
    }
}

void GameEngine::render()
{
    // Clear screen and render
    gameRenderer.setColor(0, 0, 0, 255);
    gameRenderer.clear();

    // Render old sprites (before scenes were added)
    for (auto &sprite : sprites)
    {
        sprite->render(gameRenderer);
    }
    
    // Render active scene
    if (activeScene)
    {
        activeScene->render(gameRenderer);
    }

    gameRenderer.present();
}

void GameEngine::closeSDL()
{
    // Shut down systems in correct order
    AudioManager::shutdown();
    TTF_Quit();
    SDL_Quit();
}

void GameEngine::addSprite(Sprite *sprite)
{
    // Add sprite to old system
    sprites.push_back(sprite);
}

void GameEngine::removeSprite(Sprite *sprite)
{
    // Remove first occurrence of sprite
    auto it = std::find(sprites.begin(), sprites.end(), sprite);
    if (it != sprites.end())
    {
        sprites.erase(it);
    }
}

bool GameEngine::checkCollision(const Sprite &a, const Sprite &b)
{
    // AABB collision between two sprites
    SDL_FRect rectA = a.getBoundingRect();
    SDL_FRect rectB = b.getBoundingRect();
    return SDL_HasRectIntersectionFloat(&rectA, &rectB);
}

bool GameEngine::checkPixelPerfectCollision(const Sprite& a, const Sprite& b)
{
    if (!checkCollision(a, b))
        return false;
    
    SDL_FRect rectA = a.getBoundingRect();
    SDL_FRect rectB = b.getBoundingRect();
    
    float overlapLeft = std::max(rectA.x, rectB.x);
    float overlapRight = std::min(rectA.x + rectA.w, rectB.x + rectB.w);
    float overlapTop = std::max(rectA.y, rectB.y);
    float overlapBottom = std::min(rectA.y + rectA.h, rectB.y + rectB.h);
    
    SDL_Texture* texA = a.returnNativeSprite();
    SDL_Texture* texB = b.returnNativeSprite();
    
    // If textures are missing = treat as collision
    if (!texA || !texB)
        return true;
    
    float wA, hA, wB, hB;
    SDL_GetTextureSize(texA, &wA, &hA);
    SDL_GetTextureSize(texB, &wB, &hB);
    
    for (int y = static_cast<int>(overlapTop); y < static_cast<int>(overlapBottom); ++y)
    {
        for (int x = static_cast<int>(overlapLeft); x < static_cast<int>(overlapRight); ++x)
        {
            int localXA = static_cast<int>((x - rectA.x) / rectA.w * wA);
            int localYA = static_cast<int>((y - rectA.y) / rectA.h * hA);
            int localXB = static_cast<int>((x - rectB.x) / rectB.w * wB);
            int localYB = static_cast<int>((y - rectB.y) / rectB.h * hB);
            
            if (localXA >= 0 && localXA < static_cast<int>(wA) && localYA >= 0 && localYA < static_cast<int>(hA) &&
                localXB >= 0 && localXB < static_cast<int>(wB) && localYB >= 0 && localYB < static_cast<int>(hB))
            {
                // here assumes pixel hit without alpha
                return true;
            }
        }
    }
    
    return false;
}

void GameEngine::handlePhysicsCollision(MovingSprite& a, MovingSprite& b)
{
    // Elastic collision with masses and impulse
    if (!a.hasPhysics() || !b.hasPhysics())
        return;
    
    SDL_FRect rectA = a.getBoundingRect();
    SDL_FRect rectB = b.getBoundingRect();
    
    float centerXA = rectA.x + rectA.w / 2.0f;
    float centerYA = rectA.y + rectA.h / 2.0f;
    float centerXB = rectB.x + rectB.w / 2.0f;
    float centerYB = rectB.y + rectB.h / 2.0f;
    
    float dx = centerXB - centerXA;
    float dy = centerYB - centerYA;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance == 0.0f) distance = 0.1f;
    
    float minDist = (rectA.w + rectB.w) / 2.0f;
    float overlap = minDist - distance;
    
    if (overlap > 0)
    {
        dx /= distance;
        dy /= distance;
        
        float totalMass = a.getMass() + b.getMass();
        float moveA = overlap * (b.getMass() / totalMass);
        float moveB = overlap * (a.getMass() / totalMass);
        
        a.addPosition(-dx * moveA, -dy * moveA);
        b.addPosition(dx * moveB, dy * moveB);
        
        float relVelX = b.getXVelocity() - a.getXVelocity();
        float relVelY = b.getYVelocity() - a.getYVelocity();
        
        float velAlongNormal = relVelX * dx + relVelY * dy;
        
        if (velAlongNormal > 0)
            return;
        
        float restitution = std::min(a.getElasticity(), b.getElasticity());
        
        float impulse = -(1.0f + restitution) * velAlongNormal;
        impulse /= (1.0f / a.getMass() + 1.0f / b.getMass());
        
        float impulseX = impulse * dx;
        float impulseY = impulse * dy;
        
        a.addVelocity(-impulseX / a.getMass(), -impulseY / a.getMass());
        b.addVelocity(impulseX / b.getMass(), impulseY / b.getMass());
    }
}


void GameEngine::addScene(Scene* scene)
{
    // Add scene to collection
    scenes.push_back(scene);
}

void GameEngine::setActiveScene(const std::string& sceneName)
{
    // Set active scene via name
    for (auto scene : scenes)
    {
        if (scene->getName() == sceneName)
        {
            activeScene = scene;
            return;
        }
    }
}

Scene* GameEngine::getScene(const std::string& sceneName)
{
    // Look up scene via name
    for (auto scene : scenes)
    {
        if (scene->getName() == sceneName)
        {
            return scene;
        }
    }
    return nullptr;
}

GameEngine::~GameEngine()
{
    // Free sprites and scenes
    for(auto sprite : sprites)
    {
        delete sprite;
    }
    sprites.clear();
    
    for(auto scene : scenes)
    {
        delete scene;
    }
    scenes.clear();
}