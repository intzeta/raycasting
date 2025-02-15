#include "render.h"
#include "window.h"

int main(int argc, char* args[]){
  if(init()){
    SDL_Event event;
    bool quit = 0;

    camera cam = {
      {5, 5},
      {0, 1},
      {0.66f, 0},
      5,
    };

    u32 tick, lastTicks = 0, delta = 0;

    while(!quit){
      while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) quit = 1;
      }

      tick = SDL_GetTicks();
      delta += tick - lastTicks;
      lastTicks = tick;

      if(delta >= 1000.0f / 240.0f){
        delta = 0;

        renderMap(renderer, cam);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        if(keyState[SDL_SCANCODE_W]) move(&cam, 1);
        if(keyState[SDL_SCANCODE_S]) move(&cam, -1);
        if(keyState[SDL_SCANCODE_J]) rotation(&cam, -1);
        if(keyState[SDL_SCANCODE_L]) rotation(&cam, 1);
      }

    }
  }

  quit();
  return 0;
}
