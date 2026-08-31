#pragma once

#include <raylib.h>

typedef struct {
  Vector2 dir;
  // dash, other movement controls added here
} Control;

typedef enum {
  CONTROL_PLAYER_INPUT,
  CONTROL_FOLLOW_TARGET
  // ai controls here
} ControlType;

typedef struct {
  ControlType type;
  union {
    // ai movement here, for example:
    struct { Vector2 target; } followTarget;
  } data;
} ControlSource;

typedef struct {
  float speed;
  float acc;
  Vector2 vel;
} Movement;

Control get_control();
