#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// Memory
#define MemoryCopy memcpy
#define MemoryMove memmove
#define MemorySet memset


// Math
#define FMod fmodf
#define AbsoluteValue fabsf
#define SquareRoot sqrtf
#define Floor floorf
#define Sin sinf
#define Cos cosf
#define Tan tanf
#define PI (3.1415926535897f)

// String
#define CStringToI32(s) ((i32)atoi(s))
#define CStringToI16(s) ((i16)atoi(s))
#define CStringToF32(s) ((f32)atof(s))
#define CalculateCStringLength (u32) strlen

// Helpers
#define global static
#define internal static
#define local_persist static
#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))
#define Bytes(n) n
#define Kilobytes(n) (n << 10)
#define Megabytes(n) (n << 20)
#define Gigabytes(n)  (((u64)n) << 30)
#define Terabytes(n)  (((u64)n) << 40)

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef i8       s8;
typedef i16      s16;
typedef i32      s32;
typedef i64      s64;
typedef i8       b8;
typedef i16      b16;
typedef i32      b32;
typedef i64      b64;
typedef float    f32;
typedef double   f64;

internal void SeedRandomNumberGeneratorWithTime(void)
{
    srand((unsigned int)time(0));
}

internal void SeedRandomNumberGenerator(unsigned int seed)
{
    srand(seed);
}

internal f32 RandomF32(f32 low, f32 high)
{
    return low + (high - low) * ((rand() % 10000) / 10000.f);
}
