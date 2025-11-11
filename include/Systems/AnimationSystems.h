#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <SFML/Graphics.hpp>
#include <Systems/InputSystem.h>
class AnimationSystem {
public:
  void update(Entity &entity, float dt) {
    if (!entity.sprite || !entity.animation) {
      return;
    }
    auto &sprite = entity.sprite.value();
    auto &anim = entity.animation.value();

    if (anim.animations.find(anim.currentAnimation) == anim.animations.end()) {
      return;
    }
    AnimationData &currentAnim = anim.animations[anim.currentAnimation];
    anim.frameTimer += dt;
    if (anim.frameTimer >= currentAnim.frameTime) {
      anim.frameTimer = 0.f;
      anim.currentFrame++;
    }
    if (anim.currentFrame >= currentAnim.frameCount) {
      anim.currentFrame = 0;
      if (anim.isLocked && (currentAnim.isLockable)) {
        anim.isLocked = false;
      }
    }
    sf::IntRect frameRect(
        {(int)currentAnim.startX +
             (anim.currentFrame * (int)currentAnim.frameWidth),
         currentAnim.startY},
        {(int)currentAnim.frameWidth, (int)currentAnim.frameHeight});
    sprite.sprite.setTextureRect(frameRect);
    sprite.rect = frameRect;
  }
  static void changeAnimation(Animation &anim, const std::string &newAnim) {
    if (anim.isLocked)
      return;
    if (anim.currentAnimation != newAnim) {
      anim.currentAnimation = newAnim;
      anim.currentFrame = 0;
      anim.frameTimer = 0.0f;
    }
    if (anim.animations[newAnim].isLockable  )
      anim.isLocked = true;
  }
};
