#include "main.h"
#include "color.h"
#include "camera.h"
#include "util/util.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const u32 mapData[mapSize][mapSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 2, 2, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 2, 3, 4, 1},
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

f32 absf(f32 value){
  return value > 0 ? value : value * -1;
}

void drawRect(SDL_Renderer *renderer, SDL_Rect *box, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, box);
  SDL_RenderDrawRect(renderer, box);
}

void drawMap(camera cam){

  // Map will be on the most left pixel area

  color rgba;

  for(u32 i = 0; i <= mapSize; i++){
    for(u32 j = 0; j <= mapSize; j++){

      if(mapData[i][j] == 0) rgba = (color){ 0xFF, 0xFF, 0xFF, 0xFF};
      else if(mapData[i][j] == 1) rgba = (color){ 0x00, 0x00, 0x00, 0x00};
      else if(mapData[i][j] == 2) rgba = (color){ 0xFF, 0x00, 0x00, 0xFF};
      else if(mapData[i][j] == 3) rgba = (color){ 0x00, 0xFF, 0x00, 0xFF};
      else if(mapData[i][j] == 4) rgba = (color){ 0x00, 0x00, 0xFF, 0xFF};

      SDL_Rect box = (SDL_Rect){
        j * SCREEN_HEIGHT / 10,
        i * SCREEN_HEIGHT / 10,
        SCREEN_HEIGHT / 10,
        SCREEN_HEIGHT / 10,
      };

      drawRect(renderer, &box, rgba.r, rgba.g, rgba.b, rgba.a);
    }
  }


  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_Rect playArea = (SDL_Rect){
    SCREEN_HEIGHT,
    0,
    SCREEN_WIDTH - SCREEN_HEIGHT,
    SCREEN_HEIGHT,
  };
  SDL_RenderFillRect(renderer, &playArea);
  SDL_RenderDrawRect(renderer, &playArea);

  for(f32 x = 0; x < SCREEN_WIDTH - SCREEN_HEIGHT; x++){
    f32 camX = 2 * x / (SCREEN_WIDTH - SCREEN_HEIGHT) - 1.0f; // -1, ... , 0, ... , -1

    // Getting the position of the player on 2D map
    vec2i map = {
      cam.pos.x,
      cam.pos.y,
    };

    // Calculating the ray direction thanks to the Plane and camX
    // CamX is used to denote on what part of cam->plane we are on
    // :)

    vec2 rayDir = {
      cam.dir.x + cam.plane.x * camX,
      cam.dir.y + cam.plane.y * camX,
    };

    vec2 deltaDist = {
      (rayDir.x == 0.0f) ? 1e30 : absf(1 / rayDir.x),
      (rayDir.y == 0.0f) ? 1e30 : absf(1 / rayDir.y),
    };

    s32 stepX, stepY, hit = 0, side;

    vec2 sideDist;

    if(rayDir.x < 0){
      stepX = -1;
      sideDist.x = (cam.pos.x - map.x) * deltaDist.x;
    }else{
      stepX = 1;
      sideDist.x = (map.x + 1.0 - cam.pos.x) * deltaDist.x;
    }
    if(rayDir.y < 0){
      stepY = -1;
      sideDist.y = (cam.pos.y - map.y) * deltaDist.y;
    }else{
      stepY = 1;
      sideDist.y = (map.y + 1.0 - cam.pos.y) * deltaDist.y;
    }

    while(hit == 0){
      if(sideDist.x < sideDist.y){
        sideDist.x += deltaDist.x;
        map.x += stepX;
        side = 0;
      }else{
        sideDist.y += deltaDist.y;
        map.y += stepY;
        side = 1;
      }

      if(mapData[map.x][map.y] > 0) hit = 1;
    }

    f32 perpWallDist;

    if(side == 0) perpWallDist = (sideDist.x - deltaDist.x);
    else perpWallDist = (sideDist.y - deltaDist.y);

    s32 lineHeight = SCREEN_HEIGHT / perpWallDist;
    s32 drawStart, drawEnd;

    drawStart = -lineHeight / 2.0f + SCREEN_HEIGHT / 2.0f;
    if(drawStart < 0) drawStart = 0;

    drawEnd = lineHeight / 2.0f + SCREEN_HEIGHT / 2.0f;
    if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    if(mapData[map.x][map.y] == 1) rgba = (color){ 0x00, 0x00, 0x00, 0x00};
    else if(mapData[map.x][map.y] == 2) rgba = (color){ 0xFF, 0x00, 0x00, 0xFF};
    else if(mapData[map.x][map.y] == 3) rgba = (color){ 0x00, 0xFF, 0x00, 0xFF};
    else if(mapData[map.x][map.y] == 4) rgba = (color){ 0x00, 0x00, 0xFF, 0xFF};

    if(side == 1){
      rgba.r /= 2;
      rgba.g /= 2;
      rgba.b /= 2;
    }
    SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
    SDL_RenderDrawLine(renderer, x + SCREEN_HEIGHT, drawStart, x + SCREEN_HEIGHT, drawEnd);
  }


  SDL_Rect player;
    player.x = cam.pos.y * 100 - cam.radius,
    player.y = cam.pos.x * 100 - cam.radius,
    player.w = cam.radius,
    player.h = cam.radius,

  drawRect(renderer, &player, 0x00, 0x00, 0x00, 0xFF);
}

// -1 - Rotation left
//  1 - Rotation right

void rotation(camera *cam, s32 rot){
  f32 oldDir = cam->dir.x;
  f32 oldPlane = cam->plane.x;

  cam->dir.x = cam->dir.x * cos(rot * -rotationSpeed) - cam->dir.y * sin(rot * -rotationSpeed);
  cam->dir.y = oldDir * sin(rot * -rotationSpeed) + cam->dir.y * cos(rot * -rotationSpeed);

  cam->plane.x = cam->plane.x * cos(rot * -rotationSpeed) - cam->plane.y * sin(rot * -rotationSpeed);
  cam->plane.y = oldPlane * sin(rot * -rotationSpeed) + cam->plane.y * cos(rot * -rotationSpeed);
}

int main(int argc, char* args[]){
  if(init()){
    SDL_Event event;
    bool quit = 0;

    camera cam = {
      {2, 2},
      {-1, 0},
      {0, 0.66},
      5,
    };
    u32 tick, lastTicks = 0, delta;

    while(!quit){
      while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT) quit = 1;
      }

      tick = SDL_GetTicks();
      delta = tick - lastTicks;
      lastTicks = tick;
      printf("\n%d", delta);

      drawMap(cam);
      SDL_RenderPresent(renderer);

      const Uint8* keyState = SDL_GetKeyboardState(NULL);
      if(keyState[SDL_SCANCODE_W]){
        cam.pos.x += cam.dir.x * moveSpeed;
        cam.pos.y += cam.dir.y * moveSpeed;
      }
      if(keyState[SDL_SCANCODE_S]){
        cam.pos.x -= cam.dir.x * moveSpeed;
        cam.pos.y -= cam.dir.y * moveSpeed;
      }
      if(keyState[SDL_SCANCODE_J]) rotation(&cam, -1);
      if(keyState[SDL_SCANCODE_L]) rotation(&cam, 1);
      SDL_Delay(5);

    }
  }

  quit();
  return 0;
}
