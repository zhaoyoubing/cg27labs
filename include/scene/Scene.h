#pragma once
#include <entity/EcsWorldRegistry.h>
#include <string>

class Scene {
public:
    Scene(const std::string& name);
    ~Scene() = default;

    void init();
    void update(float deltaTime);

    EcsWorldRegistry & getRegistry() { return registry; }

private:
    std::string name;
    EcsWorldRegistry registry;
};