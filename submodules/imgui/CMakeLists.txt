# You need to manually copy this file to submodules/imgui as CMakeLists.txt if you need to use the imgui library
# You also need to append imgui to your target_link_libraries() list in the CMakeLists.txt of your project


# Define a static library target for ImGui
add_library(imgui STATIC
    imgui.h
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    # Include the backends you plan to use (e.g., GLFW and OpenGL 3)
    backends/imgui_impl_glfw.h
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3.cpp
)

# Specify where other targets can find ImGui headers
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# ImGui backends rely on GLFW and OpenGL headers, so link them publicly
target_link_libraries(imgui PUBLIC glfw glad)