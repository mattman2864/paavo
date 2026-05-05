#pragma once

#include <raylib.h>
#include "ship.h"

typedef struct {
  Vector2 pos;
  Vector2 vel;
  ship_t* owner;
} projectile_t;
