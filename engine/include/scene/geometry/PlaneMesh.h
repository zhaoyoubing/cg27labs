#pragma once

#include "scene/Vertex.h"
#include "scene/MeshGeometry.h"

class PlaneMesh : public MeshGeometry {
public:
    static const int XY = 0;
    static const int XZ = 1;
    static const int YZ = 2;

    int dir_ = 1;

    float orgWid_ = 0;
    float orgLen_ = 0;
    float width_ = 1.0;
    float len_ = 1.0;
    int resWid_ = 5.0;
    int resLen_ = 5.0;

    float height_ = 0.0;


    PlaneMesh(int dir, float wid, float len,
        int res_width, int res_length,
        float orig_width = 0, float orig_length = 0, float h = 0)
    {

        dir_ = dir;
        width_ = wid;
        len_ = len;
        
        resWid_ = res_width;
        resLen_ = res_length;

        orgWid_ = orig_width;
        orgLen_ = orig_length;

        height_ = h;

        if (XY == dir) {
            createXY();
        }
        else if (XZ == dir) {
            createXZ();
        }
        else {
            createYZ();
        }
    }

private:
    void createXZ() {

        float leapWid = (float) width_ / (float)resWid_;
        //float offset_x = (float) width / 2.0f - (float) leapWid / 2.0f; // center

        float leapLen = (float) len_ / (float)resLen_;
        //float offset_z = (float) length / 2.0f - (float) leapLen / 2.0f; // center

        Vertex v;
        for (float z = 0; z < resLen_; z += 1.0f) {
            for (float x = 0; x < resWid_; x += 1.0f) {
                v.pos = glm::vec3(x * leapWid + orgWid_, height_, z * leapLen + orgLen_);
                //init_positions.push_back(glm::vec3(x*leap_x - offset_x, height, z*leap_z - offset_z));

                v.normal = glm::vec3(0,1,0);
                v.uv = glm::vec2(x /(resWid_ - 1), z / (resLen_-1));
                vertices.push_back(v);
            }
        }

        for (int i = 0; i < resWid_ - 1; i++) {
            for (int j = 0; j < resLen_ -1; j++) {
                // Triangle 1
                indices.push_back(i + j * (resWid_));
                indices.push_back((i + 1) + j * (resWid_));
                indices.push_back(i + (j + 1) * (resWid_));

                // Triangle 2
                indices.push_back((i + 1) + j * (resWid_));
                indices.push_back((i + 1) + (j + 1) * (resWid_));
                indices.push_back((i) + (j + 1) * (resWid_));

            }
        }
    }

    void createXY() {}
    void createYZ() {}
};