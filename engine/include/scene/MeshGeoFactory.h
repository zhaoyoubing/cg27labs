#pragma once

#include "geometry/PlaneMesh.h"
#include "geometry/SphereMesh.h"
#include "geometry/OpenCylinderMesh.h"

#include "MeshGeometry.h" // Mesh without materials

#include <memory>

class MeshGeoFactory {
public:
    // Generate an axis-aligned box (width, height, depth)
    static std::shared_ptr<MeshGeometry> createBox(float width, float height, float depth) {
        auto model = std::make_shared<MeshGeometry>();
        
        return model;
    }

    static std::shared_ptr<MeshGeometry> createPyramid(float size, float height) {
        auto model = std::make_shared<MeshGeometry>();

        float halfSize = size / 2.0;
        // use normal as colour
        std::vector<Vertex> vertices = {
            { 
                .pos = {-halfSize, halfSize, 0.f },  // v0
                .normal = { 1.f, 0.f, 0.f }  // Red 
            },
            { 
                .pos = {-halfSize, -halfSize, 0.f},  // v1
                .normal = {0.f, 1.f, 0.f},   // Green       
            },
            { 
                .pos = { halfSize, -halfSize, 0.f},  // v2
                .normal = {0.f, 1.f, 0.f},   // Blue       
            },
            { 
                .pos = { halfSize, halfSize, 0.f },  // v3
                .normal = {1.f, 1.f, 0.f},   // Yellow       
            },
            { 
                .pos = { 0.0f, 0.0f, -height },  // v4
                .normal = {1.f, 1.f, 1.f},   // White       
            } 
        }
        std::vector<unsigned int> indices = {
            // the original two triangles form the base
            0, 1, 2, 2, 3, 0,
            // four new triangles
            4, 1, 0,
            4, 2, 1,
            4, 3, 2,
            4, 0, 3
        };

        std::shared_ptr<MeshGeometry> meshGeo = std::make_shared<MeshGeometry>(vertices, indices);

        return meshGeo;
    }

    // Convenience wrapper: a Box where width == height == depth
    static std::shared_ptr<MeshGeometry> createCube(float size) {
        return createBox(size, size, size);
    }

    // Generate a flat 2D plane (useful for floors/walls)
    static std::shared_ptr<MeshGeometry> createPlane(float width = 1.0f, float height = 1.0f) {
        auto model = std::make_shared<MeshGeometry>();
        
        return model;
    }

    static std::shared_ptr<MeshGeometry> createOpenCylinder(
        float radius, float height, unsigned int rSegments, unsigned int hSegments)
    {
        std::shared_ptr<OpenCylinderMesh> cylinder =
                    std::make_shared<OpenCylinderMesh>(radius, height, rSegments, hSegments);

        return cylinder;
    }

    // Generate a smooth sphere
    static std::shared_ptr<MeshGeometry> createSphere(float radius, unsigned int latSeg, unsigned int longSeg ) {
         std::shared_ptr<SphereMesh> sphere =
                    std::make_shared<SphereMesh>(radius, latSeg, longSeg);

        return sphere;
    }


};