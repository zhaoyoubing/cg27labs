cmake_minimum_required(VERSION 3.15)
project(TA01a LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(TA01a
    main.cpp
    drawQuad.cpp
)

# 2. Link the global third-party libraries defined in the root CMakeLists.txt
target_link_libraries(TA01a PRIVATE
    glfw
    glm
    glad
)