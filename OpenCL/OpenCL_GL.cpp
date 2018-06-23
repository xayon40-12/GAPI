//
// Created by Nathan Touroux on 23/06/2018.
//

#include <OpenGL/OpenGL.h>
#include "OpenCL_GL.hpp"

bool OpenCL_GL::init(cl_device_type type){
    cl_int error;
    cl_uint platformIdCount = 0;
    clGetPlatformIDs (0, nullptr, &platformIdCount);
    if(platformIdCount == 0)return false;
    std::vector<cl_platform_id> platformIds (platformIdCount);
    clGetPlatformIDs (platformIdCount, platformIds.data(), nullptr);

    for(cl_platform_id platformId: platformIds){
        cl_uint deviceIdCount = 0;
        clGetDeviceIDs (platformId, type, 0, nullptr, &deviceIdCount);
        if(deviceIdCount == 0)continue;

        std::vector<cl_device_id> deviceIds(deviceIdCount);
        clGetDeviceIDs (platformId, type, deviceIdCount, deviceIds.data(), nullptr);

        cl_context_properties contextProperties[] = {
#if defined (WIN32)
        CL_GL_CONTEXT_KHR , (cl_context_properties) wglGetCurrentContext() ,
        CL_WGL_HDC_KHR , (cl_context_properties) wglGetCurrentDC() ,
#elif defined (__linux__)
        CL_GL_CONTEXT_KHR , (cl_context_properties) glXGetCurrentContext() ,
        CL_GLX_DISPLAY_KHR , (cl_context_properties) glXGetCurrentDisplay() ,
#elif defined (__APPLE__)
                CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE , (cl_context_properties) CGLGetShareGroup( CGLGetCurrentContext() ) ,
#endif
                CL_CONTEXT_PLATFORM , (cl_context_properties) platformId , 0 , 0};

        for(auto deviceId: deviceIds){
            device = deviceId;
            context = clCreateContext(contextProperties, 1, &device, 0, 0, &error);
            if(error == CL_SUCCESS){
                queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &error);
                if(error == CL_SUCCESS)return true;
            }
        }
    }

    return false;
}

bool OpenCL_GL::compute(std::vector<size_t> globalWorkSize) {
    if(sharedObjects.size() > 0){
        cl_int error = 0;
        // We must make sure that OpenGL is done with the textures, so
        // we ask to sync.
        glFinish();
        error = clEnqueueAcquireGLObjects(queue, sharedObjects.size(), sharedObjects.data(), 0, NULL, NULL);
        if(error != CL_SUCCESS) return false;

        if(!OpenCL::compute(globalWorkSize)) return false;

        // Before returning the objects to OpenGL, we sync to make sure OpenCL is done.
        error = clEnqueueReleaseGLObjects(queue, sharedObjects.size(), sharedObjects.data(), 0, NULL, NULL);
        if(error != CL_SUCCESS) return false;
        error = clFinish(queue);
        if(error != CL_SUCCESS) return false;
    }else{
        if(!OpenCL::compute(globalWorkSize)) return false;
    }

    return true;
}

bool OpenCL_GL::buildProgram(std::string source, std::string funcName) {
    if(!OpenCL::buildProgram(source, funcName)) return false;

    int offset = mems.size();
    for(int i = 0;i<sharedObjects.size();i++)
        clSetKernelArg (kernel, offset+i, sizeof(cl_mem), &sharedObjects[i]);

    return true;
}

bool OpenCL_GL::addTexture(Texture &texture, std::string readWrite) {
    cl_int error;
    cl_mem_flags flag = 0;
    if(readWrite == "r"){
        flag |= CL_MEM_READ_ONLY;
    }else if(readWrite == "w"){
        flag |= CL_MEM_WRITE_ONLY;
    }else{
        flag |= CL_MEM_READ_WRITE;
    }

    cl_mem mem = clCreateFromGLTexture(context, flag, GL_TEXTURE_2D, 0, texture.getID(), &error);
    if(error != CL_SUCCESS){
        std::cout << "error while creating clMemory from GL texture: " << error << std::endl;
        return false;
    }

    sharedObjects.push_back(mem);

    return true;
}

