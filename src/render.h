#ifndef render_h
#define render_h

#include "util/util.h"
#include "color.h"
#include "camera.h"

static const u32 mapData[mapSize][mapSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 2, 2, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 3, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 2, 3, 4, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Najprawodopodonie static jest tu nie potrzebny a wrecz szkodliwy
// ale narazie niech bedzie dopoki nie bedzie trzeba wywolac tych funkcji
// gdzies indziej.
void renderMap(SDL_Renderer *renderer, camera cam);

#endif
