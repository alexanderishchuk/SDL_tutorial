#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "cleanup.h"
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

/******************************************************************************/

// SDL Error Logger
void logSDLError (ostream &os, const string &msg) {
    os << msg << " error: " << SDL_GetError() << endl;
}


/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture (const string &file, SDL_Renderer *ren) {
/*  OLd code
    //Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = nullptr;
    //Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    //If the loading went ok, convert to texture and return the texture
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Make sure converting went ok too
        if (texture == nullptr) {
            logSDLError(cout, "SDL_CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(cout, "SDL_LoadBMP");
    }
    return texture;
    */
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr) {
        logSDLError(cout, "IMG_LoadTexture");
    }

    return texture;
}


/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}


/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

/******************************************************************************/

int main () {
    // Initializing SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(cout, "SDL_Init");
        return 1;
    }

    // Creating window
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr) {
        logSDLError(cout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }

    // Creating renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        logSDLError(cout, "SDL_CreateRenderer");
        cleanup(win);
        SDL_Quit();
        return 1;
    }

    // Loading image "hello.bmp"
    SDL_Texture *background = loadTexture("img/background.png", ren);
    SDL_Texture *image = loadTexture("img/image.png", ren);
    if (background == nullptr || image == nullptr) {
        cleanup(background, image, win, ren);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Drawing texture
    // Cleaning renderer
    SDL_RenderClear(ren);

    // Rendering background
/*  int bW, bH;
    SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
    renderTexture(background, ren, 0, 0);
    renderTexture(background, ren, bW, 0);
    renderTexture(background, ren, bW, bH);
    renderTexture(background, ren, 0, bH); */
    // Determine how many times we'll ned to fill the screen
    int xTiles = SCREEN_WIDTH / TILE_SIZE;
    int yTiles = SCREEN_HEIGHT / TILE_SIZE;
    // Draw tiles by calculating their position
    for (int i = 0; i < xTiles * yTiles; ++i) {
        int x = i % xTiles;
        int y = i / xTiles;
        renderTexture(background, ren, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
            TILE_SIZE);
    }

    // Rendering foreground image
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, ren, x, y);
    // Show refreshed screen
    SDL_RenderPresent(ren);

    SDL_Delay(10000);

    // Cleaning objects
    cleanup(background, image, ren, win);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
