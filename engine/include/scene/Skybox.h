#pragma once

#include <memory>

class MeshGeometry;
class Texture;
class Shader;
class Camera;

class Skybox
{
public:
    Skybox(
        std::shared_ptr<MeshGeometry> geometry,
        std::shared_ptr<Texture> cubemap);

    void render(
        const Camera& camera,
        const Shader& shader) const;

private:
    std::shared_ptr<MeshGeometry> geometry;
    std::shared_ptr<Texture> cubemap;
};