#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <Systems/InputSystem.h>
class DashSystem {
public:
  void update(Entity &entity, const InputSystem &input, float dt) {
    if (!entity.dash || !entity.velocity)
      return;
    auto &dash = *entity.dash;
    auto &vel = *entity.velocity;
    if(dash.cooldownTimer > 0.f){
      dash.cooldownTimer -=dt;
    }

    if (input.isKeyPressed(sf::Keyboard::Key::LShift) && !dash.isDashing) {

      dash.isDashing = true;
      dash.dashTimer = dash.dashDuration;
      dash.dashDirection =
          (vel.x > 0.f) ? sf::Vector2f{1.f, 0.f} : sf::Vector2f{-1.f, 0.f};
    }

    if (dash.isDashing) {
      dash.dashTimer -= dt;
      vel.x = dash.dashDirection.x * dash.dashForce;
      if (dash.dashTimer <= 0.f) {
        dash.isDashing = false;
      }
    }
  }
};
