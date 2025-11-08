#pragma once
#include <Components/Components.h>
#include <Entity.h>

class CollisionSystem {
public:
  CollisionInfo checkCollisions(Entity &entity1, Entity &entity2) {
    CollisionInfo collisionInfo;
    if (!entity1.collider || !entity1.transform)
      return collisionInfo;
    if (!entity2.collider || !entity2.transform)
      return collisionInfo;

    auto &collider1 = entity1.collider;
    auto &collider2 = entity2.collider;

    auto transform1 = entity1.transform;
    auto transform2 = entity2.transform;

    auto collider1Bounds = collider1->getBounds(*transform1);
    auto collider2Bounds = collider2->getBounds(*transform2);

    if (auto collision = collider1Bounds.findIntersection(collider2Bounds)) {
      collisionInfo.isCollided = true;
      collisionInfo.x = collision.value().position.x;
      collisionInfo.y = collision.value().position.y;

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
  void ResolveCollisions(Entity &entity1, Entity &entity2, CollisionInfo info) {
    if (!entity1.collider || !entity1.transform)
      return;
    if (!entity2.collider || !entity2.transform)
      return;

    auto &col1 = entity1.collider;
    auto &col2 = entity2.collider;

    auto &transform1 = entity1.transform;
    auto &transform2 = entity2.transform;
    auto vel1 = entity1.velocity ? &entity1.velocity.value() : nullptr;
    auto vel2 = entity2.velocity ? &entity2.velocity.value() : nullptr;

    if (!info.isCollided)
      return;
    float moveRatio1, moveRatio2;
    if (col1->isStatic) {
      moveRatio1 = 0.f;
      moveRatio2 = 1.f;
    } else if (col2->isStatic) {
      moveRatio1 = 1.f;
      moveRatio2 = 0.f;
    } else {
      moveRatio1 = 0.5f;
      moveRatio2 = 0.5f;
    }
    float separationOffset = .3f;
    float separationDistance = info.overlay + separationOffset;
    transform1->x += info.normal.x * separationDistance * moveRatio1;
    transform1->y += info.normal.y * separationDistance * moveRatio1;

    transform2->x += -info.normal.x * separationDistance * moveRatio2;
    transform2->y += -info.normal.y * separationDistance * moveRatio2;

    float relativeVelX = (vel1 ? vel1->x : 0.f) - (vel2 ? vel2->x : 0.f);
    float relativeVelY = (vel1 ? vel1->y : 0.f) - (vel2 ? vel2->y : 0.f);

    float velAlongNormal =
        info.normal.x * relativeVelX + info.normal.y * relativeVelY;
    if (velAlongNormal < 0) {
      if (!col1->isStatic) {
        if (info.normal.y != 0) {
          vel1->y = 0.f;
        }
        if (info.normal.x != 0) {
          vel1->x = 0.f;
        }
      }
      if (!col2->isStatic) {
        if (info.normal.y != 0) {
          vel2->y = 0.f;
        }
        if (info.normal.x != 0) {
          vel2->x = 0.f;
        }
      }
    }
  }
};
