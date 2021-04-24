#pragma once

#include <SDL2/SDL.h>
#include "maths.h"
#include "memory.h"

#define MAX_ENTITY_COUNT 1000

enum EntityType {
    ENTITY_NONE,
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_BULLET,
};

enum {
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE,
    SHAPE_RECTANGLE,
};

struct Entity {
    b32 active;
    EntityType type;

    i32 shape;
    v2 centroid_position;
    u32 half_width;
    f32 rotation = 0;

    v2 velocity;
    f32 speed;

    v4 color;
};

extern u32 ENTITY_COUNT;
extern u32 next_free_entity;
extern Entity* entity_free_list[MAX_ENTITY_COUNT];

void EntityClear(Entity* e);

Entity* EntityCreate(M_Arena* allocator);
Entity* EntitySpawn();
Entity* EntityDeactivate(Entity* e); 

void EntityDraw (SDL_Renderer* renderer, Entity* e); 

void EntityMove (Entity* e); 
void EntitySetPosition (Entity* e, v2 position); 

void EntitySetRotation(Entity* e, f32 angle);
void EntityRotate(Entity* e, f32 delta_angle);