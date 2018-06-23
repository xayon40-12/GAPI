//
// Created by Nathan Touroux on 23/06/2018.
//

#ifndef GAPI_OPENCL_GL_HPP
#define GAPI_OPENCL_GL_HPP


#include "OpenCL.hpp"
#include "../OpenGL/Texture.hpp"
#include <OpenGL/OpenGL.h>

class OpenCL_GL: public OpenCL {
private:
    std::vector<cl_mem> sharedObjects;

public:
    bool init(cl_device_type type = CL_DEVICE_TYPE_GPU) override;// CL_DEVICE_TYPE_GPU CL_DEVICE_TYPE_CPU
    bool buildProgram(std::string source, std::string funcName) override;//WARNING: all Mem must be added before creating the program
    bool compute(std::vector<size_t> globalWorkSize) override;

    bool addTexture(Texture &texture, std::string readWrite = "");
};


#endif //GAPI_OPENCL_GL_HPP
