#ifndef camera_h
#include "util/types.h"
#define camera_h

typedef struct {
  f64 posX;
  f64 posY;

  f64 dirX;
  f64 dirY;

  f64 planeX;
  f64 planeY;

  f64 radius;
} camera;

#endif
