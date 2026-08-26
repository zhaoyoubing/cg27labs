cmake_minimum_required(VERSION 3.15)
project(TA00 LANGUAGES CXX)

# 1. Define the executable target and its source files
add_executable(TA00
    main.cpp
)

# 2. Link the global third-party libraries defined in the root CMakeLists.txt
# target_link_libraries(TA00 PRIVATE
#    glfw
#    glm
#    glad
#    engine
#    spdlog
#)