cmake_minimum_required(VERSION 3.15)
project(TA01d LANGUAGES CXX)

# 1. the target executable
add_executable(TA01d)

# 2. source files
target_sources(TA01d
    PRIVATE
    main.cpp
    drawColourVertex.cpp
)

# 2. Link the global third-party libraries defined in the root CMakeLists.txt
target_link_libraries(TA01d PRIVATE
    glfw
    glad
    engine
    spdlog::spdlog
)

# copy shaders
add_custom_command(
        TARGET TA01d POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/shaders"
                $<TARGET_FILE_DIR:TA01d>/shaders    
        )