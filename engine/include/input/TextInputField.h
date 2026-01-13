#ifndef TEXTINPUTFIELD_H
#define TEXTINPUTFIELD_H

#include "TextSprite.h"
#include <string>

class TextInputField : public TextSprite
{
public:
    // Constructor and destructor
    TextInputField(Renderer& ren, const std::string& fontPath, int fontSize,
                   float x, float y, int maxLength = 20);
    
    TextInputField(const TextInputField&) = delete;
    TextInputField& operator=(const TextInputField&) = delete;
    
    ~TextInputField() override;
    

    // Handles text input
    void handleInput(SDL_Event& event);
    

    // Gets the current input text
    std::string getInputText() const { return inputText; }
    void clearInput();
    
    // Activates or deactivates the text input field
    void setActive(bool active);
    bool getActive() const { return isActive; }
    
    // Update and rendering methods
    void tick(float deltaTime) override;
    void render(Renderer& ren) override;
    
private:
    std::string inputText;
    int maxLength;
    bool isActive;
    bool showCursor;
    float cursorTimer;
    
    void updateDisplayText(Renderer& ren);
};

#endif