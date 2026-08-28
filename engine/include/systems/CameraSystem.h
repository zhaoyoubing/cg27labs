// include/systems/CameraSystem.h
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output


#include <glm/glm.hpp>

#include "ui/InputState.h"
#include "entity/EcsWorldRegistry.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"
<<<<<<< HEAD:engine/include/systems/CameraSystem.h
#include "render/CameraView.h"
=======
>>>>>>> cab3626 (rename ecs to entity for TA01):include/systems/CameraSystem.h



class CameraSystem {
public:
    void tick(ECSWorldRegistry & world, InputState&  input, float deltaTime) {
        auto entities = world.view<CameraComp>();

        for (EntityID id : entities) {

            //auto& transform = world.getComp<TransformComp>(id);
            auto& camera = world.getComp<CameraComp>(id);

            if (camera.isPrimary == false) {
                continue; // skip non-primary cameras
            }
            
            // left button pressed
            // comment it if you want Unreal style camera control
            if (! input.isLeftButtonPressed()) {
                continue; 
            }

            // 1. Process Mouse Look
            double xOffset = input.getMouseOffsetX() * camera.sensitivity;
            double yOffset = input.getMouseOffsetY() * camera.sensitivity;

            spdlog::trace("Mouse offsets: x = {}, y ={}", xOffset, yOffset);
            
            camera.yaw += static_cast<float>(xOffset);
            camera.pitch += static_cast<float>(yOffset);

            if (camera.pitch > 89.0f)  camera.pitch = 89.0f;
            if (camera.pitch < -89.0f) camera.pitch = -89.0f;

            camera.front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            camera.front.y = sin(glm::radians(camera.pitch));
            camera.front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
            camera.front = glm::normalize(camera.front);

            // 2. Process Keyboard Movement
            float velocity = 5.0f * deltaTime;
            glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));

            if (input.isKeyHeld(GLFW_KEY_W)) camera.eye += camera.front * velocity;
            if (input.isKeyHeld(GLFW_KEY_S)) camera.eye -= camera.front * velocity;
            if (input.isKeyHeld(GLFW_KEY_A)) camera.eye -= right * velocity;
            if (input.isKeyHeld(GLFW_KEY_D)) camera.eye += right * velocity;

            camera.isDirty = true; // Mark camera as dirty to recalculate matrices

            break;
        }
    }
};