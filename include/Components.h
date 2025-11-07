#pragma once
#include <SFML/Graphics.hpp>

struct Transform {

  float x, y;
  float rotation = 0.0f;

  Transform(float x = 0, float y = 0) : x(x), y(y) {}

  void translate(float dx, float dy) {
    x += dx;
    y += dy;
  }

  sf::Vector2f toVector() { return sf::Vector2f(x, y); }

  float distanceToTarget(const Transform &other) {
    float dx = other.x - x;
    float dy = other.y - y;
    return std::sqrt(dx * dx + dy * dy);
  }

  void lookAtTarget(const Transform &other) {
    float dx = other.x - x;
    float dy = other.y - y;
    rotation = std::atan2(dy, dx);
  }
};
struct Velocity {
  float x, y;
  Velocity(float x = 0, float y = 0) : x(x), y(y) {}

  sf::Vector2f toVector() { return sf::Vector2f(x, y); }
  void accelerate(float ax, float ay) {
    x += ax;
    y += ay;
  }
  void setVelocity(float nx, float ny) {
    x = nx;
    y = ny;
  }
	float getMagnitude(){return std::sqrt(x*x + y*y);}
	void normalize(){
		float magnitude = getMagnitude();
		if(magnitude > 0) {x /= magnitude; y /= magnitude;}
	}
};
struct Health {
  float current, max;
  Health(float hp) : current(hp), max(hp) {}
  void takeDamage(float amount) { current -= amount; }
  void heal(float amount) { current += amount; }
  void upgradeHealth(float amount) { max += amount; }
  float getPercentage() { return (float)(current / max) * 100.f; }
};
struct Shape {
  sf::CircleShape shape;
  float radius ;
	Shape(sf::Color color, float radius = 100.f){
		shape.setRadius(radius);
		shape.setFillColor(color);
		shape.setOrigin(sf::Vector2f(radius,radius));
	}
  void render(sf::RenderWindow &window){
    window.draw(shape);
  }
};
