cmake_minimum_required(VERSION 3.15)

# Define the project/target name in one place here
set(PROJ_NAME TA09_SSAO)

project(${PROJ_NAME} LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(${PROJ_NAME})

# 2. source
target_sources(${PROJ_NAME}
    PRIVATE
        main.cpp
        #MyGameApp.cpp
    )

# 3. third-party libraries
target_link_libraries(${PROJ_NAME} PRIVATE
    glfw
    glad
    engine
    spdlog::spdlog
    tinygltf
)

# copy shaders and models to the target directory
add_custom_command(
        TARGET ${PROJ_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/shaders"
                $<TARGET_FILE_DIR:${PROJ_NAME}>/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/assets"
                $<TARGET_FILE_DIR:${PROJ_NAME}>/assets
        )