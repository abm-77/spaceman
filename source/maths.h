#pragma once
#include "language_layer.h"

typedef union v2 v2;
union v2 {

    struct {
        f32 x;
        f32 y;
    };

    struct {
        f32 width;
        f32 height;
    };

    float elements[2];
};

typedef union v3 v3;
union v3 {

    struct {
        f32 x;
        f32 y;
        f32 z;
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
    };

    float elements[3];
};

typedef union v4 v4;
union v4 {

    struct {
        f32 x;
        f32 y;
        
        union {
            struct {
                f32 z;

                union {
                    f32 w;
                    f32 radius;
                };
            };
            struct {
                f32 width;
                f32 height;
            };
        };
    };

    struct {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

    float elements[4];
};

typedef union iv2 iv2;
union iv2
{
    struct
    {
        i32 x;
        i32 y;
    };
    
    struct
    {
        i32 width;
        i32 height;
    };
    
    i32 elements[2];
};

typedef union iv3 iv3;
union iv3
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
    };
    
    i32 elements[3];
};

typedef union iv4 iv4;
union iv4
{
    struct
    {
        i32 x;
        i32 y;
        i32 z;
        i32 w;
    };
    
    struct
    {
        i32 r;
        i32 g;
        i32 b;
        i32 a;
    };
    
    i32 elements[4];
};

#define v2(...)   (v2){ __VA_ARGS__ }
#define v3(...)   (v3){ __VA_ARGS__ }
#define v4(...)   (v4){ __VA_ARGS__ }
#define iv2(...) (iv2){ __VA_ARGS__ }
#define iv3(...) (iv3){ __VA_ARGS__ }
#define iv4(...) (iv4){ __VA_ARGS__ }

typedef struct m4 m4;
struct m4
{
    f32 elements[4][4];
};

v2 V2RotateAboutV2(v2 point, v2 center, f32 angle);

v2 V2AddV2(v2 a, v2 b);
v2 V2MinusV2(v2 a, v2 b);
v2 V2MultiplyF32(v2 v, f32 f);
f32 V2LengthSquared(v2 a);
f32 V2Length(v2 a);
f32 V2Dot(v2 a, v2 b);
v2 V2Normalize(v2 v);

v3 V3AddV3(v3 a, v3 b);
v3 V3MinusV3(v3 a, v3 b);
v3 V3MultiplyF32(v3 v, f32 f);
f32 V3LengthSquared(v3 a);
f32 V3Length(v3 a);
v3 V3Normalize(v3 v);
f32 V3Dot(v3 a, v3 b);
v3 V3Cross(v3 a, v3 b);
f32 MinimumInV3(v3 v);
f32 MaximumInV3(v3 v);

b32 V4RectHasPoint(v4 v, v2 p);
f32 V4Dot(v4 a, v4 b);
v4 V4AddV4(v4 a, v4 b);
v4 V4MinusV4(v4 a, v4 b);
v4 V4MultiplyV4(v4 a, v4 b);
v4 V4MultiplyM4(v4 v, m4 m);

m4 M4InitD(f32 diagonal);
m4 M4MultiplyM4(m4 a, m4 b);
m4 M4MultiplyF32(m4 a, f32 b);
m4 M4TranslateV3(v3 translation);
m4 M4ScaleV3(v3 scale);
m4 M4Perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z);
m4 M4LookAt(v3 eye, v3 center, v3 up);
m4 M4Inverse(m4 m);
m4 M4RemoveRotation(m4 mat);

v3 RGBToHSV(v3 rgb);
v3 HSVToRGB(v3 hsv);