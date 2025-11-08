#pragma once
#include <SFML/Graphics.hpp>

struct Transform {
  float x, y;
  float rotation = 0.0f;
  Transform(float x = 0.f, float y = 0.f) : x(x), y(y) {}
};
struct Velocity {
  float x, y;
  Velocity(float x = 0.f, float y = 0.f) : x(x), y(y) {}
};
struct Movement {
  float maxSpeed;
  float acceleration;
  float friction = 15.f;
  Movement(float maxSpeed = 500.f, float acceleration = 3000.f)
      : maxSpeed(maxSpeed), acceleration(acceleration) {}
};
struct JumpComponent {
  float jumpForce;
  float jumpCooldown;
  float maxJumps;
  float usedJumps;

  bool canJump = true;
  float timer = 0.f;

  float coyoteTimer;
  float maxCoyoteTime;

  JumpComponent(float jumpForce = 450.f, float jumpCooldown = 0.3f,
                float maxJumps = 2)
      : jumpForce(jumpForce), jumpCooldown(jumpCooldown), maxJumps(maxJumps),
        usedJumps(0), coyoteTimer(0.f), maxCoyoteTime(0.03f) {}
  void resetJumps() { usedJumps = 0; }
};
struct Dash {
  float dashForce;
  float dashDuration;
  bool canDash = true;
  bool isDashing = false;
  float dashTimer = 0.f;
  float cooldownTimer = 0.f;
  float dashCooldown = .5f;
  sf::Vector2f dashDirection{0.f, 0.f};
  Dash(float force = 700.f, float duration = 0.12f)
      : dashForce(force), dashDuration(duration) {}
};
struct Health {
  float current, max;
  Health(float current = 100.f, float max = 100.f)
      : current(current), max(max) {}
};
struct Shape {
  sf::RectangleShape shape;
  Shape(sf::Color color = sf::Color::Black, float x = 10.f, float y = 50.f) {
    shape.setOrigin({x / 2.f, y / 2.f});
    shape.setSize({x, y});
    shape.setFillColor(color);
  }
};
struct Collider {
  float width, height;
  bool isStatic;
  bool isTrigger;
  Collider(float width = 50.f, float height = 50.f, bool isStatic = false,
           bool isTrigger = false)
      : width(width), height(height), isStatic(isStatic), isTrigger(isTrigger) {
  }
  sf::FloatRect getBounds(const Transform &transform) {
    return sf::FloatRect(
        {transform.x - width / 2.f, transform.y - height / 2.f},
        {width, height});
  }
};
struct CollisionInfo {
  float x, y;
  bool isCollided = false;
  sf::Vector2f normal;
  float overlay;
};
struct Grounded {
  bool isGrounded = false;
};

enum class EntityLayer { Default, Player, Ground };
struct Tag {
  EntityLayer tag = EntityLayer::Default;
  Tag(EntityLayer layer) : tag(layer) {}
};
