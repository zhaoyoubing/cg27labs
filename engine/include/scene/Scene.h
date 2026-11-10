#pragma once

#include "entity/EcsWorldRegistry.h"
#include "scene/AABB.h"
#include "scene/SceneNode.h"
#include "entity/LightComp.h"
#include "entity/CameraComp.h"
#include "render/CubeMap.h"

#include <string>
#include <memory>

struct Scene {
    std::string name_;
    ECSWorldRegistry ecsWorld_;

    CubeMap skybox_;

    std::vector<LightComp> lightList;

    Scene(const std::string& name = "default") : name_ (name) {
       root_ = std::make_unique<SceneNode>(); 
    }
    
    ~Scene() = default;

    //void init();
    //void update(float deltaTime);
    AABB getBBox() {
        return root_->getBBox();
    }

    std::unique_ptr<SceneNode> root_;

    CameraComp getMainCamera() {
        auto viewCam = ecsWorld_.view<CameraComp>();
        CameraComp camera;

        // a default camera in case no primary camera is found
        for (auto entity : viewCam)
        {
            auto & cam = ecsWorld_.getComp<CameraComp>(entity);
            if (cam.isPrimary) {
                camera = cam;
                break;
            }
        }

        return camera;
    }

    std::vector<LightComp> getLights() {
        std::vector<LightComp> lights;

        auto viewLight = ecsWorld_.view<LightComp>();
        for (auto entity : viewLight)
        {
            auto & light = ecsWorld_.getComp<LightComp>(entity);
            lights.push_back(light);
        }

        return lights;
    }
};