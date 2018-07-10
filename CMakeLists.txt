cmake_minimum_required(VERSION 3.10)
project(particle_attractor_opengl)

set(CMAKE_CXX_STANDARD 17)

## -- CUDA --
find_package(CUDA REQUIRED)
if(NOT ${CUDA_FOUND})
    message("CUDA not found")
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(CUDA_VERSION_STRING VERSION_EQUAL 9.0 OR CUDA_VERSION_STRING VERSION_GREATER 9.0)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 8.0
                OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 8.0)
            message(FATAL_ERROR "CUDA 9.0 does not support gcc6+. Please choose an older gcc version like:\n
cmake -DCMAKE_C_COMPILER=`which gcc-7` -DCMAKE_CXX_COMPILER=`which g++-7` ..")
        endif()
    elseif(CUDA_VERSION_STRING VERSION_EQUAL 7.5 OR CUDA_VERSION_STRING VERSION_GREATER 7.5)
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0
                OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 5.0)
            message(WARNING "CUDA 7.5 does not support gcc5+. Please choose an older gcc version like:\n
cmake -DCMAKE_C_COMPILER=`which gcc-4` -DCMAKE_CXX_COMPILER=`which g++-4` ..")
        endif()
    endif()
endif()

include_directories(${CUDA_INCLUDE_DIRS})
list(APPEND CUDA_NVCC_FLAGS "-std=c++14 -gencode arch=compute_50,code=compute_50")
set(CUDA_PROPAGATE_HOST_FLAGS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND CUDA_NVCC_FLAGS "-g -lineinfo")
endif()

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

cuda_add_executable(
        ${PROJECT_NAME}
        src/Application.cpp
        src/shaders/Shader.cpp
        src/shaders/Shader.h
        src/VertexArray.cpp
        src/VertexArray.h
        src/VertexBufferLayout.h
        src/VertexBuffer.cpp
        src/VertexBuffer.h
        src/DisplayManager.cpp
        src/DisplayManager.h
        src/Renderer.cpp
        src/Renderer.h
        src/cudaGlobal.h
        src/kernel.cu
)

target_link_libraries(
        ${PROJECT_NAME}
        ${CUDA_LIBRARIES}
        ${OPENGL_LIBRARY}
        ${GLEW_LIBRARY}
        glfw
)