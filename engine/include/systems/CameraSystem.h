// include/systems/CameraSystem.h
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h> // For colored console output


#include <glm/glm.hpp>

#include "ui/InputState.h"
#include "entity/EcsWorldRegistry.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"
#include "render/CameraView.h"



class CameraSystem {
public:
    void tick(ECSWorldRegistry & world, InputState&  input, float deltaTime) {
        auto entities = world.view<CameraComp>();

        //spdlog::trace("Camera System tick");

        for (EntityID id : entities) {

            //auto& transform = world.getComp<TransformComp>(id);
            auto& camera = world.getComp<CameraComp>(id);

            if (camera.isPrimary == false) {
                continue; // skip non-primary cameras
            }

            // window resize
            if (input.bWinResized) {
                spdlog::debug("window resizing: {} {}", input.winWidth, input.winHeight);
                
                camera.viewport.w = input.winWidth;
                camera.viewport.h = input.winHeight;
                input.bWinResized = false;
                camera.isDirty = true;
            }

            // a temporary workaround
            // calculate the front vector after camera init
            camera.updateFrontFromYawPitch();

            // 2. Process Keyboard Movement
            float velocity = 5.0f * deltaTime;
            glm::vec3 right = glm::normalize(glm::cross(camera.front, camera.up));

            if (input.isKeyHeld(GLFW_KEY_W)) {
                spdlog::trace("W pressed");
                camera.eye += camera.front * velocity;
                camera.isDirty = true;
            }
            if (input.isKeyHeld(GLFW_KEY_S)) {
                camera.eye -= camera.front * velocity;
                camera.isDirty = true;
            }

            if (input.isKeyHeld(GLFW_KEY_A)) {
                camera.eye -= right * velocity;
                camera.isDirty = true;
            }

            if (input.isKeyHeld(GLFW_KEY_D)) {
                camera.eye += right * velocity;
                camera.isDirty = true;
            }

            if (input.isKeyHeld(GLFW_KEY_E)) {
                camera.eye += camera.up * velocity;
                camera.isDirty = true;
            }

            if (input.isKeyHeld(GLFW_KEY_Q)) {
                camera.eye -= camera.up * velocity;
                camera.isDirty = true;
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

            
            spdlog::trace("Camera Yaw Pitch: yaw = {}, pitch ={}", camera.yaw, camera.pitch);

            if (camera.pitch > 89.0f)  camera.pitch = 89.0f;
            if (camera.pitch < -89.0f) camera.pitch = -89.0f;

            spdlog::trace("Camera Eye: x = {}, y = {}, z = {}, fov = {}", camera.eye.x, camera.eye.y, camera.eye.z, camera.fov);
            spdlog::trace("Camera front: x = {}, y = {}, z = {}", camera.front.x, camera.front.y, camera.front.z);

            camera.isDirty = true; // Mark camera as dirty to recalculate matrices
            camera.updateFrontFromYawPitch();

            break;
        }
    }
};