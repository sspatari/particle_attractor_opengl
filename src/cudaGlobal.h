//
// Created by strongheart on 7/9/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_CUDAGLOBAL_H
#define PARTICLE_ATTRACTOR_OPENGL_CUDAGLOBAL_H

#include <sstream>
#include <stdexcept>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <device_launch_parameters.h>

#define CHECK_CUDA(ans) checkCuda((ans), "", #ans, __FILE__, __LINE__)
#define CHECK_LAST(msg) checkCuda(cudaGetLastError(), msg, "CHECK_LAST", __FILE__, __LINE__)

inline
void throwError(int code,
                 const char* error_string,
                 const char* msg,
                 const char* func,
                 const char* file,
                 int line) {
    throw std::runtime_error("CUDA Error "
                             +std::string(msg)
                             +" "+std::string(error_string)
                             +" ["+std::to_string(code)+"]"
                             +" "+std::string(file)
                             +":"+std::to_string(line)
                             +" "+std::string(func)
    );
}

inline
void checkCuda(cudaError_t code, const char *msg, const char *func, const char *file, int line) {
    if (code != cudaSuccess) {
        throwError(static_cast<int>(code),
                    cudaGetErrorString(code), msg, func, file, line);
    }
}


inline
std::stringstream getCUDADeviceInformations(int dev) {
    std::stringstream info;
    cudaDeviceProp prop;
    int runtimeVersion = 0;
    size_t f=0, t=0;
    CHECK_CUDA(cudaRuntimeGetVersion(&runtimeVersion));
    CHECK_CUDA(cudaGetDeviceProperties(&prop, dev));
    CHECK_CUDA(cudaMemGetInfo(&f, &t));
    info << '"' << prop.name << '"'
         << ", \"CC\", " << prop.major << '.' << prop.minor
         << ", \"Multiprocessors\", "<< prop.multiProcessorCount
         << ", \"Memory [MiB]\", "<< t/1048576
         << ", \"MemoryFree [MiB]\", " << f/1048576
         << ", \"MemClock [MHz]\", " << prop.memoryClockRate/1000
         << ", \"GPUClock [MHz]\", " << prop.clockRate/1000
         << ", \"CUDA Runtime\", " << runtimeVersion
        ;
    return info;
}

inline
std::stringstream listCudaDevices() {
    std::stringstream info;
    int nrdev = 0;
    CHECK_CUDA(cudaGetDeviceCount(&nrdev));
    if(nrdev==0)
        throw std::runtime_error("No CUDA capable device found");
    for(int i=0; i<nrdev; ++i)
        info << "\"ID\"," << i << "," << getCUDADeviceInformations(i).str() << std::endl;
    return info;
}

#endif //PARTICLE_ATTRACTOR_OPENGL_CUDAGLOBAL_H
