cmake_minimum_required(VERSION 3.10)
project(particle_attractor_opengl)

set(CMAKE_CXX_STANDARD 14)

## -- OpenGL --
find_package(OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
endif()

## -- GLEW --
find_package(GLEW REQUIRED)
if(NOT ${GLEW_FOUND})
    message("GLEW not found")
endif()

## -- GLFW --
find_package(glfw3 REQUIRED)
if(NOT {glfw3_FOUND)
    message("glfw3 not found")
endif()

add_executable(
        ${PROJECT_NAME}
        src/main.cpp
        src/shaders/Shader.cpp
        src/shaders/Shader.h
        src/VertexArray.cpp
        src/VertexArray.h
        src/VertexBufferLayout.h
        src/VertexBuffer.cpp
        src/VertexBuffer.h
        src/DisplayManager.cpp
        src/DisplayManager.h
)

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARY}
        ${GLEW_LIBRARY}
        glfw
)