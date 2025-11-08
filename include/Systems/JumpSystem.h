#pragma once
#include <Components/Components.h>
#include <Entity.h>
#include <Systems/InputSystem.h>
class JumpSystem {
public:
  void update(Entity &entity, const InputSystem &input, float dt) {
    if (!entity.jump || !entity.collider || !entity.transform ||
        !entity.velocity || !entity.grounded)
      return;
    if(!entity.jump.value().canJump){
      entity.jump->timer -= dt;
      if(entity.jump->timer <= 0){
        entity.jump->canJump = true;
      }
    }
    bool grounded = entity.grounded.value().isGrounded;
    if(grounded){
      entity.jump->coyoteTimer = entity.jump->maxCoyoteTime;
    }
    else{
      entity.jump->coyoteTimer -= dt;
    }
  
    if (input.jumpPressed() && entity.jump->coyoteTimer > 0.f&& entity.jump.value().canJump) {
      entity.velocity->y -= entity.jump->jumpForce;
      entity.jump->timer = entity.jump.value().jumpCooldown;
    }
  }
};
