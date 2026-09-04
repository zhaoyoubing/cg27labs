#include "SkyboxPass.h"

void SkyboxPass::render(const Skybox& skybox,
    const Camera& camera)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skybox.render(camera, *shader);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}