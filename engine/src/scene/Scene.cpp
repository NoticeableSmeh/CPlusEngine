#include "Scene.h"
#include "Renderer.h"
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <iostream>

Scene::Scene(const std::string& name)
    : sceneName(name), backgroundPath(""), backgroundTexture(nullptr) 
{
    // Create empty scene with name; no background from start
}

Scene::~Scene()
{
    // Free allocated sprites
    for(auto sprite : sprites)
    {
        delete sprite;
    }
    sprites.clear();

    // Free background texture if set
    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
    }

}

void Scene::addSprite(Sprite* sprite)
{
    // Add sprite to scene's list
    sprites.push_back(sprite);
}

void Scene::removeSprite(Sprite* sprite)
{
    // Remove first occurrence of sprite
    auto it = std::find(sprites.begin(), sprites.end(), sprite);
    if (it != sprites.end())
    {
        sprites.erase(it);
    }
}

void Scene::update(float deltaTime)
{
    // Update all sprites in the scene
    for (auto sprite : sprites)
    {
        sprite->tick(deltaTime);
    }
}

void Scene::render(Renderer& ren)
{
    // Draw background over entire window if it exists
    if (backgroundTexture)
    {
        SDL_FRect destRect = {
            0, 
            0, 
            static_cast<float>(ren.getWidth()), 
            static_cast<float>(ren.getHeight())
        };
        SDL_RenderTexture(ren.returnNativeRenderer(), backgroundTexture, nullptr, &destRect);
    }
    
    // Draw all sprites in the scene
    for (auto sprite : sprites)
    {
        sprite->render(ren);
    }
}

void Scene::setBackground(const std::string& backgroundPath, Renderer& ren)
{
    this->backgroundPath = backgroundPath;
    
    // Free old background if it existed
    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
    }
    
    // Load new background texture
    backgroundTexture = IMG_LoadTexture(ren.returnNativeRenderer(), backgroundPath.c_str());
    
    if (!backgroundTexture)
    {
        std::cout << "Failed to load background: " << backgroundPath << std::endl;
    }
}