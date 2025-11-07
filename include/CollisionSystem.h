#pragma once

#include <Components.h>

class CollisionSystem {
public:
  CollisionInfo checkCollisions(Collider &collider1, Transform &transform1,
                                Collider &collider2, Transform &transform2) {
    auto collider1Bounds = collider1.getBounds(transform1);
    auto collider2Bounds = collider2.getBounds(transform2);
    CollisionInfo collisionInfo;
    if (auto collision = collider1Bounds.findIntersection(collider2Bounds)) {
      collisionInfo.isCollided = true;
      collisionInfo.x = collision->position.x;
      collisionInfo.y = collision->position.y;

      float left = (collider1Bounds.position.x + collider1Bounds.size.x) -
                   collider2Bounds.position.x;
      float right = (collider2Bounds.position.x + collider2Bounds.size.x) -
                    collider1Bounds.position.x;
      float top = (collider1Bounds.position.y + collider1Bounds.size.y) -
                  collider2Bounds.position.y;
      float bottom = (collider2Bounds.position.y + collider2Bounds.size.y) -
                     collider1Bounds.position.y;

      float minPenetration = std::min({left, right, top, bottom});
      collisionInfo.overlay = minPenetration;

      if (minPenetration == left) {
        collisionInfo.normal = sf::Vector2f(-1, 0); // Push left
      } else if (minPenetration == right) {
        collisionInfo.normal = sf::Vector2f(1, 0); // Push right
      } else if (minPenetration == top) {
        collisionInfo.normal = sf::Vector2f(0, -1); // Push up
      } else {
        collisionInfo.normal = sf::Vector2f(0, 1); // Push down
      }
    }
    return collisionInfo;
  }
  void ResolveCollisions(Transform &transform1, Velocity &vel1, Collider &col1,
                         Transform &transform2, Velocity &vel2, Collider &col2,
                         CollisionInfo info) {
    if (!info.isCollided)
      return;
    float moveRatio1, moveRatio2;
    if (col1.isStatic) {
      moveRatio1 = 0.f;
      moveRatio2 = 1.f;
    } else if (col2.isStatic) {
      moveRatio1 = 1.f;
      moveRatio2 = 0.f;
    } else {
      moveRatio1 = 0.5f;
      moveRatio2 = 0.5f;
    }
    float separationOffset = 1.f;
    float separationDistance = info.overlay + separationOffset;
    transform1.translate(-info.normal.x * separationDistance * moveRatio1,
                         -info.normal.y * separationDistance * moveRatio1);
    transform2.translate(info.normal.x * separationDistance * moveRatio2,
                         info.normal.y * separationDistance * moveRatio2);

    float relativeVelX = vel1.toVector().x - vel2.toVector().x;
    float relativeVelY = vel1.toVector().y - vel2.toVector().y;

    float velAlongNormal =
        info.normal.x * relativeVelX + info.normal.y * relativeVelY;
    if (velAlongNormal < 0) {
      if (!col1.isStatic) {
        if (info.normal.y != 0) {
          vel1.y = 0.f;
        }
        if (info.normal.x != 0) {
          vel1.x = 0.f;
        }
      }
      if (!col2.isStatic) {
        if (info.normal.y != 0) {
          vel2.y = 0.f;
        }
        if (info.normal.x != 0) {
          vel2.x = 0.f;
        }
      }
    }
  }
};
