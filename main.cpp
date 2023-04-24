#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <ctime>
#include "game.h"
#include "TextObject.h"

using namespace std;

const int SCREEN_WIDTH = 1400;
const int SCREEN_HEIGHT = 800;

/*int step = 20;*/
const int x_central = SCREEN_WIDTH/2.0;
const int y_central = SCREEN_HEIGHT/2.0;

TTF_Font* font = NULL;





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
struct point {
    int x_,y_;
    Uint32 time_point;
    void get_point(){
        x_ = rand()%SCREEN_WIDTH;
        y_ = rand()%SCREEN_HEIGHT;
        time_point = SDL_GetTicks();
    }
    int size = 15;
    void render (SDL_Renderer* renderer, SDL_Rect* prect){
        SDL_Rect rectpoint;
        rectpoint.x = x_;
        rectpoint.y = y_;
        rectpoint.w = size;
        rectpoint.h = size;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1);
        SDL_RenderFillRect(renderer,&rectpoint);
        if (eaten(prect)){
            cerr << "Score : " << score << endl;
            get_point();
        }
        else if (SDL_GetTicks() - time_point >= 10000){
            get_point();
        }
    }
    bool eaten (SDL_Rect* prect){
        int x_check = x_ + size/2;
        int y_check = y_ + size/2;
        if ((x_check >= prect->x && x_check <= prect->x + prect->w) && (y_check >= prect->y && y_check <= prect->y + prect->h)){
            score ++;
            return 1;
        }
        return 0;
    }
    int score = 0;
};

struct Laser{
    int size = 3;
    int x[6],y[6];
    bool gameOver = 0;
    void init (){
         x[0] = - x_central ; y[0] = -y_central; // Tâm màn hình tại [0,0]
         x[1] = - SCREEN_HEIGHT / 2.0 ; y[1] = - x_central;
         x[2] = 0 ; y[2] = - x_central;
         x[3] = SCREEN_HEIGHT / 2.0 ; y[3] = - x_central;
         x[4] = x_central ; y[4] = - x_central;
         x[5] = x_central ; y[5] = 0;
    }
    int side = 1;
    void roll(SDL_Rect* prect){
        for (int i = 0 ; i < 6 ; i++){
            int new_x = x[i]*0.9999 - y[i]*0.0141*side;
            int new_y = x[i]*0.0141*side + y[i]*0.9999;
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
            if (touched(prect,x[i],y[i])) gameOver = 1;
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


    void render (SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer,255,255,120,1);
        for (int i = 0 ; i < 6 ; i++){
            SDL_RenderDrawLine(renderer, x[i]+x_central, y[i]+y_central, x_central-x[i], y_central-y[i]);
        }

    }
};
void menu (SDL_Renderer* renderer){

}


/*void moveUp (SDL_Rect* prect){
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
}*/

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);





    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_Texture* GameOver = loadTexture("GameOver.png", renderer);

    SDL_Texture* background = loadTexture("Background 1.png", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);


    SDL_Texture* pic = loadTexture("slime.png", renderer);
    SDL_Rect rect;

    SDL_QueryTexture (pic, NULL, NULL, &rect.w, &rect.h);
    SDL_QueryTexture (pic, NULL, NULL, &rect.w, &rect.h);
    rect.w /= 11.0;
    rect.h /= 11.0;
    rect.x = x_central;
    rect.y = y_central;
    SDL_RenderCopy(renderer, pic, NULL, &rect);

    SDL_RenderPresent(renderer);

    if (TTF_Init() < 0)
    {
    SDL_Log("%s", TTF_GetError());
        return -1;
    }
    font = TTF_OpenFont("Font//Calibri Light.ttf",20);
    TextObject score_text;
    score_text.setColor(TextObject::White_Text);
    TextObject red_text;
    red_text.setColor(TextObject::Red_Text);
    TextObject black_text;
    black_text.setColor(TextObject::Black_Text);



    Laser laser;
    laser.init();
    point Point;
    Point.get_point();

    Uint32 time = SDL_GetTicks();

    std::string str_switch = " SWITCH ";

    bool quit = false;
    SDL_Event event;
    bool game1 = 0;
    while (!quit){
        if (SDL_PollEvent(&event) == 0) {
            continue;
        }
        else if (event.type == SDL_QUIT) break;
        else if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym){
                case SDLK_ESCAPE: quit = true; break;
                case SDLK_SPACE: game1 = 1; break;
                default : break;
            }
        }
        else if (event.type == SDL_MOUSEMOTION){
            int x_mouse,y_mouse;
            SDL_GetMouseState (&x_mouse,&y_mouse);
            rect.x = x_mouse - rect.w/2;
            rect.y = y_mouse - rect.h/2;
        }
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderCopy(renderer, pic, NULL, &rect);
        SDL_RenderPresent(renderer);
        laser.render(renderer);
        SDL_RenderPresent(renderer);
        while (game1){

            if (SDL_GetTicks() - time  >= 30000){
                laser.side *= -1;
                time = SDL_GetTicks();
            }

            laser.roll(&rect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            SDL_RenderCopy(renderer, background, NULL, NULL);
            SDL_RenderCopy(renderer, pic, NULL, &rect);


            laser.render(renderer);
            Point.render(renderer,&rect);
            if (SDL_GetTicks() - time  >= 28000){
                red_text.setText(str_switch);
                red_text.loadFromRenderText(font, renderer);
                red_text.render(renderer,SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT - 200);
                SDL_RenderPresent(renderer);
                //SDL_Delay(50);

            }

            //Show score
            std::string str_score = "Score: ";
            std::string str_val = std::to_string(Point.score);
            str_score += str_val;

            score_text.setText(str_score);
            score_text.loadFromRenderText(font, renderer);
            score_text.render(renderer,SCREEN_WIDTH - 200, 15);

            SDL_RenderPresent(renderer);

            if (SDL_PollEvent(&event) == 0) {
                SDL_Delay(50);
                continue;
            }
            else if (event.type == SDL_QUIT) break;
            else if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE: quit = 1; game1 = 0 ; break;
                    default : break;
                }
            }
            else if (event.type == SDL_MOUSEMOTION){
                int x_mouse,y_mouse;
                SDL_GetMouseState (&x_mouse,&y_mouse);
                rect.x = x_mouse - rect.w/2;
                rect.y = y_mouse - rect.h/2;
            }
            if (laser.gameOver) {
                SDL_RenderCopy(renderer, GameOver, NULL, NULL);

                font = TTF_OpenFont("Font//Calibri Light.ttf",60);
                score_text.loadFromRenderText(font, renderer);
                score_text.render(renderer,SCREEN_WIDTH/2 -100, SCREEN_HEIGHT -200);
                quit = 1;

                SDL_RenderPresent(renderer);
                quit = 1;
                game1 = 0;
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
    TTF_Quit();
    return 0;
}


