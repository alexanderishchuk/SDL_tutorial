#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// SDL Error Logger
void logSDLError (ostream &os, const string &msg) {
    os << msg << " error: " << SDL_GetError() << endl;
}

int main () {
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(cout, "SDL_Init");
        return 1;
    }

    // Creating window
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        logSDLError(cout, "SDL_CreateWindow");
        return 1;
    }

    // Creating renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        logSDLError(cout, "SDL_CreateRenderer");
        return 1;
    }

    // Loading image "hello.bmp"
    SDL_Surface *bmp = SDL_LoadBMP("img/hello.bmp");
    if (bmp == nullptr) {
        logSDLError(cout, "SDL_LoadBMP");
        return 1;
    }

    // Loading texture to renderer
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    // Cleaninig memory from needless image.
    SDL_FreeSurface(bmp);
    if (tex == nullptr) {
        logSDLError(cout, "SDL_CreateTextureFromSurface");
        return 1;
    }

    // Drawing texture
    // Cleaning renderer
    SDL_RenderClear(ren);
    // Rendering texture
    SDL_RenderCopy(ren, tex, NULL, NULL);
    // Show refreshed screen
    SDL_RenderPresent(ren);

    SDL_Delay(2000);

    // Cleaning objects
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
