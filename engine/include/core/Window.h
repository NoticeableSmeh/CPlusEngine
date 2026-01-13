#include <SDL3/SDL.h>
#include <iostream>

#ifndef WINDOW_H
#define WINDOW_H


class Renderer;
class Window{

    public:
    // Constructors
    Window();
    Window(std::string title, const int width, const int height);

    // No copy constructor or assignment 
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Initializes SDL window
    SDL_Window* initializeWindow(const std::string title, const int width, const int height);
    // Get SDL_Window*
    SDL_Window* returnNativeWindow() const;


    ~Window();
    
    private:
    
    // Pointer to SDL window
    SDL_Window* p_gameWindow;
    
    // Internal update of title/size
    void updateWindow();


    
};

#endif