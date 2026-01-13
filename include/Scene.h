#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Sprite.h"

class Renderer;

class Scene
{
public:
    Scene(const std::string& name);
    ~Scene();

    // No copy constructor or assignment
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    

    // Manage sprites in the scene
    void addSprite(Sprite* sprite);
    void removeSprite(Sprite* sprite);
    
    // Updates all sprites in the scene
    void update(float deltaTime);
    
    // Render the scene
    void render(Renderer& ren);
    

    // Sets screen size for the scene
    void setScreenSize(int width, int height);  
    

    // Sets background image for the scene
    void setBackground(const std::string& backgroundPath, Renderer& ren);
    

    // Gets the scene's name
    std::string getName() const { return sceneName; }
    
    // Gets all sprites in the scene
    std::vector<Sprite*>& getSprites() { return sprites; }
    
private:
    std::string sceneName;
    std::vector<Sprite*> sprites;
    std::string backgroundPath;
    SDL_Texture* backgroundTexture; // Background texture if set

    int screenWidth;
    int screenHeight;
};

#endif