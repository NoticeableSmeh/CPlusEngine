#include "InputManager.h"
#include <iostream>

// Initialize mouse position
int InputManager::mouseX{0};
int InputManager::mouseY{0};

// Global quit flag
bool InputManager::quit{false};

// Key status arrays
bool InputManager::currentKeyState[static_cast<int>(Key::Alt) + 1] = {false};
bool InputManager::previousKeyState[static_cast<int>(Key::Alt) + 1] = {false};

// Mouse button status arrays 
bool InputManager::currentMouseState[4] = {false};
bool InputManager::previousMouseState[4] = {false};

SDL_Keycode InputManager::keyToSDL_Key(Key key){

    switch(key){
    
        case Key::Left:
            return SDLK_LEFT;
        case Key::Right:
            return SDLK_RIGHT;
        case Key::Up:
            return SDLK_UP;
        case Key::Down:
            return SDLK_DOWN;
        case Key::Space:
            return SDLK_SPACE;
        case Key::Enter:
            return SDLK_RETURN;
        case Key::Escape:
            return SDLK_ESCAPE;
        case Key::Delete:
            return SDLK_DELETE;
        case Key::A:
            return SDLK_A;
        case Key::B:
            return SDLK_B;
        case Key::C:
            return SDLK_C;
        case Key::D:
            return SDLK_D;
        case Key::E:         
            return SDLK_E;
        case Key::F:         
            return SDLK_F;
        case Key::G:         
            return SDLK_G;
        case Key::H:         
            return SDLK_H;
        case Key::I:         
            return SDLK_I;
        case Key::J:         
            return SDLK_J;
        case Key::K:         
            return SDLK_K;
        case Key::L:         
            return SDLK_L;
        case Key::M:         
            return SDLK_M;
        case Key::N:         
            return SDLK_N;
        case Key::O:         
            return SDLK_O;
        case Key::P:         
            return SDLK_P;
        case Key::Q:         
            return SDLK_Q;
        case Key::R:         
            return SDLK_R;
        case Key::S:         
            return SDLK_S;
        case Key::T:         
            return SDLK_T;
        case Key::U:         
            return SDLK_U;
        case Key::V:         
            return SDLK_V;
        case Key::W:         
            return SDLK_W;
        case Key::X:         
            return SDLK_X;
        case Key::Y:         
            return SDLK_Y;
        case Key::Z:         
            return SDLK_Z;
        case Key::Num0:      
            return SDLK_0;
        case Key::Num1:      
            return SDLK_1;
        case Key::Num2:      
            return SDLK_2;
        case Key::Num3:      
            return SDLK_3;
        case Key::Num4:      
            return SDLK_4;
        case Key::Num5:      
            return SDLK_5;
        case Key::Num6:      
            return SDLK_6;
        case Key::Num7:      
            return SDLK_7;
        case Key::Num8:      
            return SDLK_8;
        case Key::Num9:      
            return SDLK_9;
        default:            
            return SDLK_UNKNOWN;
    }//Switch case

}

void InputManager::update(){
    // No longer used
}

void InputManager::update(const std::vector<SDL_Event>& events){

    for (int i = 0; i < static_cast<int>(Key::Alt); i++)
    {
        previousKeyState[i] = currentKeyState[i];
    }
    
    for (int i = 0; i < 4; i++)
    {
        previousMouseState[i] = currentMouseState[i];
    }

    for (const auto& e : events){
        SDL_Keycode sdlKey;

        switch(e.type){

            case SDL_EVENT_QUIT:
                quit = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                sdlKey = e.key.key;

                // You can cast an enum entry to int to get its index
                for (int i = 0; i < (static_cast<int>(Key::Alt) + 1); i++){

                    Key engineKey = static_cast<Key>(i);
                    if (keyToSDL_Key(engineKey) == sdlKey){
                        currentKeyState[i] = true;
                        break;
                    }
                }

                break;

            case SDL_EVENT_KEY_UP:
                sdlKey = e.key.key;

                for (int i = 0; i < (static_cast<int>(Key::Alt) + 1); i++){

                    Key engineKey = static_cast<Key>(i);
                    if (keyToSDL_Key(engineKey) == sdlKey){

                        currentKeyState[i] = false;
                        break;
                    }
                }

                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (e.button.button >= 1 && e.button.button <= 3)

                    currentMouseState[e.button.button] = true;
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_UP:
                if (e.button.button >= 1 && e.button.button <= 3)

                    currentMouseState[e.button.button] = false;
                break;
                
            case SDL_EVENT_MOUSE_MOTION:

                mouseX = static_cast<int>(e.motion.x);
                mouseY = static_cast<int>(e.motion.y);
                break;
        }

    }
    

}

bool InputManager::getKey(Key key)
{
    int index = static_cast<int>(key);
    return currentKeyState[index];
}

bool InputManager::getKeyDown(Key key)
{
    int index = static_cast<int>(key);
    return currentKeyState[index] && !previousKeyState[index];
}

bool InputManager::getKeyUp(Key key)
{
    int index = static_cast<int>(key);
    return !currentKeyState[index] && previousKeyState[index];
}

bool InputManager::getMouseButton(MouseButton button)
{
    int index = static_cast<int>(button);
    if (index >= 1 && index <= 3)
        return currentMouseState[index];
    return false;
}

bool InputManager::getMouseButtonDown(MouseButton button)
{
    int index = static_cast<int>(button);
    if (index >= 1 && index <= 3)
        return currentMouseState[index] && !previousMouseState[index];
    return false;
}

bool InputManager::getMouseButtonUp(MouseButton button)
{
    int index = static_cast<int>(button);
    if (index >= 1 && index <= 3)
        return !currentMouseState[index] && previousMouseState[index];
    return false;
}

int InputManager::getMouseX()
{
    return mouseX;
}

int InputManager::getMouseY()
{
    return mouseY;
}

bool InputManager::shouldQuit()
{
    return quit;
}