#pragma once
#include "language_layer.h"
#include "entity.h"

b32 CircleCircleCollision (Entity* e1, Entity* e2) {
    f32 total_radius_squared = e1->half_width + e2->half_width;
    total_radius_squared = total_radius_squared * total_radius_squared;
    f32 center_to_center_distance_squared = V2LengthSquared(V2MinusV2(e1->centroid_position, e2->centroid_position));

    if (center_to_center_distance_squared < total_radius_squared)
        return true;

    return false;
}

// TODO(bryson): maybe make it so the order doesn't matter?
b32 TriangleCircleCollision (Entity* e1, Entity* e2) { // e1 = triangle, e2 = circle
    // Not a Triangle-Circle collision
    if (!((e1->shape == SHAPE_TRIANGLE && e2->shape == SHAPE_CIRCLE) || (e1->shape == SHAPE_CIRCLE && e2->shape == SHAPE_CIRCLE))) {
        printf("THIS FUNCTION ONLY TESTS COLLISIONS BETWEEN TRIANGLES AND CIRCLES!\n");
        return false;
    }

    v2 c_triangle = e1->centroid_position;
    f32 hw = e1->half_width;
    f32 rot = e1->rotation;

    v2 c_circle = e2->centroid_position;
    f32 radius_squared = e2->half_width * e2->half_width;
    
    v2 vertices [3] = {
        V2RotateAboutV2(v2(c_triangle.x + hw, c_triangle.y), c_triangle, rot),
        V2RotateAboutV2(v2(c_triangle.x - hw, c_triangle.y + hw), c_triangle, rot),
        V2RotateAboutV2(v2(c_triangle.x - hw, c_triangle.y - hw), c_triangle, rot)
    };

    v2 edges [3];
    v2 vector_directions [3];
    f32 vector_square_lengths [3];

    for (int i = 0; i < 3; i++) {
        vector_directions[i] = V2MinusV2(c_circle, vertices[i]);
        vector_square_lengths[i] = V2LengthSquared(vector_directions[i]) - radius_squared;

        // Vertex Inside Circle
        if (vector_square_lengths[i] <= 0) 
            return true;

        v2 next = vertices[(i+1) % 3];
        edges[i] = V2MinusV2(next, vertices[i]);
    }

    // Circle Inside Triangle
    if (((edges[0].y * vector_directions[0].x) - (edges[0].x * vector_directions[0].y) <= 0) &&
        ((edges[1].y * vector_directions[1].x) - (edges[1].x * vector_directions[1].y) <= 0) &&
        ((edges[2].y * vector_directions[2].x) - (edges[2].x * vector_directions[2].y) <= 0))
        return true;
    
    // Edge Intersection
    for (int i = 0; i < 3; i++) {
        f32 k = edges[i].x * vector_directions[i].x + edges[i].y * vector_directions[i].y;
        if (k > 0) {
            f32 len_sqr = edges[i].x*edges[i].x + edges[i].y*edges[i].y;     
            if (k < len_sqr) {
                if (vector_square_lengths[i] * len_sqr <= k*k) return true;
            }
        }
    }

    return false;
}