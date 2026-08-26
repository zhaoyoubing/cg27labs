#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

class Pipeline {
public:
    // Pass a collection of compiled shaders (e.g., Vertex + Fragment)
    Pipeline(const std::vector<Shader*>& shaders);
    ~Pipeline();

    void bind() const;
    void unbind() const;

    // Uniform Caching & Setters
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    GLuint programID_ = 0;
    mutable std::unordered_map<std::string, GLint> uniformLocCache_;

    GLint getUniformLocation(const std::string& name) const;
    void checkLinkErrors();
};