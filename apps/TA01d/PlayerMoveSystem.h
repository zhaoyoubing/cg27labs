#pragma once

#include <GLFW/glfw3.h>
#include "entity/Entity.h"
#include "entity/TransformComp.h"
#include "ui/InputState.h"

class PlayerMoveSystem {
public:
    void Update(ECSWorldPool& world, EntityID id, float deltaTime) {

        if (!world.HasComp<TransformComp>(id)) return;

        auto& trans = world.GetComp<TransformComp>(id);

        float deltaPos = 0.05f * deltaTime;
        float deltaAngle = 5 * deltaTime;

        if (InputState::IsKeyHeld(GLFW_KEY_LEFT)) {
            trans.rot.y -= deltaAngle;
        } else (InputState::IsKeyHeld(GLFW_KEY_RIGHT)) {
            trans.rot.y += deltaAngle;
        } if (InputState::IsKeyHeld(GLFW_KEY_DOWN)) {
            trans.rot.x += deltaAngle;
        } else (InputState::IsKeyHeld(GLFW_KEY_UP)) {
            trans.rot.x -= deltaAngle;
        }

        if (InputState::IsKeyHeld(GLFW_KEY_A)) {
            trans.pos.x -= deltaPos;
        } else if (InputState::IsKeyHeld(GLFW_KEY_D)) {
            trans.pos.x += deltaPos;
        } else if (InputState::IsKeyHeld(GLFW_KEY_W)) {
            trans.pos.z += deltaPos;
        } else if (InputState::IsKeyHeld(GLFW_KEY_S)) {
            trans.pos.z -= deltaPos;
        } else if (InputState::IsKeyHeld(GLFW_KEY_E)) {
            trans.pos.y += deltaPos;
        } else if (InputState::IsKeyHeld(GLFW_KEY_Q)) {
            trans.pos.y -= deltaPos;
        }
    }

};