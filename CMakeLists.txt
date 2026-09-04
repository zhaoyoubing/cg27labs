cmake_minimum_required(VERSION 3.15)
project(GraphicsLabs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Global Include Directories ---
# has been includeed in the engine target, so no need to include it here
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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

# 4. tinygltf (Header-only) as an interface target
add_library(tinygltf INTERFACE)

target_include_directories(tinygltf INTERFACE 
    ${CMAKE_SOURCE_DIR}/submodules/tinygltf
)

# 5. spdlog for logging and debug messages
add_subdirectory(submodules/spdlog)

# 6. Tracy for GPU profiling
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy Profiler" FORCE)
add_subdirectory(submodules/tracy)

# 7. imgui (Static library) for GUI
add_subdirectory(submodules/imgui)

add_subdirectory(engine)

# --- Include Sub-Apps ---
add_subdirectory(apps)