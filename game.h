#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#ifndef __GAME_H__
#define __GAME_H__
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer);

#endif // __GAME_H__
