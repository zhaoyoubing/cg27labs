add_library(engine STATIC
    device/Shader.cpp
    device/GPUPipeline.cpp
)

# Point to include directory
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(engine PUBLIC glfw glm glad tinygltf)