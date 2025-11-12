#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <Systems/InputSystem.h>
#include <iostream>
class DashSystem {
public:
  void update(Entity &entity, const InputSystem &input, float dt) {
    if (!entity.dash || !entity.velocity)
      return;
    auto &dash = *entity.dash;
    auto &vel = *entity.velocity;

    if (input.isKeyHeld(sf::Keyboard::Key::LShift) && !dash.isDashing &&
        dash.cooldownTimer <= 0.f) {

      dash.dashTimer = dash.dashDuration;
      if (input.isKeyPressed(sf::Keyboard::Key::A) ||
          input.isKeyPressed(sf::Keyboard::Key::Left)) {
        dash.dashDirection.x = -1.f;
        dash.isDashing = true;
      }
      if (input.isKeyPressed(sf::Keyboard::Key::D) ||
          input.isKeyPressed(sf::Keyboard::Key::Right)) {
        dash.dashDirection.x = 1.f;
        dash.isDashing = true;
      }
    }

    if (dash.isDashing) {
      std::cout << "DASHING" << vel.x << "\n";
      dash.dashTimer -= dt;
      vel.x = dash.dashDirection.x * dash.dashForce;
      if (dash.dashTimer <= 0.f) {
        dash.isDashing = false;
        dash.cooldownTimer = dash.dashCooldown;
      }
    }
    if (dash.cooldownTimer > 0.f) {
      dash.cooldownTimer -= dt;
    }
  }
};
