#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "POC KET MON STA";

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(string path, SDL_Renderer* renderer);

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    SDL_Texture* background = loadTexture("Ko.Elizabeth.full.2947878.jpg", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    SDL_Texture* front = loadTexture("7_1_2023.png", renderer);
    SDL_RenderCopy(renderer, front, NULL, NULL);
    // Your drawing code here
    /*SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 122, 0, 0);
    SDL_RenderDrawLine(renderer, 100, 100, 200, 200);*/
    // use SDL_RenderPresent(renderer) to show it
    SDL_RenderPresent (renderer);

	waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture(string path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = nullptr;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str() );
    if ( loadedSurface == nullptr)
        cout << "Unable to load image " << path << " SDL_image Error: "
            << IMG_GetError() << endl;
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface);
        if (newTexture == nullptr)
        {
            cout << "Unable to create texture from " << path
                << "SDL Error: " << SDL_GetError() << endl;
        }

    // Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface);
    }
    return newTexture;
}
