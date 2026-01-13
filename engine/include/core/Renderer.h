#include <iostream>
#include "SDL3/SDL.h"

#ifndef RENDERER_H
#define RENDERER_H

class Sprite;
class Texture;
class Window;
class Renderer{


    public:
    Renderer();

    // Initializes renderer with a window
    Renderer(Window& p_gameWindow);

    // No copy constructor or assignment
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    

    // Gets the native SDL renderer
    SDL_Renderer* returnNativeRenderer() const;
    
    // Initializes renderer with a window
    void initializeRenderer(Window& gameWindow);

    // Sets current draw color (RGBA)
    void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // Clears backbuffer
    void clear();

    // Presents backbuffer to screen
    void present();

    
    int getWidth() const { return screenWidth; }
    int getHeight() const { return screenHeight; }

    ~Renderer();

    private: 

    SDL_Renderer* p_gameRenderer; // SDL renderer pointer
    
    
    int screenWidth;  // Cache: window width
    int screenHeight; // Cache: window height

    

};




#endif 