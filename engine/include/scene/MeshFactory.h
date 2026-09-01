#pragma once

#include "geometry/PlaneMesh.h"
#include "geometry/SphereMesh.h"
#include "geometry/OpenCylinderMesh.h"

#include "GltfMeshLoader.h"

#include "Mesh.h" // Mesh without materials

#include <memory>
#include <vector>

class MeshFactory {
public:

    static std::shared_ptr<Mesh> loadGltf(std::string filepath, TextureManager& texMgr) {
        // Load a glTF model using the GltfMeshLoader
        std::shared_ptr<Mesh> mesh = GltfMeshLoader::loadModel(filepath, texMgr);
        return mesh;
    }

    // Generate an axis-aligned box (width, height, depth)
    static std::shared_ptr<Mesh> createBox(float width, float height, float depth) {
        auto model = std::make_shared<Mesh>();
        
        return model;
    }

    static std::shared_ptr<Mesh> createPyramid(float size, float height) {
        auto model = std::make_shared<Mesh>();

        float halfSize = size / 2.0;

        // not using normals and texture uvs
        std::vector<Vertex> vertices = {
            { { -halfSize,  halfSize, 0.f }, glm::vec3(0, 0, -1), glm::vec2(0, 0)  }, // v0
            { { -halfSize, -halfSize, 0.f }, glm::vec3(0, 0, -1), glm::vec2(0, 0)  },  // v1
            { {  halfSize, -halfSize, 0.f }, glm::vec3(0, 0, -1), glm::vec2(0, 0)  },  // v2
            { {  halfSize,  halfSize, 0.f }, glm::vec3(0, 0, -1), glm::vec2(0, 0)  },  // v3
            { {      0.0f,  0.0f, -height }, glm::vec3(0, 0, 1), glm::vec2(0, 0)   }  // v4
        };

        std::vector<glm::vec3> colours = {
            { 1.f, 0.f, 0.f },  // Red 
            { 0.f, 1.f, 0.f },  // Green  
            { 0.f, 1.f, 0.f },  // Blue  
            { 1.f, 1.f, 0.f },  // Yellow  
            { 1.f, 1.f, 1.f },  // White        
        };

        std::vector<uint32_t> indices = {
            // the original two triangles form the base
            0, 1, 2, 2, 3, 0,
            // four new triangles
            4, 1, 0,
            4, 2, 1,
            4, 3, 2,
            4, 0, 3
        };

        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        mesh->vertices = vertices;
        mesh->colours = colours;
        mesh->indices = indices;

        return mesh;
    }

    // Convenience wrapper: a Box where width == height == depth
    static std::shared_ptr<Mesh> createCube(float size) {
        return createBox(size, size, size);
    }

    // Generate a flat 2D plane (useful for floors/walls)
    static std::shared_ptr<Mesh> createPlane(float width = 1.0f, float height = 1.0f) {
        auto model = std::make_shared<Mesh>();
        
        return model;
    }

    static std::shared_ptr<Mesh> createOpenCylinder(
        float radius, float height, unsigned int rSegments, unsigned int hSegments)
    {
        std::shared_ptr<OpenCylinderMesh> cylinder =
                    std::make_shared<OpenCylinderMesh>(radius, height, rSegments, hSegments);

        return cylinder;
    }

    // Generate a smooth sphere
    static std::shared_ptr<Mesh> createSphere(float radius, unsigned int latSeg, unsigned int longSeg ) {
         std::shared_ptr<SphereMesh> sphere =
                    std::make_shared<SphereMesh>(radius, latSeg, longSeg);

        return sphere;
    }


};