#include "game.h"
#include "TextObject.h"
#include <SDL_ttf.h>
#include <string>

TextObject::TextObject()
{
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;
    texture = NULL;
}

TextObject::~TextObject()
{

}

bool TextObject::loadFromRenderText(TTF_Font* font, SDL_Renderer* renderer)
{
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if (text_surface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        mWidth = text_surface->w;
        mHeight = text_surface->h;
        SDL_FreeSurface(text_surface);
    }

    return texture != NULL;
}

void TextObject::free()
{
if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void TextObject::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}

void TextObject::setColor(int type)
{
    if (type == Red_Text)
    {
        SDL_Color color = {255,0,0};
        text_color = color;
    }
    else if(type == White_Text)
    {
        SDL_Color color = {255,255,255};
        text_color = color;
    }
    else if(type == Black_Text)
    {
        SDL_Color color = {0,0,0};
        text_color = color;
    }
}

void TextObject::render( SDL_Renderer* renderer,
                    int x, int y,
                    SDL_Rect* clip /*= NULL*/,
                    double angle /*= 0.0*/,
                    SDL_Point* center /*= NULL*/,
                    SDL_RendererFlip flip /*= SDL_FLIP_NONE*/ )
{
    SDL_Rect renderQuad = {x,y,mWidth,mHeight};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}
