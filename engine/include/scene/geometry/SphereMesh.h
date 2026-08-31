#pragma once

#include "scene/Vertex.h"
#include "scene/Mesh.h"

#include <glm/glm.hpp>

#include <spdlog/spdlog.h>

class SphereMesh : public Mesh {
public:
    float r_ = 1.0;
    unsigned int latSeg_ = 24;
    unsigned int longSeg_ = 24;

    SphereMesh() {
        initMesh();
    }

    SphereMesh(float radius, unsigned int latSeg, unsigned int longSeg):
        r_(radius), latSeg_(latSeg), longSeg_(longSeg)
    {
            initMesh();
    }

private:
    void initMesh() {
        float PI = glm::pi<float>();

        unsigned int res_phi = longSeg_;
        unsigned int res_theta = latSeg_;
        float radius = r_;

        float stepPhi = 2.0 * PI / res_phi;
        float stepTheta = PI / res_theta;

        uint32_t idx = 0;

        Vertex v;
        // Top piece
        glm::vec3 top = glm::vec3(0, 1, 0) ;
        vertices.push_back({radius * top, top, glm::vec2(0,0)});

        glm::vec3 pos = glm::vec3(0.0);
        float phi = 0;

        for (uint32_t j = 0; j < res_phi; j++) {
            
            idx++;

            // current radius
            float cr = sin(stepTheta);

            pos = {cr * cos(stepPhi * j), cos(stepTheta),  cr * sin(stepPhi * j)   };

            vertices.push_back({radius * pos, pos, glm::vec2(j / (float) res_phi, 1 / (float) res_theta) });

            //positions.push_back(radius * pos);
            //normals.push_back(pos);
            //uvs.push_back( glm::vec2(j / (float) res_phi, 1 / (float) res_theta) );
            
            if (j > 0) {
                indices.insert(indices.end(), {0, idx - 1, idx});
            } else {
                indices.insert(indices.end(), {0, res_phi, 1});   // j = 0
            }

            phi += stepPhi;
        }

        // Middle piece
        for (int i = 2; i < res_theta; i++) 
        {
            phi = 0;
            float y = cos(i * stepTheta);
            float cr = sin(i * stepTheta);

            for (int j = 0; j < res_phi; j++) 
            {
                idx ++;

                pos = glm::vec3(cr * cos(phi), y, cr * sin(phi) );
                
                vertices.push_back({radius * pos, pos, glm::vec2(j / (float) res_phi, 1 / (float) res_theta) });

                //positions.push_back(radius * pos);
                //normals.push_back(pos);
                //uvs.push_back(glm::vec2(j / (float) res_phi, 1 / (float) res_theta) );

                if (j > 0) {
                    indices.insert(indices.end(), {idx - 1 - res_phi, idx - 1, idx  });
                    indices.insert(indices.end(), {idx - res_phi, idx - 1 - res_phi, idx  });
                } else {
                    // j = 0
                    indices.insert(indices.end(), {idx - res_phi, idx - 1, idx});
                    indices.insert(indices.end(), {idx - 1, idx + res_phi - 1, idx});
                }

                phi += stepPhi;
            }
        }

        // Bottom piece
        glm::vec3 bottom = glm::normalize(glm::vec3(0, -1, 0));

        vertices.push_back({radius * bottom, bottom, glm::vec2(0, 1.0) });

        //positions.push_back(radius * bottom);
        //normals.push_back(bottom);
        //uvs.push_back(glm::vec2(0, 1.0));

        phi = 0;

        for (uint32_t j = 0; j < res_phi; j++) {
            idx++;

            if (j > 0) {
                indices.insert(indices.end(), {idx - res_phi, idx - res_phi - 1, res_phi * ( res_theta - 1) + 1});
            } else {
                // j = 0
                indices.insert(indices.end(), {res_phi * ( res_theta - 1) , idx - res_phi, res_phi * ( res_theta - 1) + 1});
            }

        }

        spdlog::debug("Vertex number {}, Index number {}", vertices.size(), indices.size());
    }
};