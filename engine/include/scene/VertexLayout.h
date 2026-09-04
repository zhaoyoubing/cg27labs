#include "glad/glad.h"

enum class VertexAttribute
{
    Position,
    Normal,
    TexCoord
};

struct VertexElement
{
    VertexAttribute attribute;
    GLint componentCount;
    GLenum type;
};