// include/systems/CameraSystem.h
#pragma once

#include <glm/glm.hpp>

#include "ui/InputState.h"
#include "entity/entity.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"



class CameraSystem {
public:
    void Update(ECSWorldPool& world, float deltaTime) {
        auto entities = world.View<TransformComp, CameraComp>();

        for (EntityID id : entities) {
            auto& transform = world.GetComp<TransformComp>(id);
            auto& camera = world.GetComp<CameraComp>(id);

            // 1. Process Mouse Look
            double xOffset = InputState::GetMouseOffsetX() * camera.sensitivity;
            double yOffset = InputState::GetMouseOffsetY() * camera.sensitivity;

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

            if (InputState::IsKeyHeld(GLFW_KEY_W)) transform.pos += camera.front * velocity;
            if (InputState::IsKeyHeld(GLFW_KEY_S)) transform.pos -= camera.front * velocity;
            if (InputState::IsKeyHeld(GLFW_KEY_A)) transform.pos -= right * velocity;
            if (InputState::IsKeyHeld(GLFW_KEY_D)) transform.pos += right * velocity;
        }
    }
};