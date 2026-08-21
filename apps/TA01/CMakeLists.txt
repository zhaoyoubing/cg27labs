cmake_minimum_required(VERSION 3.15)
project(TA01 LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(TA01
    main.cpp
    # You can add more cpp files to this lab later
)

# 2. Link the global third-party libraries defined in the root CMakeLists.txt
target_link_libraries(TA01 PRIVATE
    glfw
    glm
    glad
)