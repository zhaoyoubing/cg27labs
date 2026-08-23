cmake_minimum_required(VERSION 3.15)
project(TA01b LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(TA01b
    main.cpp
    drawColourVertex.cpp
)

# 2. Link the global third-party libraries defined in the root CMakeLists.txt
target_link_libraries(TA01b PRIVATE
    glfw
    glad
    engine
)

# copy shaders
add_custom_command(
        TARGET TA01b POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/shaders"
                $<TARGET_FILE_DIR:TA01b>/shaders    
        )