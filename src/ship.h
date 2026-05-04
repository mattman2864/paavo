#pragma once

#include <raylib.h>

#define MAX_ANGULAR_VEL 270

typedef struct {
  // position values
  Vector2 pos;
  float ang;

  // velocity values
  Vector2 vel;
  float ang_vel;

  // acceleration values
  float acc;
  float ang_acc;
} ship_t;

ship_t* create_ship(Vector2 position, float acceleration,
                    float angular_acceleration);
void move_ship(ship_t* ship, Vector2 mousePosition, Vector2 dir, float dt);
void draw_ship(ship_t* ship);
void destroy_ship(ship_t* ship);
