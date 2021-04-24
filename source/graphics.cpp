#include "graphics.h"

void DrawSetColor(SDL_Renderer* renderer, v4 color) {
    SDL_SetRenderDrawColor(renderer, (u8) color.r, (u8)color.g, (u8)color.b, (u8)color.a);
}

void DrawClear(SDL_Renderer* renderer, v4 color) {
    DrawSetColor(renderer, color);
    SDL_RenderClear(renderer);
}
void DrawSetPixel(SDL_Renderer* renderer, v2 position, v4 color) {
    DrawSetColor(renderer, color);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

void DrawCircleOutline(SDL_Renderer* renderer, v2 center, i32 radius, v4 color) {

    f64 error = (f64) -radius;

    f64 x = (f64)radius - 0.5;
    f64 y = 0.5;

    f64 cx = center.x - 0.5;
    f64 cy = center.y - 0.5;

    while (x >= y) {
        DrawSetPixel(renderer, v2((int)(cx + x), (int)(cy + y)), color );
        DrawSetPixel(renderer, v2((int)(cx + y), (int)(cy + x)), color );

        if (x != 0)
        {
            DrawSetPixel(renderer, v2((int)(cx - x), (int)(cy + y)), color);
            DrawSetPixel(renderer, v2((int)(cx + y), (int)(cy - x)), color);
        }

        if (y != 0)
        {
            DrawSetPixel(renderer, v2((int)(cx + x), (int)(cy - y)), color);
            DrawSetPixel(renderer, v2((int)(cx - y), (int)(cy + x)), color);
        }

        if (x != 0 && y != 0)
        {
            DrawSetPixel(renderer, v2((int)(cx - x), (int)(cy - y)), color);
            DrawSetPixel(renderer, v2((int)(cx - y), (int)(cy - x)), color);
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
}

void DrawCircleFilled(SDL_Renderer* renderer, v2 center, i32 radius, v4 color) {
    static const i32 BPP = 4;

    for (f64 dy = 1; dy <= radius; dy += 1.0)
    {
        f64 dx = Floor(SquareRoot((2.0 * radius * dy) - (dy * dy)));
        i32 x = center.x - dx;

        DrawSetColor(renderer, color);
        SDL_RenderDrawLine(renderer, center.x - dx, center.y + dy - radius, center.x + dx, center.y + dy - radius);
        SDL_RenderDrawLine(renderer, center.x - dx, center.y - dy + radius, center.x + dx, center.y - dy + radius);
    }
}

void DrawTriangleOutline(SDL_Renderer* renderer, v2 points[], v4 color) {
    DrawSetColor(renderer, color);

    SDL_Point p[4] = {
        {points[0].x, points[0].y},
        {points[1].x, points[1].y},
        {points[2].x, points[2].y},
        {points[0].x, points[0].y},
    };

    SDL_RenderDrawLines(renderer, p, 4);
}


void DrawRectangleOutline(SDL_Renderer* renderer, v4 rect, v4 color) {
    DrawSetColor(renderer, color);

    SDL_Rect r = {
        rect.x,
        rect.y,
        rect.width,
        rect.height
    };

    SDL_RenderDrawRect(renderer, &r);
}

void DrawRectangleOutlineEX(SDL_Renderer* renderer, v2 points[], v4 color) {
    DrawSetColor(renderer, color);

    SDL_Point p[5] = {
        {points[0].x, points[0].y},
        {points[1].x, points[1].y},
        {points[2].x, points[2].y},
        {points[3].x, points[3].y},
        {points[0].x, points[0].y},
    };

    SDL_RenderDrawLines(renderer, p, 4);
}

void DrawRectangleFilled(SDL_Renderer* renderer, v4 rect, v4 color) {
    DrawSetColor(renderer, color);

    SDL_Rect r = {
        rect.x,
        rect.y,
        rect.width,
        rect.height
    };

    SDL_RenderFillRect(renderer, &r);
}

void TextInit(Text* text, const char* t, SDL_Renderer* renderer, TTF_Font* font,  v4 text_color) {
    // Free image
    ImageFree(&text->image);

    // Clear text
    MemorySet(text->text, 0, sizeof(text->text));

    strcpy(text->text, t);

    // Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, t, 
    (SDL_Color) {
        text_color.r, 
        text_color.g, 
        text_color.b, 
        text_color.a
    });

    if(text_surface == NULL)
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        text->image.texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if( text->image.texture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            text->image.width = text_surface->w;
            text->image.height = text_surface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( text_surface );
    }
}

void ImageRender(SDL_Renderer* renderer, v2 position, Image* image) {
    SDL_Rect renderQuad = {position.x, position.y, image->width, image->height};
    SDL_RenderCopy(renderer, image->texture, NULL, &renderQuad);
}

void ImageFree (Image* image) {
    //Free texture if it exists
    if(image->texture  != NULL )
    {
        SDL_DestroyTexture( image->texture );
        image->texture = NULL;
        image->width = 0;
        image->height = 0;
    }

}