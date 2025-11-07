#pragma once
#include <SFML/Graphics.hpp>

class InputSystem {
private:
  std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;
  std::unordered_map<sf::Keyboard::Key, bool> previousKeyState;

public:
  void update() {
    previousKeyState = currentKeyState;
    for (auto &[key, state] : currentKeyState) {
      state = sf::Keyboard::isKeyPressed(key);
    }
  }
  void trackKey(sf::Keyboard::Key key) {
    if (currentKeyState.find(key) == currentKeyState.end()) {
      currentKeyState[key] = false;
      previousKeyState[key] = false;
    }
  }
  bool isKeyHeld(sf::Keyboard::Key key) {
    auto it = currentKeyState.find(key);
    if (it != currentKeyState.end() && it->second) {
    }
    return it != currentKeyState.end() && it->second;
  }
  bool isKeyPressed(sf::Keyboard::Key key) {
    auto current = currentKeyState.find(key);
    auto previous = currentKeyState.find(key);

    if (current == currentKeyState.end())
      return false;

    bool currentlyPressed = current->second;
    bool previouslyPressed =
        (previous != previousKeyState.end()) && previous->second;
    return currentlyPressed && previouslyPressed;
  }
  bool isKeyReleased(sf::Keyboard::Key key) {
    auto current = currentKeyState.find(key);
    auto previous = currentKeyState.find(key);

    if (previous == previousKeyState.end())
      return false;

    bool currentlyPressed = current->second;
    bool previouslyPressed =
        (previous != previousKeyState.end()) && previous->second;
    return !currentlyPressed && previouslyPressed;
  }
  float getAxisX() {
    float axis = 0.f;
    if (isKeyHeld(sf::Keyboard::Key::A) ||
        isKeyHeld(sf::Keyboard::Key::Right)) {
      axis += 1.f;
    }
    if (isKeyHeld(sf::Keyboard::Key::D) || isKeyHeld(sf::Keyboard::Key::Left)) {
      axis -= 1.f;
    }
    return axis;
  }
  float getAxisY() {
    float axis = 0.f;
    if (isKeyHeld(sf::Keyboard::Key::W) || isKeyHeld(sf::Keyboard::Key::Up)) {
      axis -= 1.f;
    }
    return axis;
  }
  bool jumpPressed() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
  }
};
