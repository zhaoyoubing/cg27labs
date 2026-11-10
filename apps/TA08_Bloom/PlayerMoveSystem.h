#pragma once

#include <GLFW/glfw3.h>
#include "entity/EcsTypes.h"
#include "entity/EcsWorldRegistry.h"
#include "entity/TransformComp.h"
#include "ui/InputState.h"

class PlayerMoveSystem {
public:
    void tick(ECSWorldRegistry & world,  EntityID id, InputState & input, float deltaTime) {

        if (!world.hasComp<TransformComp>(id)) return;

        auto& trans = world.getComp<TransformComp>(id);

        float deltaPos = 0.1f * deltaTime;
        float deltaAngle = 50.0f * deltaTime;

        if (input.isKeyHeld(GLFW_KEY_LEFT)) {
            trans.rot.y -= deltaAngle;
        } else if (input.isKeyHeld(GLFW_KEY_RIGHT)) {
            trans.rot.y += deltaAngle;
        } else if (input.isKeyHeld(GLFW_KEY_DOWN)) {
            trans.rot.x += deltaAngle;
        } else if (input.isKeyHeld(GLFW_KEY_UP)) {
            trans.rot.x -= deltaAngle;
        }

        if (input.isKeyHeld(GLFW_KEY_A)) {
            trans.pos.x -= deltaPos;
        } else if (input.isKeyHeld(GLFW_KEY_D)) {
            trans.pos.x += deltaPos;
        } else if (input.isKeyHeld(GLFW_KEY_W)) {
            trans.pos.z += deltaPos;
        } else if (input.isKeyHeld(GLFW_KEY_S)) {
            trans.pos.z -= deltaPos;
        } else if (input.isKeyHeld(GLFW_KEY_E)) {
            trans.pos.y += deltaPos;
        } else if (input.isKeyHeld(GLFW_KEY_Q)) {
            trans.pos.y -= deltaPos;
        }
    }

};