add_library(engine STATIC)

# source file
target_sources(engine
    PRIVATE
        GameApp.cpp
        device/Shader.cpp
        device/GPUPipeline.cpp
        scene/GltfMeshLoader.cpp
)

# the include directory
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# external libraries
target_link_libraries(engine PUBLIC glfw glm glad stb_image tinygltf spdlog::spdlog)