cmake_minimum_required(VERSION 3.23)
project(V-Engine VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include/glad/include
    ${PROJECT_SOURCE_DIR}/include/glfw/include
    ${PROJECT_SOURCE_DIR}/include/glm
)

# GLAD
add_library(glad STATIC
    ${PROJECT_SOURCE_DIR}/include/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${PROJECT_SOURCE_DIR}/include/glad/include
)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/include/glfw)


# Add all source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

add_executable(V-Engine ${SOURCES})

# Add include directories (so the compiler can find your headers)
target_include_directories(V-Engine PRIVATE src)

# Link libraries
target_link_libraries(V-Engine
    glad
    glfw
    ${CMAKE_DL_LIBS}
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(V-Engine opengl32)
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(V-Engine OpenGL::GL)
elseif(APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(V-Engine OpenGL::GL)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(V-Engine PRIVATE /W4)
else()
    target_compile_options(V-Engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
