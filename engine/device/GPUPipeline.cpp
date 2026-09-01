#include <iostream>

#include "device/GPUPipeline.h"

#include <glm/gtc/type_ptr.hpp>  // for glm::value_ptr

GPUPipeline::GPUPipeline(const std::vector<Shader*>& shaders) {
    programID_ = glCreateProgram();

    // Attach all provided shader stages
    for (Shader* shader : shaders) {
        glAttachShader(programID_, shader->getID());
    }

    glLinkProgram(programID_);
    checkLinkErrors();
}

GPUPipeline::~GPUPipeline() {
    glDeleteProgram(programID_);
}

void GPUPipeline::bind() const {
    glUseProgram(programID_);
}

void GPUPipeline::unbind() const {
    glUseProgram(0);
}


GLint GPUPipeline::getUniformLocation(const std::string& name) const {
    if (uniformLocCache_.find(name) != uniformLocCache_.end())
        return uniformLocCache_[name];

    GLint location = glGetUniformLocation(programID_, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::PIPELINE::UNIFORM_NOT_FOUND: " << name << std::endl;
    }
    uniformLocCache_[name] = location;
    return location;
}

bool GPUPipeline::hasUniform(std::string_view name) const {
    GLint location =
        glGetUniformLocation(programID_, std::string(name).c_str());

    return location != -1;
}

void GPUPipeline::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void GPUPipeline::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void GPUPipeline::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void GPUPipeline::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void GPUPipeline::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void GPUPipeline::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void GPUPipeline::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void GPUPipeline::checkLinkErrors() {
    GLint success;
    glGetProgramiv(programID_, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(programID_, 1024, nullptr, infoLog);
        std::cout << "ERROR::PIPELINE::LINKING_FAILED\n" << infoLog << std::endl;
    }
}