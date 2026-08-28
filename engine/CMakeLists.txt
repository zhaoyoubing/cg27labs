add_library(engine STATIC)

file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

list(REMOVE_ITEM ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/render/SceneRenderer.cpp"
)

# source file
target_sources(engine
    PRIVATE
    ${ENGINE_SOURCES}
)

# the include directory
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# external libraries
target_link_libraries(engine PUBLIC glfw glm glad tinygltf spdlog::spdlog)