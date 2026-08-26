// include/entity/EntityFactory.hpp
#pragma once
#include "ecs/EcsTypes.h"
#include "ecs/components/TransformComp.h"
#include "ecs/components/CameraComp.hpp"


#include <glm/glm.hpp>

class EntityFactory {
public:
    static EntityID createCamera(ECSWorldPool& world, 
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f)) {
        // 1. Create a raw entity handle from your ECS world pool
        EntityID cam = world.CreateEntity();

        // 2. Add the TransformComponent to define WHERE the camera is in the world
        world.addComp<TransformComponent>(cam, TransformComp{
            .position = pos,
            .rotation = glm::vec3(0.0f),
            .scale = glm::vec3(1.0f)
        });

        // 3. Add the CameraComponent to define HOW it views the world (FOV, planes, angles)
        world.addComp<CameraComponent>(cam, CameraComp{
            .fov = 45.0f,
            .nearPlane = 0.1f,
            .farPlane = 100.0f,
            .yaw = -90.0f, // Looking down the negative Z-axis by default
            .pitch = 0.0f,
            //.sensitivity = 0.1f
        });

        return cameraEntity;
    }
};