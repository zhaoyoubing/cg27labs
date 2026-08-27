#pragma once

#include <memory>
//#include "MeshGeometry.hpp" // Your VAO/VBO container

class MeshGeoFactory {
public:
    // Generate an axis-aligned box (width, height, depth)
    static std::shared_ptr<MeshGeometry> CreateBox(float width, float height, float depth);

    // Convenience wrapper: a Box where width == height == depth
    static std::shared_ptr<MeshGeometry> CreateCube(float size) {
        return CreateBox(size, size, size);
    }

    // Generate a smooth sphere
    static std::shared_ptr<MeshGeometry> CreateSphere(float radius = 1.0f, unsigned int sectors = 36, unsigned int stacks = 18) {

    }

    // Generate a flat 2D plane (useful for floors/walls)
    static std::shared_ptr<MeshGeometry> CreatePlane(float width = 1.0f, float height = 1.0f) {
        
    }
};