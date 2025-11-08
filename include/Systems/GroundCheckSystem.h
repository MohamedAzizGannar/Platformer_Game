#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <SFML/Graphics.hpp>
#include <Systems/InputSystem.h>

class GroundCheckSystem {
public:
  bool isGrounded(Entity &entity1, Entity &entity2) {
    if (!entity1.collider || !entity1.transform)
      return false;
    if (!entity2.collider || !entity2.transform)
      return false;

    auto &col1 = entity1.collider;
    auto &col2 = entity2.collider;
    auto &transform1 = entity1.transform;
    auto &transform2 = entity2.transform;
    auto cBounds = col1->getBounds(*transform1);
    auto pBounds = col2->getBounds(*transform2);

    float cBottom = cBounds.position.y + cBounds.size.y;
    float pTop = pBounds.position.y;
    float distance = pTop - cBottom; // vertical distance above platform
    const float tolerance =
        5.f; // increase this if gravity moves character faster

    // check vertical proximity
    bool verticalCheck = distance >= 0.f && distance <= tolerance;

    // check horizontal overlap (full width)
    bool horizontalCheck =
        cBounds.position.x + cBounds.size.x > pBounds.position.x &&
        cBounds.position.x < pBounds.position.x + pBounds.size.x;

    return verticalCheck && horizontalCheck;
  }
  void update(std::vector<Entity> &entities) {
    for (auto &entity : entities) {
      if (!entity.grounded || !entity.collider || !entity.transform ||
          !entity.tag)
        continue;

      bool onGround = false;
      for (auto &other : entities) {
        if (entity.id == other.id || !other.collider || !other.transform ||
            !other.tag)
          continue;

        if (other.tag->tag != EntityLayer::Ground)
          continue;
        if (isGrounded(entity, other)) {
          onGround = true;
          break;
        }
      }
      entity.grounded->isGrounded = onGround;
    }
  }
};
