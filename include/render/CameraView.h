// include/render/CameraView.h

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "entity/CameraComp.h"

struct Viewport {
    float x = 0.0f;
    float y = 0.0f;
    float w = 800.0f;
    float h = 600.0f;

    float GetAspectRatio() const {
        return (h > 0.0f) ? (w / h) : (16.0f / 9.0f);
    }
};

class CameraView {
private:
    glm::vec3 eye = {0.0f, 0.0f, 0.0f};
    CameraComp camera;
    Viewport viewport;

public:
    CameraView(CameraComp cam, Viewport vp)
        : camera(cam), viewport(vp) {}

    void setViewport(float w, float h) {
        viewport.w = w;
        viewport.h = h;
    }

    // It can query the world directly when needed
    glm::mat4 getProjMatrix(const CameraComp& camera) const {
        return glm::perspective(glm::radians(camera.fov), viewport.GetAspectRatio(), camera.near, camera.far);
    }

    // Calculates the view matrix using the entity's world position and camera front vector
    glm::mat4 getViewMatrix(const glm::vec3& e) const {
        return glm::lookAt(e, e + camera.front, camera.up);
    }

    // You can even wrap OpenGL's viewport command here!
    void applyToRenderer() const {
        glViewport(static_cast<GLint>(viewport.x), 
                   static_cast<GLint>(viewport.y), 
                   static_cast<GLsizei>(viewport.w), 
                   static_cast<GLsizei>(viewport.h));
    }
};