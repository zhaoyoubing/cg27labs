// include/systems/CameraSystem.h
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output


#include <glm/glm.hpp>

#include "ui/InputState.h"
#include "entity/entity.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"



class CameraSystem {
public:
    void update(ECSWorldPool& world, InputState&  input, float deltaTime) {
        auto entities = world.View<TransformComp, CameraComp>();

        for (EntityID id : entities) {

            auto& transform = world.GetComp<TransformComp>(id);
            auto& camera = world.GetComp<CameraComp>(id);

            
            // left button pressed
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

            if (input.isKeyHeld(GLFW_KEY_W)) transform.pos += camera.front * velocity;
            if (input.isKeyHeld(GLFW_KEY_S)) transform.pos -= camera.front * velocity;
            if (input.isKeyHeld(GLFW_KEY_A)) transform.pos -= right * velocity;
            if (input.isKeyHeld(GLFW_KEY_D)) transform.pos += right * velocity;
        }
    }
};