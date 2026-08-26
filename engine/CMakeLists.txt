add_library(engine STATIC
    GameApp.cpp
    render/Shader.cpp
    render/GPUPipeline.cpp
)

# Point to the root-level include directory (one level up from engine/)
target_include_directories(engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(engine PUBLIC glfw glm glad stb_image tinygltf spdlog::spdlog)