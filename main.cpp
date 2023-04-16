#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "game.h"

using namespace std;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;

int step = 20;
const int x_central = SCREEN_WIDTH/2.0;
const int y_central = SCREEN_HEIGHT/2.0;

/*enum Action{
    LEFT, RIGHT, UP, DOWN, NONE, QUIT, PAUSE
};

Action getUserAction() {
    SDL_Event e;
    if (SDL_PollEvent(&e) == 0) return NONE;
    if (e.type == SDL_QUIT) return QUIT;
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_ESCAPE: return QUIT;
            case SDLK_LEFT: return LEFT;
            case SDLK_RIGHT: return RIGHT;
            case SDLK_DOWN: return DOWN;
            case SDLK_UP: return UP;
            case SDLK_PAUSE: return PAUSE;
            default: return NONE;
        }
    }
    return NONE;
}*/

struct Laser{
    int size = 3;
    int x[6],y[6];
    int touched_count = 0;
    void init (){
         x[0] = - x_central ; y[0] = -y_central; // Tâm màn hình tại [0,0]
         x[1] = - SCREEN_HEIGHT / 2.0 ; y[1] = - y_central;
         x[2] = 0 ; y[2] = - y_central;
         x[3] = SCREEN_HEIGHT / 2.0 ; y[3] = - y_central;
         x[4] = x_central ; y[4] = - y_central;
         x[5] = x_central ; y[5] = 0;
    }

    void roll(SDL_Rect* prect){
        for (int i = 0 ; i < 6 ; i++){
            int new_x = x[i]*0.9999 - y[i]*0.0141;
            int new_y = x[i]*0.0141 + y[i]*0.9999;
            if (new_x >= 0){
                if (new_y >= 0){
                    new_x ++;
                    new_y ++;
                }
                else {
                    new_x ++;
                    new_y --;
                }
            }
            else{
                if (new_y >= 0){
                    new_x --;
                    new_y ++;
                }
                else {
                    new_x --;
                    new_y --;
                }
            }
            x[i] = new_x;
            y[i] = new_y;
            if (touched(prect,x[i],y[i])) gameOver();
        }
    }

    bool touched (SDL_Rect* prect,int x_, int y_){
        int x1_rect = prect->x - x_central;
        int x2_rect = prect->x + prect->w - x_central;
        int y1_rect = prect->y - y_central;
        int y2_rect = prect->y + prect->h - y_central;
        if (x_ == 0){
            if (x1_rect * x2_rect <= 0) return 1;
        }
        else {
            float k = 1.0 * y_/x_;
            if (((y1_rect - k*x1_rect)*(y2_rect - k*x2_rect) <= 0) ||
                ((y1_rect - k*x2_rect)*(y2_rect - k*x1_rect) <= 0))
                    return 1;
        }
        return 0;
    }

    void gameOver (){
        cerr << touched_count++ << endl;
    }

    void render (SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer,255,255,120,1);
        for (int i = 0 ; i < 6 ; i++){
            SDL_RenderDrawLine(renderer, x[i]+x_central, y[i]+y_central, x_central-x[i], y_central-y[i]);
        }

    }
};
void menu (SDL_Renderer* renderer){

}


void moveUp (SDL_Rect* prect){
    if (prect->y > step) prect->y -= step;
}
void moveDown (SDL_Rect* prect){
    if (prect->y + prect->h < SCREEN_HEIGHT - step) prect->y += step;
}
void moveLeft (SDL_Rect* prect){
    if (prect->x > step) prect->x -= step;
}
void moveRight (SDL_Rect* prect){
    if (prect->x + prect->w < SCREEN_WIDTH - step) prect->x += step;
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);



    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* background = loadTexture("Background 1.png", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);


    SDL_Texture* pic = loadTexture("ruet.png", renderer);
    SDL_Rect rect;

    SDL_QueryTexture (pic, NULL, NULL, &rect.w, &rect.h);
    rect.w /= 15.0;
    rect.h /= 15.0;
    rect.x = x_central;
    rect.y = y_central;
    SDL_RenderCopy(renderer, pic, NULL, &rect);

    SDL_RenderPresent(renderer);

    Laser laser;
    laser.init();

    bool quit = false;
    SDL_Event event;
    while (!quit){
        if (SDL_PollEvent(&event) == 0) {
            continue;
        }
        else if (event.type == SDL_QUIT) break;
        else if (event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE: quit = true; break;
                case SDLK_UP: moveUp(&rect);break;
                case SDLK_DOWN: moveDown(&rect);break;
                case SDLK_LEFT: moveLeft(&rect);break;
                case SDLK_RIGHT: moveRight(&rect);break;
                default: break;
            }
        }
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderCopy(renderer, pic, NULL, &rect);
        SDL_RenderPresent(renderer);
        while (!quit){
            laser.roll(&rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            SDL_RenderCopy(renderer, background, NULL, NULL);
            SDL_RenderCopy(renderer, pic, NULL, &rect);


            laser.render(renderer);
            SDL_RenderPresent(renderer);

            if (SDL_PollEvent(&event) == 0) {
                SDL_Delay(100);
                continue;
            }
            else if (event.type == SDL_QUIT) break;
            else if (event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE: quit = true; break;
                    case SDLK_UP: moveUp(&rect);break;
                    case SDLK_DOWN: moveDown(&rect);break;
                    case SDLK_LEFT: moveLeft(&rect);break;
                    case SDLK_RIGHT: moveRight(&rect);break;
                    default: break;
                }
            }
        }






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


