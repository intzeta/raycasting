#ifndef camera_h
#include "util/util.h"
#define camera_h

typedef struct {
  vec2 pos;
  vec2 dir;
  vec2 plane;

  f32 radius;
} camera;

// Mozna tu zaoszczedzic kilka bajtow uzywajac boola
// Bo rotacja jest tylko na -1 i 1 wiec mozna by zrobic
// na 0 i 1. Zmienic przy okazji w camera.c

void move(camera *cam, s32 rot);
void rotation(camera *cam, s32 rot);

#endif
