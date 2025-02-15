#include "render.h"

static void drawRect(SDL_Renderer *renderer, SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_Rect rectangle = rect;
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderFillRect(renderer, &rectangle);
  SDL_RenderDrawRect(renderer, &rectangle);
}

static void verticalLine(SDL_Renderer *renderer, u32 x, u32 y1, u32 y2, color rgba){
  SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
  SDL_RenderDrawLine(renderer, x, y1, x, y2);
}

void renderMap(SDL_Renderer *renderer, camera cam){

  // TODO: Change it to bitwise.
  // :)

  color rgba;

  for(u32 i = 0; i <= mapSize; i++){
    for(u32 j = 0; j <= mapSize; j++){

      switch(mapData[i][j]){
        case 0: rgba = (color){ 0xFF, 0xFF, 0xFF, 0xFF}; break;
        case 1: rgba = (color){ 0x00, 0x00, 0x00, 0x00}; break;
        case 2: rgba = (color){ 0xFF, 0x00, 0x00, 0xFF}; break;
        case 3: rgba = (color){ 0x00, 0xFF, 0x00, 0xFF}; break;
        case 4: rgba = (color){ 0x00, 0x00, 0xFF, 0xFF}; break;
        default: rgba = (color){ 0x00, 0x00, 0x00, 0x00}; break;
      }

      drawRect(renderer,
        (SDL_Rect){
          j * SCREEN_HEIGHT / mapSize,
          i * SCREEN_HEIGHT / mapSize,
          SCREEN_HEIGHT / mapSize,
          SCREEN_HEIGHT / mapSize,
      }, rgba.r, rgba.g, rgba.b, rgba.a);
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

  for(u32 x = 0; x < SCREEN_WIDTH - SCREEN_HEIGHT; x++){
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

    switch(mapData[map.x][map.y]){
      case 1: rgba = (color){ 0x00, 0x00, 0x00, 0x00}; break;
      case 2: rgba = (color){ 0xff, 0x00, 0x00, 0xff}; break;
      case 3: rgba = (color){ 0x00, 0xff, 0x00, 0xff}; break;
      case 4: rgba = (color){ 0x00, 0x00, 0xff, 0xff}; break;
      default: rgba = (color){ 0x00, 0x00, 0x00, 0x00}; break;
    }

    if(side == 1){
      rgba.r /= 2;
      rgba.g /= 2;
      rgba.b /= 2;
    }

    if(x % 8 == 0){
      SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
      SDL_RenderDrawLine(renderer,
        cam.pos.y * cellSize - cam.radius / 2,
        cam.pos.x * cellSize - cam.radius / 2,
        cam.pos.y * cellSize + rayDir.y * perpWallDist * cellSize,
        cam.pos.x * cellSize + rayDir.x * perpWallDist * cellSize
      );
    }

    verticalLine(renderer, x + SCREEN_HEIGHT, drawStart, drawEnd, rgba);
    verticalLine(renderer, x + SCREEN_HEIGHT, drawEnd, SCREEN_HEIGHT - 1, (color){0x22, 0x22, 0x22, 0xFF});
  }
}
