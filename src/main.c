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
  {1, 0, 0, 0, 0, 0, 4, 4, 0, 1},
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

f32 absolute(f32 value){
  return value > 0 ? value : value * -1;
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
      else if(map[i][j] == 1) rgba = (color){ 0x00, 0x00, 0x00, 0x00};
      else if(map[i][j] == 2) rgba = (color){ 0xFF, 0x00, 0x00, 0xFF};
      else if(map[i][j] == 3) rgba = (color){ 0x00, 0xFF, 0x00, 0xFF};
      else if(map[i][j] == 4) rgba = (color){ 0x00, 0x00, 0xFF, 0xFF};

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
  SDL_Rect pixel = (SDL_Rect){
    SCREEN_HEIGHT,
    0,
    SCREEN_WIDTH - SCREEN_HEIGHT,
    SCREEN_HEIGHT,
  };
  SDL_RenderFillRect(renderer, &pixel);
  SDL_RenderDrawRect(renderer, &pixel);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
  SDL_RenderDrawLine(renderer, cam->pos.x, cam->pos.y, cam->pos.x + cam->dir.x, cam->pos.y + cam->dir.y);
  SDL_RenderDrawLine(renderer, cam->pos.x + cam->dir.x, cam->pos.y + cam->dir.y, cam->pos.x + cam->dir.x + cam->plane.x, cam->pos.y + cam->dir.y + cam->plane.y);
  SDL_RenderDrawLine(renderer, cam->pos.x + cam->dir.x, cam->pos.y + cam->dir.y, cam->pos.x + cam->dir.x - cam->plane.x, cam->pos.y + cam->dir.y - cam->plane.y);
  SDL_RenderDrawLine(renderer, cam->pos.x + cam->dir.x, cam->pos.y + cam->dir.y, cam->pos.x + cam->dir.x - cam->plane.x, cam->pos.y + cam->dir.y - cam->plane.y);


  for(f32 x = 0; x < SCREEN_WIDTH - SCREEN_HEIGHT; x++){
    f32 cameraX = 2 * x / (SCREEN_WIDTH - SCREEN_HEIGHT) - 1.0f;

    s32 mapX = cam->pos.x;
    s32 mapY = cam->pos.y;

    f32 rayDirX = cam->dir.x + cam->plane.x * cameraX;
    f32 rayDirY = cam->dir.y + cam->plane.y * cameraX;


    f32 deltaDistX = (rayDirX == 0.0f) ? 1e30 : absolute(1 / rayDirX);
    f32 deltaDistY = (rayDirY == 0.0f) ? 1e30 : absolute(1 / rayDirY);

    f32 sideDistX, sideDistY;
    s32 stepX, stepY, hit = 0, side;

    if(rayDirX < 0){
      stepX = -1;
      sideDistX = (cam->pos.x - mapX) * deltaDistX;
    }else{
      stepX = 1;
      sideDistX = (mapX + 1.0 - cam->pos.x) * deltaDistX;
    }
    if(rayDirY < 0){
      stepY = -1;
      sideDistY = (cam->pos.y - mapY) * deltaDistY;
    }else{
      stepY = 1;
      sideDistY = (mapY + 1.0 - cam->pos.y) * deltaDistY;
    }

    while(hit == 0){
      if(sideDistX < sideDistY){
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }else{
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }

      if(map[mapX][mapY] > 0) hit = 1;
    }

    f32 perpWallDist;

    if(side == 0) perpWallDist = (sideDistX - deltaDistX);
    else perpWallDist = (sideDistY - deltaDistY);

    s32 lineHeight = SCREEN_HEIGHT / perpWallDist;
    s32 drawStart, drawEnd;

    drawStart = -lineHeight / 2.0f + SCREEN_HEIGHT / 2.0f;
    if(drawStart < 0) drawStart = 0;

    drawEnd = lineHeight / 2.0f + SCREEN_HEIGHT / 2.0f;
    if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    if(map[mapX][mapY] == 1) rgba = (color){ 0x00, 0x00, 0x00, 0x00};
    else if(map[mapX][mapY] == 2) rgba = (color){ 0xFF, 0x00, 0x00, 0xFF};
    else if(map[mapX][mapY] == 3) rgba = (color){ 0x00, 0xFF, 0x00, 0xFF};
    else if(map[mapX][mapY] == 4) rgba = (color){ 0x00, 0x00, 0xFF, 0xFF};

    if(side == 1){
      rgba.r /= 2;
      rgba.g /= 2;
      rgba.b /= 2;
    }
    SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
    SDL_RenderDrawLine(renderer, x + SCREEN_HEIGHT, drawStart, x + SCREEN_HEIGHT, drawEnd);
  }


  SDL_Rect player;
    player.x = cam->pos.y * 100 - cam->radius,
    player.y = cam->pos.x * 100 - cam->radius,
    player.w = cam->radius,
    player.h = cam->radius,

  printf("%d, %d, %d, %d\n", player.x, player.y, player.w, player.h);
  drawRect(renderer, &player, 0x00, 0x00, 0x00, 0xFF);
}

int main(int argc, char* args[]){
  if(init()){
    SDL_Event event;
    bool quit = 0;

    camera cam;
    cam.pos.x = 2;
    cam.pos.y = 2;
    cam.dir.x = -1;
    cam.dir.y = 0;
    cam.plane.x = 0;
    cam.plane.y = 0.66;
    cam.radius = 5;

    u32 tick, lastTicks = 0, delta;
    f32 rotationSpeed = 0.05f;
    f32 moveSpeed = 0.02f;

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
      if(keyState[SDL_SCANCODE_W]){
        cam.pos.x += cam.dir.x * moveSpeed;
        cam.pos.y += cam.dir.y * moveSpeed;
      }
      if(keyState[SDL_SCANCODE_S]){
        cam.pos.x -= cam.dir.x * moveSpeed;
        cam.pos.y -= cam.dir.y * moveSpeed;
      }
      if(keyState[SDL_SCANCODE_J]){
        f32 oldDir = cam.dir.x;
        cam.dir.x = cam.dir.x * cos(rotationSpeed) - cam.dir.y * sin(rotationSpeed);
        cam.dir.y = oldDir * sin(rotationSpeed) + cam.dir.y * cos(rotationSpeed);
        f32 oldPlane = cam.plane.x;
        cam.plane.x = cam.plane.x * cos(rotationSpeed) - cam.plane.y * sin(rotationSpeed);
        cam.plane.y = oldPlane * sin(rotationSpeed) + cam.plane.y * cos(rotationSpeed);
      }
      if(keyState[SDL_SCANCODE_L]){
        f32 oldDir = cam.dir.x;
        cam.dir.x = cam.dir.x * cos(-rotationSpeed) - cam.dir.y * sin(-rotationSpeed);
        cam.dir.y = oldDir * sin(-rotationSpeed) + cam.dir.y * cos(-rotationSpeed);
        f32 oldPlane = cam.plane.x;
        cam.plane.x = cam.plane.x * cos(-rotationSpeed) - cam.plane.y * sin(-rotationSpeed);
        cam.plane.y = oldPlane * sin(-rotationSpeed) + cam.plane.y * cos(-rotationSpeed);
      }
      SDL_Delay(5);

      printf("cam: %f   %f  dir:  %f  %f  plane:  %f  %f", cam.pos.x, cam.pos.y, cam.dir.x, cam.dir.y, cam.plane.x, cam.plane.y);
    }
  }

  quit();
  return 0;
}
