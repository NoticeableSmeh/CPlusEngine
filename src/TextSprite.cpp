#include "TextSprite.h"
#include "Renderer.h"
#include <iostream>

TextSprite::TextSprite(Renderer &ren, std::string fontPath, int fontSize,
                       std::string text, SDL_Color color, float x, float y)
    : Sprite()
{

    // Initialize position for text sprite
    this->xPosition = x;
    this->yPosition = y;

    this->sourceRect = nullptr;
    this->p_sprite = nullptr;

    this->text = text;
    this->color = color;

    // Open TTF font
    font = TTF_OpenFont(fontPath.c_str(), fontSize);

    if (!font)
    {
        SDL_Log("Could not load font '%s': %s", fontPath.c_str(), SDL_GetError());
    }

    // Create initial texture for the text
    updateTexture(ren);
}

TextSprite::~TextSprite()
{
    // Close font if opened
    if (font)
    {
        TTF_CloseFont(font);
    }
}

void TextSprite::setText(Renderer &ren, const std::string &newText)
{
    if (text != newText)
    {
        // Update text and build new texture
        text = newText;
        updateTexture(ren);
    }
}

void TextSprite::setColor(Renderer &ren, SDL_Color newColor)
{
    if (color.r != newColor.r || color.g != newColor.g || color.b != newColor.b || color.a != newColor.a)
    {
        // Change color and update texture
        color = newColor;
        updateTexture(ren);
    }
}

void TextSprite::updateTexture(Renderer &ren)
{
    if (!font)
        return;

    // Free previous texture if it exists
    if (this->p_sprite)
    {
        SDL_DestroyTexture(this->p_sprite);
        this->p_sprite = nullptr;
    }

    if (text.empty())
        return;

    // Render text to surface and create texture
    SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), 0, color);

    if (surface)
    {
        this->p_sprite = SDL_CreateTextureFromSurface(ren.returnNativeRenderer(), surface);

        // Update size based on rendered text
        this->width = static_cast<float>(surface->w);
        this->height = static_cast<float>(surface->h);

        SDL_DestroySurface(surface);
    }
    else
    {
        SDL_Log("Could not render text: %s", SDL_GetError());
    }
}