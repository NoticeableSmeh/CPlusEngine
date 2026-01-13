#include "Window.h"
#include <iostream>

Window::Window(std::string title, const int width, const int height)
{
    // Create window directly at construction
    p_gameWindow = initializeWindow(title, width, height);
}

Window::Window()
{
    
}


Window::~Window()
{
    // Close and release SDL_Window
    SDL_DestroyWindow(p_gameWindow);
}

SDL_Window* Window::initializeWindow(const std::string title, const int width, const int height)
{
    // Create window; if fails, log error
    if(p_gameWindow = SDL_CreateWindow(title.c_str(), width, height, 0); p_gameWindow == nullptr)   
    {         
        SDL_Log("Window could not be created! %s", SDL_GetError());    
        
    }

    return p_gameWindow; 
}




SDL_Window* Window::returnNativeWindow() const
{
    // Give access to SDL_Window*
    return p_gameWindow;
}

