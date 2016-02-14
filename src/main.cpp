#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

int main ()
{
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Creating window
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Creating renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Loading image "hello.bmp"
    SDL_Surface *bmp = SDL_LoadBMP("img/hello.bmp");
    if (bmp == nullptr) {
        cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Loading texture to renderer
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    // Cleaninig memory from needless image.
    SDL_FreeSurface(bmp);
    if (tex == nullptr) {
        cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
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
