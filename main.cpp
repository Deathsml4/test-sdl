#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

using namespace std;

int step = 5;



int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    /*SDL_Texture* background = loadTexture("Background 1.png", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);*/

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* pic = loadTexture("ruet.png", renderer);
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    SDL_QueryTexture (pic, NULL, NULL, &rect.w, &rect.h);
    rect.w /= 10.0;
    rect.h /= 10.0;

    SDL_Event event;
    while (true){
        if (SDL_WaitEvent(&event) == 0) continue;
        else if (event.type == SDL_QUIT) break;
        else if (event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE: break;
                case SDLK_UP: rect.y -= step;break;
                case SDLK_DOWN: rect.y += step;break;
                case SDLK_LEFT: rect.x -= step;break;
                case SDLK_RIGHT: rect.x += step;break;
                default: break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, pic, NULL, &rect);
        SDL_RenderPresent(renderer);

    }
    SDL_RenderPresent (renderer);
    waitUntilKeyPressed();
    SDL_Delay(100);
    // Your drawing code here

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // use SDL_RenderPresent(renderer) to show it
    SDL_RenderPresent (renderer);

	waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}


