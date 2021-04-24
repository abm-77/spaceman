#include "entity.h"
#include "graphics.h"

u32 ENTITY_COUNT = 0;
u32 next_free_entity = -1;
Entity* entity_free_list[MAX_ENTITY_COUNT];

void EntityDraw (SDL_Renderer* renderer, Entity* e) {
    v2 c = e->centroid_position;
    i32 hw = e->half_width;
    v4 col = e->color;
    f32 rot = e->rotation;

    switch (e->shape) {
        case SHAPE_CIRCLE: {
            DrawCircleOutline(renderer, c, hw, col);
            break;
        }
        case SHAPE_TRIANGLE: {
            v2 points[3] = {
                V2RotateAboutV2(v2(c.x + hw, c.y), c, rot),
                V2RotateAboutV2(v2(c.x - hw, c.y + hw), c, rot), 
                V2RotateAboutV2(v2(c.x - hw, c.y - hw), c, rot)
            };

            DrawTriangleOutline(renderer, points, col);
            break;
        }
        case SHAPE_RECTANGLE: {
            v2 points[4] = {
                V2RotateAboutV2(v2(c.x - hw, c.y - hw), c, rot),
                V2RotateAboutV2(v2(c.x + hw, c.y - hw), c, rot),
                V2RotateAboutV2(v2(c.x - hw, c.y + hw), c, rot),
                V2RotateAboutV2(v2(c.x + hw, c.y + hw), c, rot) 
            };

            DrawRectangleOutlineEX(renderer, points, col);
            break;
        }
    }
}

// DEMO PART 2: Allocating
Entity* EntityCreate(M_Arena* allocator) {
    Entity* entity = NULL;

    if (ENTITY_COUNT < MAX_ENTITY_COUNT) {
        entity = (Entity*) ArenaAllocate(allocator, sizeof(Entity));
        EntityClear(entity);

        next_free_entity += 1;
        entity_free_list[next_free_entity] = entity;

        ENTITY_COUNT += 1;
    }

    return entity;
}

void EntitySetPosition (Entity* e, v2 position) {
    e->centroid_position = position;
}

void EntityMove (Entity* e) {
    e->centroid_position = V2AddV2(e->centroid_position, V2MultiplyF32(e->velocity, e->speed));
}

void EntitySetRotation(Entity* e, f32 angle) {
    e->rotation = FMod(angle * (PI / 180), 360);
}

void EntityRotate(Entity* e, f32 delta_angle) {
    e->rotation = FMod(e->rotation + delta_angle, 360);
}

// DEMO PART 5: Free List
Entity* EntitySpawn() {
    Entity* e = NULL;

    if (next_free_entity >= 0 && next_free_entity < MAX_ENTITY_COUNT) {
        e = entity_free_list[next_free_entity];

        EntityClear(e); 
        e->active = true;

        next_free_entity -= 1;
    }

    return e;
}

Entity* EntityDeactivate(Entity* e)  {
    printf("Entity was deactivated!\n");
    next_free_entity += 1;
    entity_free_list[next_free_entity] = e;
    EntityClear(e);
}

void EntityClear(Entity* e) {
    e->active = false;
    e->type = ENTITY_NONE;
    e->shape = -1;
    e->color = v4(0, 0, 0, 0);
    e->centroid_position = v2 (0, 0);
    e->half_width = 0;
    e->velocity = v2 (0, 0);
    e->speed = 0;
    e->rotation = 0;
}