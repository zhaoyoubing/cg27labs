#pragma once

#include "scene/Vertex.h"
#include "scene/Mesh.h"

enum class PlaneDirection {
    XY,
    XZ,
    YZ
};

class PlaneMesh : public Mesh {
public:

    PlaneDirection dir_ = PlaneDirection::XZ;

    glm::vec3 origin_;

    float width_ = 1.0;
    float len_ = 1.0;

    int resWid_ = 5.0;
    int resLen_ = 5.0;

    PlaneMesh(float wid, float len,
        int res_wid, int res_len,
        glm::vec3 orig = glm::vec3(0.0f), PlaneDirection dir = PlaneDirection::XZ )
        : width_(wid), len_(len), resWid_(res_wid), resLen_(res_len), origin_(orig), dir_ (dir)
    {
        if (PlaneDirection::XY == dir) {
            createXY();
        }
        else if (PlaneDirection::XZ == dir) {
            createXZ();
        }
        else {
            createYZ();
        }
    }

private:
    void createXZ() {

        float leapWid = (float) width_ / (float)resWid_;
        float leapLen = (float) len_ / (float)resLen_;

        for (float z = 0; z < resLen_; z += 1.0f) {
            for (float x = 0; x < resWid_; x += 1.0f) {
                positions.push_back ( glm::vec3(x * leapWid, 0, z * leapLen) + origin_);
                normals.push_back(glm::vec3(0,1,0));
                uvs.push_back(glm::vec2(x /(resWid_ - 1), z / (resLen_-1)) );          
            }
        }

        for (uint32_t i = 0; i < resWid_ - 1; i++) {
            for (uint32_t j = 0; j < resLen_ -1; j++) {
                indices.insert(indices.end(), 
                        { i + j * resWid_,  (i + 1) + j * resWid_, i + (j + 1) * resWid_} ); // Triangle 1
                indices.insert(indices.end(), 
                        {(i + 1) + j * resWid_,  (i + 1) + (j + 1) * resWid_, i + (j + 1) * resWid_}); // Triangle 2

            }
        }
    }

    void createXY() {}
    void createYZ() {}
};