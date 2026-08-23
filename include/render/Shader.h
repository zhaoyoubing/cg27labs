#pragma once
#include <string>
#include <glad/glad.h>

enum class ShaderStage {
    Vertex,
    Fragment,
    Compute,
    // Task,
    // Mesh

};

class Shader {
public:
    Shader(ShaderStage stage, const std::string& filepath);
    ~Shader();

    GLuint getID() const { return m_RendererID; }
    ShaderStage getStage() const { return m_Stage; }

private:
    GLuint m_RendererID = 0;
    ShaderStage m_Stage;
    
    GLenum getGLStage(ShaderStage stage) const;
    std::string readFile(const std::string& filepath);
    void checkCompileErrors(GLuint shader);
};