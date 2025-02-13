#ifndef types_h
#define types_h

#include <stdint.h>

typedef uint32_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

typedef struct {
  f32 x;
  f32 y;
} vec2;

typedef struct {
  u32 x;
  u32 y;
} vec2i;

typedef struct {
  u32 x;
  u32 y;
} vec2u;
#endif
