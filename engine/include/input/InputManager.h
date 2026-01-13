#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL3/SDL.h>             
#include <SDL3/SDL_main.h>
#include <vector>

// Enum for keys and mouse buttons
enum class Key{

    Left, Right, Up, Down, Space, Enter, Escape, Backspace, Delete,
    A, B, C, D, E, F, G, H, I, J, K, L,
    M, N, O, P, Q, R, S, T, U, V, W, X,
    Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Tab, Shift, Ctrl, Alt
};

enum class MouseButton{

    Left = 1, Middle = 2, Right = 3
};

class InputManager{

public:

    // Update current input status 
    static void update();
    // Batch update from given event vector
    static void update(const std::vector<SDL_Event>& events);

    // Keyboard handling
    static bool getKey(Key key);
    static bool getKeyDown(Key key);
    static bool getKeyUp(Key key);

    // Mouse button handling
    static bool getMouseButton(MouseButton button);
    static bool getMouseButtonDown(MouseButton button);
    static bool getMouseButtonUp(MouseButton button);

    // Current mouse position
    static int getMouseX();
    static int getMouseY();

    // Flag to quit program (QUIT)
    static bool shouldQuit();

private:

    // Private constructor to prevent instantiation
    InputManager() = delete;

    // Keyboard status: current and previous
    static bool currentKeyState[static_cast<int>(Key::Alt) + 1];
    static bool previousKeyState[static_cast<int>(Key::Alt) + 1];

    // Mouse button status: current and previous
    static bool currentMouseState[4];
    static bool previousMouseState[4];

    // Mapping from own Key to SDL_Keycode
    static SDL_Keycode keyToSDL_Key(Key key);

    // Last read mouse position
    static int mouseX;
    static int mouseY;

    // Internal quit flag
    static bool quit;

};

#endif