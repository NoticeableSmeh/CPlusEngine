#include "Renderer.h"
#include "Window.h"
#include "Sprite.h"


Renderer::Renderer(Window& p_gameWindow)
    : screenWidth(0), screenHeight(0)
{
    // Create SDL_Renderer linked to the window
    p_gameRenderer = SDL_CreateRenderer(p_gameWindow.returnNativeWindow(), NULL);

    if(!p_gameRenderer){
        std::cerr << "Error failed to initialize renderer: " << SDL_GetError() << std::endl;
    }
    
    // Get window size for caching
    SDL_GetWindowSize(p_gameWindow.returnNativeWindow(), &screenWidth, &screenHeight);
}

Renderer::Renderer()
    : p_gameRenderer(nullptr), screenWidth(0), screenHeight(0)
{

}

void Renderer::initializeRenderer(Window& gameWindow)
{
    // Initialize renderer afterwards (if window created later)
    p_gameRenderer = SDL_CreateRenderer(gameWindow.returnNativeWindow(), NULL);

    if(!p_gameRenderer){
        std::cerr << "Error failed to initialize renderer: " << SDL_GetError() << std::endl;
    }
    
    
    // Update cached screen size
    SDL_GetWindowSize(gameWindow.returnNativeWindow(), &screenWidth, &screenHeight);
}

SDL_Renderer* Renderer::returnNativeRenderer() const
{
    return p_gameRenderer;  
}

Renderer::~Renderer(){
    // Release renderer resources
    SDL_DestroyRenderer(p_gameRenderer);
}   


void Renderer::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Set draw color for subsequent draw calls
    SDL_SetRenderDrawColor(p_gameRenderer, r, g, b, a);
}


void Renderer::clear()
{
    // Clear backbuffer with current draw color
    SDL_RenderClear(p_gameRenderer);
}

void Renderer::present()
{
    // Present backbuffer to screen
    SDL_RenderPresent(p_gameRenderer);
}



