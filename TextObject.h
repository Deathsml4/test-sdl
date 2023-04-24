
#ifndef _TEXT_OBJECT_H_
#define _TEXT_OBJECT_H_

#include <iostream>
#include "game.h"
#include <SDL_ttf.h>

class TextObject
{
    public:
        //Initializes variables
        TextObject();

        //Deallocates memory
        ~TextObject();
        enum TextColor
        {
            Red_Text = 0,
            White_Text = 1,
            Black_Text = 2,
        };

        //Loads image at specified path
        bool loadFromFile( std::string path );

        bool loadFromRenderText(TTF_Font* font , SDL_Renderer* renderer);

        /*Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        */

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        void setColor( int type);

        /*Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );*/

        //Renders texture at given point
        void render( SDL_Renderer* renderer,
                    int x, int y,
                    SDL_Rect* clip = NULL,
                    double angle = 0.0,
                    SDL_Point* center = NULL,
                    SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Gets image dimensions
        int getWidth() const {return mWidth;}
        int getHeight() const {return mHeight ;}
        void setText (const std::string& text) { str_val = text; }
        std::string GetText () const {return str_val; }

    private:
        std::string str_val;
        SDL_Color text_color;
        //The actual hardware texture
        SDL_Texture* texture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

#endif
