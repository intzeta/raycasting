#include "main.h"
#include "color.h"
#include "camera.h"
#include "util/util.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const u32 map[mapSize][mapSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 3, 3, 1},
  {1, 2, 2, 0, 0, 0, 0, 0, 3, 1},
  {1, 0, 0, 0, 0, 0, 4, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 4, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 4, 4, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

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

void drawRect(SDL_Renderer *renderer, SDL_Rect *box, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, box);
  SDL_RenderDrawRect(renderer, box);
}

void drawMap(camera *cam){

  // Map will be on the most left pixel area

  color rgba;

  for(u32 i = 0; i <= mapSize; i++){
    for(u32 j = 0; j <= mapSize; j++){

      if(map[i][j] == 0) rgba = (color){ 0xFF, 0xFF, 0xFF, 0xFF};
      if(map[i][j] == 1) rgba = (color){ 0x00, 0x00, 0x00, 0x00};
      if(map[i][j] == 2) rgba = (color){ 0xFF, 0x00, 0x00, 0xFF};
      if(map[i][j] == 3) rgba = (color){ 0x00, 0xFF, 0x00, 0xFF};
      if(map[i][j] == 4) rgba = (color){ 0x00, 0x00, 0xFF, 0xFF};

      SDL_Rect box = (SDL_Rect){
        j * SCREEN_HEIGHT / 10,
        i * SCREEN_HEIGHT / 10,
        SCREEN_HEIGHT / 10,
        SCREEN_HEIGHT / 10,
      };

      drawRect(renderer, &box, rgba.r, rgba.g, rgba.b, rgba.a);
    }
  }

  f64 radiusSquared = cam->radius * cam->radius;
  for(f64 i = cam->posX - cam->radius; i <= cam->posX + cam->radius; i++){
    for(f64 j = cam->posY - cam->radius; j <= cam->posY + cam->radius; j++){
      f64 distanceSquared = (i - cam->posX) * (i - cam->posX) + (j - cam->posY) * (j - cam->posY);
      if(distanceSquared < radiusSquared){
        SDL_Rect pixel = (SDL_Rect){
          i,
          j,
          1,
          1,
        };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &pixel);
        SDL_RenderDrawRect(renderer, &pixel);
      }
    }
  }


}

int main(int argc, char* args[]){
  if(init()){
    SDL_Event event;
    bool quit = 0;

    camera cam;
    cam.posX = 200;
    cam.posY = 200;
    cam.radius = 10;

    u32 tick, lastTicks = 0, delta;

    while(!quit){
      while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) quit = 1;
      }

      tick = SDL_GetTicks();
      delta = tick - lastTicks;
      lastTicks = tick;
      printf("\n%d", delta);

      drawMap(&cam);
      SDL_RenderPresent(renderer);

      const Uint8* keyState = SDL_GetKeyboardState(NULL);
      if(keyState[SDL_SCANCODE_A]) cam.posX -= 1;
      if(keyState[SDL_SCANCODE_D]) cam.posX += 1;
      if(keyState[SDL_SCANCODE_W]) cam.posY -= 1;
      if(keyState[SDL_SCANCODE_S]) cam.posY += 1;
      SDL_Delay(5);
    }
  }

  quit();
  return 0;
}
