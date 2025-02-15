#include "camera.h"

// Zmiena rot i jej znaczenie
// -1 - Do tylu
//  1 - Do przodu

void move(camera *cam, s32 rot){
  cam->pos.x += (cam->dir.x * moveSpeed) * rot;
  cam->pos.y += (cam->dir.y * moveSpeed) * rot;
}

// Zmiena rot i jej znaczenie
// -1 - Rotacja w lewo
//  1 - Rotacja w prawo

void rotation(camera *cam, s32 rot){
  f32 oldDir = cam->dir.x;
  f32 oldPlane = cam->plane.x;

  cam->dir.x = cam->dir.x * cos(rot * -rotationSpeed) - cam->dir.y * sin(rot * -rotationSpeed);
  cam->dir.y = oldDir * sin(rot * -rotationSpeed) + cam->dir.y * cos(rot * -rotationSpeed);

  cam->plane.x = cam->plane.x * cos(rot * -rotationSpeed) - cam->plane.y * sin(rot * -rotationSpeed);
  cam->plane.y = oldPlane * sin(rot * -rotationSpeed) + cam->plane.y * cos(rot * -rotationSpeed);
}
