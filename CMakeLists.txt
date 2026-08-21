cmake_minimum_required(VERSION 3.15)
project(GraphicsLabs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Third-Party Dependencies (Submodules) ---

# 1. GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(submodules/glfw)

# 2. GLM (Header-only)
add_subdirectory(submodules/glm)


# 3. GLAD (Static library)
add_subdirectory(submodules/glad)


# --- Include Sub-Apps ---
add_subdirectory(apps)