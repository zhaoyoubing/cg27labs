cmake_minimum_required(VERSION 3.15)
project(TA01a LANGUAGES CXX)

# 1. the target executable
add_executable(TA01a)

# 2. source file
target_sources(TA01a
    PRIVATE
    main.cpp
    drawQuad.cpp
)

# 3. Link the global third-party libraries defined in the root CMakeLists.txt
target_link_libraries(TA01a PRIVATE
    glfw
    glm
    glad
)