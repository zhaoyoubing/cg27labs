cmake_minimum_required(VERSION 3.15)
project(TA02 LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(TA02b)

# 2. source
target_sources(TA02b
    PRIVATE
        main.cpp
        #MyGameApp.cpp
    )

# 3. third-party libraries
target_link_libraries(TA02b PRIVATE
    glfw
    glad
    engine
    spdlog::spdlog
    tinygltf
)

# copy shaders and models to the target directory
add_custom_command(
        TARGET TA02b POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/shaders"
                $<TARGET_FILE_DIR:TA02b>/shaders    
        )