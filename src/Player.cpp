#include "Player.h"
#include "Input.h"
#include <raylib.h>

Player::Player() :
  input(DEFAULT_KB),
  sprite("assets/entities/player/player.png")
{
  sprite.showHitbox = true;
}

void Player::update(float dt) {
  input.update(this);
  body.update(dt);
  sprite.update(dt);
}

void Player::draw() {
  sprite.draw(this, new (Camera2D) {
      .offset = {(float)GetScreenWidth()/2, (float)GetScreenHeight()/2},
      .target = {this->body.getPosition().x, this->body.getPosition().y},
      .rotation = 0,
      .zoom = 1.f,
      });
}

Body* Player::getBody() {
  return &body;
}
