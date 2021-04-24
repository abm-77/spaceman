#pragma once

#include "maths.h"
#include <SDL2/SDL_ttf.h>


struct Image {
    SDL_Texture* texture;
    u32 width;
    u32 height;
};

struct Text {
    Image image;
    char text[1024]; 
};


void ImageFree(Image* image);
void ImageRender(SDL_Renderer* renderer, v2 position, Image* image); 
void TextInit(Text* text, const char* t, SDL_Renderer* renderer, TTF_Font* font,  v4 text_color);

void DrawSetColor(SDL_Renderer* rednerer, v4 color);
void DrawClear(SDL_Renderer* renderer, v4 color);
void DrawSetPixel(SDL_Renderer* renderer, v2 position, v4 color);

void DrawCircleOutline(SDL_Renderer* renderer, v2 center, i32 radius, v4 color);

void DrawCircleFilled(SDL_Renderer* renderer, v2 center, i32 radius, v4 color);

// TODO (bryson): Combine into polygon function?
void DrawTriangleOutline(SDL_Renderer* renderer, v2 points[], v4 color);
void DrawRectangleOutline (SDL_Renderer* renderer, v4 rect, v4 color);
void DrawRectangleOutlineEX (SDL_Renderer* renderer, v2 points[], v4 color);



//void DrawRectangleFilled  (SDL_Renderer* renderer, v2 points[], v4 color);