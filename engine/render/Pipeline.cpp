#include "render/pipeline.h"

#include <iostream>

Pipeline::Pipeline(const std::vector<Shader*>& shaders) {
    m_ProgramID = glCreateProgram();

    // Attach all provided shader stages
    for (Shader* shader : shaders) {
        glAttachShader(m_ProgramID, shader->getID());
    }

    glLinkProgram(m_ProgramID);
    checkLinkErrors();
}

Pipeline::~Pipeline() {
    glDeleteProgram(m_ProgramID);
}

void Pipeline::bind() const {
    glUseProgram(m_ProgramID);
}

void Pipeline::unbind() const {
    glUseProgram(0);
}

GLint Pipeline::getUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1) {
        std::cout << "WARNING::PIPELINE::UNIFORM_NOT_FOUND: " << name << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void Pipeline::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

// Other uniform setter implementations...

void Pipeline::checkLinkErrors() {
    GLint success;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(m_ProgramID, 1024, nullptr, infoLog);
        std::cout << "ERROR::PIPELINE::LINKING_FAILED\n" << infoLog << std::endl;
    }
}