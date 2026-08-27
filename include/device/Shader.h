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

    GLuint getID() const { return rendererID_; }
    ShaderStage getStage() const { return stage_; }

private:
    GLuint rendererID_ = 0;
    ShaderStage stage_;
    
    GLenum getGLStage(ShaderStage stage) const;
    std::string readFile(const std::string& filepath);
    void checkCompileErrors(GLuint shader);
};