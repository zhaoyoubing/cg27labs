#include "device/shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Shader::Shader(ShaderStage stage, const std::string& filepath) 
    : stage_(stage) {
    std::string code = readFile(filepath);
    const char* src = code.c_str();

    GLenum glStage = getGLStage(stage);
    rendererID_ = glCreateShader(glStage);
    glShaderSource(rendererID_, 1, &src, nullptr);
    glCompileShader(rendererID_);
    checkCompileErrors(rendererID_);
}

Shader::~Shader() {
    if (rendererID_) {
        glDeleteShader(rendererID_);
    }
}

std::string Shader::readFile (const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "ERROR::SHADER::FAILED_TO_OPEN_FILE: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLenum Shader::getGLStage(ShaderStage stage) const {
    switch (stage) {
        case ShaderStage::Vertex:   return GL_VERTEX_SHADER;
        case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
        case ShaderStage::Compute:  return GL_COMPUTE_SHADER;
        // case ShaderStage::Task:     return GL_TASK_SHADER_NV; // Extension dependent
        // case ShaderStage::Mesh:     return GL_MESH_SHADER_NV; // Extension dependent
    }
    return 0;
}

void Shader::checkCompileErrors (GLuint shader) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        // Use std::vector instead of manual new[]/delete[] to prevent memory leaks
        std::vector<GLchar> log(length > 0 ? length : 1, 0);
        glGetShaderInfoLog(shader, length, nullptr, log.data());

        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << log.data() << "\n";
    }
}
