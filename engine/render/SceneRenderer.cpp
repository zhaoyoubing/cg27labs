#include "SceneRenderer.h"
#include "../Components/TransformComponent.h"
#include "../Components/MeshRendererComponent.h"
#include <glm/gtc/matrix_transform.hpp>

SceneRenderer::SceneRenderer(uint32_t width, uint32_t height) {
    // --- Initialize Shaders ---
    shadowShader = std::make_shared<Shader>("assets/shaders/shadow.vert", "assets/shaders/shadow.frag");
    pbrShader    = std::make_shared<Shader>("assets/shaders/pbr.vert", "assets/shaders/pbr.frag");

    // --- Initialize Shadow Pass Resources ---
    shadowFramebuffer = std::make_shared<Framebuffer>(2048, 2048); 
    shadowPass = std::make_unique<RenderPass>(shadowFramebuffer);
    
    shadowPipeline = std::make_unique<PipelineState>();
    shadowPipeline->shader = shadowShader;
    shadowPipeline->depthTest = true;
    shadowPipeline->cullMode = GL_FRONT;

    // --- Initialize Main Pass Resources ---
    mainPass = std::make_unique<RenderPass>(nullptr); 
    
    pbrPipeline = std::make_unique<PipelineState>();
    pbrPipeline->shader = pbrShader;
    pbrPipeline->depthTest = true;
    pbrPipeline->cullMode = GL_BACK;
}

void SceneRenderer::renderFrame(entt::registry& registry) {
    glm::mat4 lightView = glm::lookAt(glm::vec3(0, 15, 5), glm::vec3(0), glm::vec3(0, 1, 0));
    glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 50.0f);

    glm::mat4 camView = glm::lookAt(glm::vec3(0, 2, 5), glm::vec3(0), glm::vec3(0, 1, 0));
    glm::mat4 camProj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);

    // --- 1. Execute Shadow Pass ---
    renderSystem.executePass(registry, *shadowPass, *shadowPipeline, lightView, lightProj);

    // --- 2. Execute Main Geometry Pass ---
    renderSystem.executePass(registry, *mainPass, *pbrPipeline, camView, camProj);
}