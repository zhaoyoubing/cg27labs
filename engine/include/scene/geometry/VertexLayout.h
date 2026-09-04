#pragma once

#include <vector>

// experimental
// flexible layout for binary vertex interleaved data
enum class VertexAttributeType {
    Position, Normal, Tangent, UV0, UV1, Color
};

struct VertexLayoutElement {
    VertexAttributeType type;
    uint32_t componentCount; // e.g., 3 for vec3, 2 for vec2
    uint32_t offset;         // byte offset inside the stride
};

/*
class MeshGeometry {
    std::vector<uint8_t> rawVertexData; // Raw byte blob
    std::vector<VertexLayoutElement> layout; // Describes how to read the blob
    std::vector<unsigned int> indices;
    uint32_t vertexStride = 0;
};
*/