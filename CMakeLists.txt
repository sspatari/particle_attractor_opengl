cmake_minimum_required(VERSION 3.10)
project(particle_atraction_opengl)

set(CMAKE_CXX_STANDARD 14)

## -- GLFW --
find_package(glfw3 REQUIRED)
if(NOT {glfw3_FOUND)
    message("glfw3 not found")
endif()

## -- OpenGL --
find_package(OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARY}
        glfw
)