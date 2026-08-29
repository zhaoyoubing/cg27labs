#pragma once

#include "scene/MeshGeometry.h"

#include <vector>

class OpenCylinderMesh : public MeshGeometry {
public:
    float r_ = 1.0;
    float h_ = 1.0;

    unsigned int rSeg_ = 32;
    unsigned int hSeg_ = 32;

    OpenCylinderMesh(float r, float h,
        unsigned int rSeg, unsigned int hSeg) : r_(r), h_(h), rSeg_{rSeg}, hSeg_{hSeg}
        {
            initMesh(r, h, rSeg, hSeg);
        }

private:    
    void initMesh(
        float radius, float height,
        unsigned int radialSegments, unsigned int heightSegments)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const float halfHeight = height * 0.5f;

        // Generate vertices
        for (unsigned int y = 0; y <= heightSegments; ++y)
        {
            float v = static_cast<float>(y) / heightSegments;
            float yPos = -halfHeight + v * height;

            for (unsigned int x = 0; x <= radialSegments; ++x)
            {
                float u = static_cast<float>(x) / radialSegments;

                float theta = u * 2.0f * glm::pi<float>();

                float xPos = radius * std::cos(theta);
                float zPos = radius * std::sin(theta);

                glm::vec3 position{xPos, yPos, zPos ;
                glm::vec3 normal = glm::normalize(glm::vec3(xPos, 0.0f, zPos ));
                vertices.push_back({position, normal, {u, v} });
            }
        }

        // Generate indices
        for (unsigned int y = 0; y < heightSegments; ++y)
        {
            for (unsigned int x = 0; x < radialSegments; ++x)
            {
                unsigned int row1 = y * (radialSegments + 1);

                unsigned int row2 = (y + 1) * (radialSegments + 1);

                unsigned int bottomLeft  = row1 + x;
                unsigned int bottomRight = row1 + x + 1;
                unsigned int topLeft     = row2 + x;
                unsigned int topRight    = row2 + x + 1;

                // First triangle
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
                indices.push_back(topRight);

                // Second triangle
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                indices.push_back(topLeft);
            }
        }

    }
};