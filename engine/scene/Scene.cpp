#include "Scene.h"

#include "entity/TransformComp.h"
#include "entity/MeshComp.h"
#include "Utils/ModelLoader.h"

Scene::Scene(const std::string& name) : name(name) {}

void Scene::init() {
    // 1. Create a camera entity
    auto cameraEntity = registry.create();
    // registry.emplace<CameraComponent>(cameraEntity, ...);

    // 2. Load a 3D model and populate entities/components
    ModelLoader::loadModel(registry, "assets/models/nanosuit.gltf");
}