#ifndef main_h
#define main_h

#define SCREEN_HEIGHT 1000.0f
#define SCREEN_WIDTH 1000.0f + SCREEN_HEIGHT
#define mapSize 10
#define cellSize SCREEN_HEIGHT / mapSize

#define rotationSpeed 0.02f
#define moveSpeed 0.02f

#define max(a, b) ({__typeof__(a) _a = (a), _b = (b); _a > _b ? _a : _b; })
#define min(a, b) ({__typeof__(a) _a = (a), _b = (b); _a < _b ? _a : _b; })
#define absf(a) ({__typeof__(a) _a = (a); _a > 0 ? _a : _a * -1; })


#endif
