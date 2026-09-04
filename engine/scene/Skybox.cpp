#include "Skybox.h"

#include "MeshGeometry.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

Skybox::Skybox(
    std::shared_ptr<MeshGeometry> geometry,
    std::shared_ptr<Texture> cubemap)
    : geometry(std::move(geometry)),
      cubemap(std::move(cubemap))
{
}

void Skybox::render(
    const Camera& camera,
    const Shader& shader) const
{
    shader.use();

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    // Remove camera translation.
    view = glm::mat4(glm::mat3(view));

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->getID());

    shader.setInt("skybox", 0);

    geometry->draw();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}