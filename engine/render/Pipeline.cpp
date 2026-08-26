#include "render/pipeline.h"

#include <iostream>

Pipeline::Pipeline(const std::vector<Shader*>& shaders) {
    programID_ = glCreateProgram();

    // Attach all provided shader stages
    for (Shader* shader : shaders) {
        glAttachShader(programID_, shader->getID());
    }

    glLinkProgram(programID_);
    checkLinkErrors();
}

Pipeline::~Pipeline() {
    glDeleteProgram(programID_);
}

void Pipeline::bind() const {
    glUseProgram(programID_);
}

void Pipeline::unbind() const {
    glUseProgram(0);
}

GLint Pipeline::getUniformLocation(const std::string& name) const {
    if (uniformLocCache_.find(name) != uniformLocCache_.end())
        return uniformLocCache_[name];

    GLint location = glGetUniformLocation(programID_, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::PIPELINE::UNIFORM_NOT_FOUND: " << name << std::endl;
    }
    uniformLocCache_[name] = location;
    return location;
}

void Pipeline::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

// Other uniform setter implementations...

void Pipeline::checkLinkErrors() {
    GLint success;
    glGetProgramiv(programID_, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(programID_, 1024, nullptr, infoLog);
        std::cout << "ERROR::PIPELINE::LINKING_FAILED\n" << infoLog << std::endl;
    }
}