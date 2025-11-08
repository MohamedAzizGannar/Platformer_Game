#pragma once
#include <SFML/Graphics.hpp>

class InputSystem {
private:
  std::unordered_map<sf::Keyboard::Key, bool> currentKeyState;
  std::unordered_map<sf::Keyboard::Key, bool> previousKeyState;

  std::unordered_map<sf::Keyboard::Key, sf::Time> lastTimePressed;
  sf::Clock clock;
  sf::Time doublePressThreshold = sf::milliseconds(300);

public:
  void update() {
    previousKeyState = currentKeyState;
    for (auto &[key, state] : currentKeyState) {
      state = sf::Keyboard::isKeyPressed(key);

      if (isKeyPressed(key)) {
        lastTimePressed[key] = clock.getElapsedTime();
      }
    }
  }
  void trackKey(sf::Keyboard::Key key) {
    if (currentKeyState.find(key) == currentKeyState.end()) {
      currentKeyState[key] = false;
      previousKeyState[key] = false;
    }
  }
  bool isKeyHeld(sf::Keyboard::Key key) const {
    auto it = currentKeyState.find(key);
    return it != currentKeyState.end() && it->second;
  }
  bool isKeyPressed(sf::Keyboard::Key key) const {
    auto current = currentKeyState.find(key);
    auto previous = previousKeyState.find(key);

    if (current == currentKeyState.end())
      return false;

    bool currentlyPressed = current->second;
    bool previouslyPressed =
        (previous != previousKeyState.end()) && previous->second;
    return currentlyPressed && !previouslyPressed;
  }
  bool isKeyReleased(sf::Keyboard::Key key) const {
    auto current = currentKeyState.find(key);
    auto previous = previousKeyState.find(key);

    if (previous == previousKeyState.end())
      return false;

    bool currentlyPressed = current->second;
    bool previouslyPressed =
        (previous != previousKeyState.end()) && previous->second;
    return !currentlyPressed && previouslyPressed;
  }
  bool isKeyDoublePressed(sf::Keyboard::Key key) {
    if (isKeyPressed(key)) {
      sf::Time now = clock.getElapsedTime();
      sf::Time last = lastTimePressed[key];
      if (last != sf::Time::Zero && (now - last) <= doublePressThreshold) {
        lastTimePressed[key] =
            sf::Time::Zero; // reset to avoid triple-detection
        return true;
      } else {
        lastTimePressed[key] =
            now; // first press or too slow → start new timing
      }
    }
    return false;
  }
  float getAxisX() const {
    float axis = 0.f;
    if (isKeyHeld(sf::Keyboard::Key::D) ||
        isKeyHeld(sf::Keyboard::Key::Right)) {
      axis += 1.f;
    }
    if (isKeyHeld(sf::Keyboard::Key::A) || isKeyHeld(sf::Keyboard::Key::Left)) {
      axis -= 1.f;
    }
    return axis;
  }
  float getAxisY() const {
    float axis = 0.f;
    if (isKeyHeld(sf::Keyboard::Key::W) || isKeyHeld(sf::Keyboard::Key::Up)) {
      axis -= 1.f;
    }
    return axis;
  }
  bool jumpPressed() const { return isKeyPressed(sf::Keyboard::Key::Space); }
};
