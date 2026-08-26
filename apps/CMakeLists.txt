cmake_minimum_required(VERSION 3.15)
project(GLApps NONE)

# Add each lab folder you create under apps/
# add_subdirectory(TA01a)
# add_subdirectory(TA01b)
#add_subdirectory(TA01c)
# add_subdirectory(TA01d)

# Recursively find all CMakeLists.txt files underneath the 'src' directory
file(GLOB_RECURSE CHILD_CMAKE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt"
)

# Loop through each found file and extract its parent directory
foreach(CHILD_CMAKE_FILE ${CHILD_CMAKE_FILES})
    get_filename_component(SUBDIR_PATH "${CHILD_CMAKE_FILE}" DIRECTORY)
    
    # Message to show it's working during configuration
    message(STATUS "Auto-adding subdirectory: ${SUBDIR_PATH}")
    
    # Register it with CMake
    add_subdirectory(${SUBDIR_PATH})
endforeach()
