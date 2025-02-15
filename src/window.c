#include "window.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool init(){
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("init failed! - %s\n", SDL_GetError());
    return 0;
  }
  window = SDL_CreateWindow("Raycasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if(window == NULL){
    printf("window failed! - %s\n", SDL_GetError());
    return 0;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL){
    printf("renderer failed! - %s\n", SDL_GetError());
    return 0;
  }

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderPresent(renderer);

  return 1;
}

void quit(){
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  window = NULL;
  renderer = NULL;
  SDL_Quit();
}

