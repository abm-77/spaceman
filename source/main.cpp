#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "language_layer.h"
#include "program_options.h"
#include "maths.h"
#include "memory.h"
#include "entity.h"
#include "graphics.h"
#include "timer.h"
#include "collision.h"

struct Application {
    SDL_Window* window;
    SDL_Renderer* renderer;
    b32 running;
};

struct Input {
    b32 up;
    b32 down;
    b32 left;
    b32 right;
    b32 shoot;
};

global Application game;

global Input input;
global const u8* keyboard;
global Timer spawn_timer;
global f32 spawn_delay = 0;

global M_Arena g_entities;

global Entity* player;
global Timer shoot_delay_timer;
global f32 shoot_delay = 0.15f;

global std::vector<Entity*> active_bullets;

global TTF_Font* font = NULL;
global Text score_text;
global u32 score = 0;


b32 Init (void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not intialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    i32 img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) { 
        printf ("SDL_image could not initializ! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    if( TTF_Init() == -1 ) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }

    game.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (game.window == NULL) {
        printf("SDL Window could not be created! SDL Error %s \n", SDL_GetError());
        return false;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    if (game.renderer == NULL) {
        printf("SDL Renderer could not be created! SDL Error %s \n", SDL_GetError());
        return false;
    }

    SeedRandomNumberGeneratorWithTime();

    game.running = true;
    TimerStart(&spawn_timer);

    keyboard = SDL_GetKeyboardState(NULL);

    // DEMO Part 1: Creating
    // Allocate enough space for double the max amount of entities
    ArenaCreate(&g_entities, 2 * (MAX_ENTITY_COUNT * sizeof(Entity)));
    
    for (int i = 0; i < MAX_ENTITY_COUNT; i++) 
        Entity* e = EntityCreate(&g_entities);

    player = EntitySpawn();
    player->type = ENTITY_PLAYER; 
    player->shape = SHAPE_TRIANGLE;
    EntitySetPosition(player, v2(DEFAULT_WINDOW_WIDTH/2, DEFAULT_WINDOW_HEIGHT/2));
    player->color = v4(0, 255, 0, 255);
    player->half_width = 30;
    player->velocity = v2(0, 0);
    player->speed = 250;
    EntitySetRotation(player, 0);
    TimerStart(&shoot_delay_timer);

    font = TTF_OpenFont("resources/SpaceMono.ttf", 28);

    return true;
}

void PollInput () {
    SDL_Event e;

    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            game.running = false;
        }
    }

    input.up = keyboard[SDL_SCANCODE_W];
    input.down = keyboard[SDL_SCANCODE_S];
    input.right = keyboard[SDL_SCANCODE_D];
    input.left = keyboard[SDL_SCANCODE_A];
    input.shoot = keyboard[SDL_SCANCODE_SPACE];
}

void Update (f32 delta_time) {
    i32 horizontal_axis = input.right - input.left; 
    EntityRotate(player, 5 * delta_time * horizontal_axis); 

    i32 vertical_axis = input.up - input.down;
    v2 delta = {Cos(player->rotation), Sin(player->rotation)};
    player->velocity = V2MultiplyF32(delta, vertical_axis * delta_time);


    // DEMO PART 3: Iterating 
    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        Entity* curr = (Entity*) (g_entities.ptr + (i * sizeof(Entity)));
        if (curr != NULL) {
            if (curr->active) {
                if (curr->type == ENTITY_ENEMY) {
                    for (int i =0; i < active_bullets.size(); i++) {                            
                        if (active_bullets[i]->active && CircleCircleCollision(curr, active_bullets[i])) {
                            score += 10;
                            EntityDeactivate(curr);
                            EntityDeactivate(active_bullets[i]);
                            active_bullets.erase(active_bullets.begin() + i);
                        }
                    }

                    v2 player_direction = V2Normalize(V2MinusV2(player->centroid_position, curr->centroid_position));
                    curr->velocity = V2MultiplyF32(player_direction, delta_time);
                }

                EntityMove(curr);
            }
        }
    }

    for (int i = 0; i < active_bullets.size(); i++) {
        v2 c = active_bullets[i]->centroid_position;
        if (c.x < 0 || c.x > DEFAULT_WINDOW_WIDTH || c.y < 0 || c.y > DEFAULT_WINDOW_HEIGHT) {
            EntityDeactivate(active_bullets[i]);
            active_bullets.erase(active_bullets.begin() + i);
        }
    }

    if (input.shoot && TimerGetElapsedTime(shoot_delay_timer) > shoot_delay) {
        TimerStart(&shoot_delay_timer);
        Entity* bullet = EntitySpawn();
        if (bullet != NULL) {
            bullet->type = ENTITY_BULLET;

            bullet->shape = SHAPE_CIRCLE;
            bullet->half_width = 10;
            bullet->color = v4 (255, 255, 0, 255);

            v2 nose_position = V2RotateAboutV2(v2(player->centroid_position.x + player->half_width, player->centroid_position.y), player->centroid_position, player->rotation);
            EntitySetPosition(bullet, nose_position);
            bullet->speed = 2.5f;
            bullet->velocity = V2Normalize(V2MinusV2(nose_position, player->centroid_position));

            active_bullets.push_back(bullet);
        }
    }

    if (TimerGetElapsedTime(spawn_timer) > spawn_delay) {
        printf("Spawning\n");
        TimerStart(&spawn_timer);
        spawn_delay = RandomF32(3, 6);

        u32 N = RandomF32(1, 5); 
        for (int i = 0; i < N; i++) {
            Entity* enemy = EntitySpawn();
            if (enemy != NULL) {
                enemy->type = ENTITY_ENEMY;
                enemy->shape = SHAPE_CIRCLE;
                EntitySetPosition(enemy, v2(RandomF32(0, DEFAULT_WINDOW_WIDTH), RandomF32(0, DEFAULT_WINDOW_HEIGHT)));
                enemy->color = v4(255, 0, 0, 255);
                enemy->half_width = 20;
                enemy->velocity = v2(0, 0);
                enemy->speed = RandomF32(10, 40);
                EntitySetRotation(enemy, 0);
            } else {
                break;
            }
        }
    }
}

void Draw () {
    DrawClear(game.renderer,v4(0, 0, 0, 255));

    char _score[128];
    sprintf(_score, "Score: %d", score); 
    TextInit(&score_text, _score, game.renderer, font, v4(255, 255, 255, 255));

    ImageRender(game.renderer, v2(10, 10), &score_text.image);

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        Entity* curr = (Entity*) (g_entities.ptr + (i * sizeof(Entity)));
        if (curr) {
            if (curr->active){
                EntityDraw(game.renderer, curr);
            }
        }
    }

    SDL_RenderPresent(game.renderer);
}

void Close (void) {
    TTF_CloseFont(font);
    font = NULL;

    ImageFree(&score_text.image);

    // DEMO PART 4: Freeing
    ArenaFree(&g_entities);

    SDL_DestroyWindow(game.window);
    SDL_DestroyRenderer(game.renderer);
    IMG_Quit();
    SDL_Quit();
}

int main (void) {
    if (!Init()) {
        printf("Failed to initialize! \n");
        Close();
        return -1;
    } 

    Timer cap_timer;
    Timer step_timer;

    f32 delta_time = 0;
    while (game.running) {
        TimerStart(&cap_timer);

        // Input
        PollInput();

        // Update 
        delta_time = TimerGetElapsedTime(step_timer);
        Update(delta_time);
        TimerStart(&step_timer);

        // Draw
        Draw();

        // Frame Cap
        int frameTicks = TimerGetTicks(cap_timer);
        if(frameTicks < SCREEN_TICKS_PER_FRAME)
        {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
        }
    }

    Close();
    return 0;
}
