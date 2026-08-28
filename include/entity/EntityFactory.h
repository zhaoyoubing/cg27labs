<<<<<<<< HEAD:engine/include/entity/EntityFactory.h
// engine/include/entity/EntityFactory.h
========
// include/entity/EntityFactory.h
>>>>>>>> cab3626 (rename ecs to entity for TA01):include/entity/EntityFactory.h
#pragma once
#include "entity/EcsTypes.h"
#include "entity/EcsWorldRegistry.h"
#include "entity/TransformComp.h"
#include "entity/CameraComp.h"


#include <glm/glm.hpp>

class EntityFactory {
public:
    static EntityID createCamera(ECSWorldRegistry & world, 
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f)) {
        // 1. Create a raw entity handle from your ECS world pool
        EntityID cam = world.CreateEntity();

        // 2. Add the TransformComponent to define WHERE the camera is in the world
        world.addComp<TransformComponent>(cam, TransformComp{
            .pos = pos,
            .rot = glm::vec3(0.0f),
            .scale = glm::vec3(1.0f)
        });

        // 3. Add the CameraComponent to define HOW it views the world (FOV, planes, angles)
        world.addComp<CameraComponent>(cam, CameraComp{
            .fov = 45.0f,
            .near = 0.1f,
            .far = 100.0f,
            .yaw = -90.0f, // Looking down the negative Z-axis by default
            .pitch = 0.0f,
            .sensitivity = 0.1f
        });

        return cameraEntity;
    }
};