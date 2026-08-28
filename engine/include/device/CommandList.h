#pragma once

#include <glm/glm.hpp>

#include <scene/geometry/MeshGeoGPU.h>

enum class CommandType
{
    BindPipeline,
    BindMesh,
    BindTexture,
    SetUniform,
    DrawIndexed
};

struct RenderCommand
{
    CommandType type;

    const MeshGeoGPU* mesh = nullptr;
    unsigned int pipeline = 0;
    unsigned int texture = 0;

    glm::mat4 transform{1.0f};
};

/*
CPU
 │
 │ RECORD
 ▼
┌──────────────────────────┐
│       CommandList        │
│                          │
│ Bind Pipeline            │
│ Bind Mesh 1              │
│ Draw Indexed             │
│ Bind Mesh 2              │
│ Draw Indexed              │
└──────────────────────────┘
 │
 │ EXECUTE
 ▼
OpenGL
 │
 ├── glUseProgram()
 ├── glBindVertexArray()
 ├── glDrawElements()
 ├── glBindVertexArray()
 └── glDrawElements()

*/
class CommandList
{
public:
    void bindPipeline(unsigned int pipeline);

    void bindMesh(const MeshGPU& mesh);

    void bindTexture(unsigned int texture);

    void drawIndexed();

    void execute() const;

private:
    std::vector<RenderCommand> commands_;
};


/*
exampels:

CommandList commands;

commands.bindPipeline(shaderProgram);
commands.bindMesh(mesh1);
commands.drawIndexed();

commands.bindMesh(mesh2);
commands.drawIndexed();

commands.execute();
*/