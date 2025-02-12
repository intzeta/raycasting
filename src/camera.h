#ifndef camera_h
#include "util/types.h"
#define camera_h

typedef struct {
  vec2 pos;
  vec2 dir;
  vec2 plane;

  f32 radius;
} camera;

#endif
