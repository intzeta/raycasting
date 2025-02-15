#ifndef window_h
#define window_h

#include "util/util.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool init();
void quit();

#endif
