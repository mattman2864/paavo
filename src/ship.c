#include "ship.h"
#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>


Vector2 RotatePoint(Vector2 point, Vector2 center, float angle) {
  angle = angle * PI / 180;

  float s = sin(angle);
  float c = cos(angle);

  float x = point.x - center.x;
  float y = point.y - center.y;

  return (Vector2){
    x * c - y * s + center.x,
    x * s + y * c + center.y
  };
}

/*
 * creates and allocates a new ship
 */

ship_t* create_ship(Vector2 position, float acceleration,
                    float ang_acceleration) {
  ship_t* ship = calloc(1, sizeof(ship_t));
  ship->pos = position;
  ship->acc = acceleration;
  ship->ang_acc = ang_acceleration;
  return ship;
}

/*
 * updates a ship's physics based on keyboard+mouse input
 */

void move_ship(ship_t* ship, Vector2 mousePosition, Vector2 dir, float dt) {
  // angle
  Vector2 toMouse = Vector2Subtract(mousePosition,
                    (Vector2){GetScreenWidth() / 2.f, GetScreenHeight() / 2.f});
  float angleToMouse = ship->ang - atan2f(toMouse.y, toMouse.x) * 180.f / PI + 90;
  while (angleToMouse > 180.f)  angleToMouse -= 360.f;
  while (angleToMouse < -180.f) angleToMouse += 360.f;
  float targetAngularVel = Clamp(angleToMouse * ship->ang_acc, 
                                 -MAX_ANGULAR_VEL, MAX_ANGULAR_VEL);
  ship->ang_vel = Lerp(ship->ang_vel, targetAngularVel, 16.0f * dt);
  ship->ang = atan2f(toMouse.y, toMouse.x) * 180 / PI + 90;

  // position
  dir = Vector2Normalize(dir);
  float max_radius = GetScreenHeight() / 4.f;
  Vector2 forward = {sin(ship->ang * PI / 180), -cos(ship->ang * PI / 180)};
  float power_percent = Clamp((Vector2Length(toMouse) / max_radius), 0, 1);
  Vector2 accel = Vector2Rotate(Vector2Scale(dir, ship->acc * power_percent),
                                Vector2Angle((Vector2){1., 0}, forward));
  ship->vel = Vector2Add(ship->vel, Vector2Scale(accel, dt));
  float damping = 1;
  if (Vector2Length(dir) == 0) {
    damping *= 2;
    if (Vector2Length(ship->vel) < 20) ship->vel = (Vector2){ 0 };
  }

  ship->vel = Vector2Scale(ship->vel, 1.f - damping * dt);
  ship->pos = Vector2Add(ship->pos, Vector2Scale(ship->vel, dt));
}

void draw_ship(ship_t* ship) {
  Vector2 nose  = { ship->pos.x,       ship->pos.y - 20 };
  Vector2 left  = { ship->pos.x - 12,  ship->pos.y + 12 };
  Vector2 right = { ship->pos.x + 12,  ship->pos.y + 12 };

  nose  = RotatePoint(nose,  ship->pos, ship->ang);
  left  = RotatePoint(left,  ship->pos, ship->ang);
  right = RotatePoint(right, ship->pos, ship->ang);

  DrawTriangle(nose, left, right, WHITE);
}

/*
 * deallocates a ship
 */

void destroy_ship(ship_t *ship) {
  free(ship);
}
