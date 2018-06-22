//
//  OpenCL.cpp
//  OpenCL
//
//  Created by Nathan Touroux on 27/03/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#include "OpenCL.hpp"

using namespace std;

OpenCL::OpenCL(): context(0), program(0), kernel(0), device(0){
    
}
OpenCL::~OpenCL(){
    clFlush(queue);
    clFinish(queue);
    clReleaseContext(context);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    for(Mem mem:mems)
        clReleaseMemObject(mem.mem);
}

bool OpenCL::init(cl_device_type type){
    cl_uint platformIdCount = 0;
    clGetPlatformIDs (0, nullptr, &platformIdCount);
    if(platformIdCount == 0)return false;
    vector<cl_platform_id> platformIds (platformIdCount);
    clGetPlatformIDs (platformIdCount, platformIds.data(), nullptr);
    
    char info[50];
    size_t size;
    for(cl_platform_id id: platformIds){
        clGetPlatformInfo(id, CL_PLATFORM_NAME, sizeof(info), &info, &size);
        platforms.push_back(string(info));
    }
    
    cl_uint deviceIdCount = 0;
    clGetDeviceIDs (platformIds [0], CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceIdCount);
    if(deviceIdCount == 0)return false;
    for(int i = 0; i<deviceIdCount;i++)
        deviceIds.push_back(cl_device_id());
    clGetDeviceIDs (platformIds [0], CL_DEVICE_TYPE_ALL, deviceIdCount, deviceIds.data(), nullptr);
    cl_device_type t;
    device = deviceIds[0];
    for(cl_device_id id: deviceIds){
        clGetDeviceInfo(id, CL_DEVICE_NAME, sizeof(info), &info, &size);
        clGetDeviceInfo(id, CL_DEVICE_TYPE, sizeof(t), &t, &size);
        devices.push_back({string(info), t});
        if(t == type) device = id;
    }
    
    const cl_context_properties contextProperties [] = {
        CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties> (platformIds [0]), 0, 0};
    cl_int error;
    context = clCreateContext(contextProperties, deviceIdCount, deviceIds.data(), nullptr, nullptr, &error);
    if(error != CL_SUCCESS)return false;
    queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &error);
    if(error != CL_SUCCESS)return false;
    
    return true;
}

bool OpenCL::buildProgram(string source, string funcName){
    if(source.find(".cl")){
        std::ifstream in(source);
        std::string str((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
        source = str;
        in.close();
    }
    cl_int error;
    size_t length = (size_t)source.length();
    char **tab = new char*[1];
    tab[0] = (char *)source.c_str();
    program = clCreateProgramWithSource(context, 1, (const char**)tab, &length, &error);
    tab[0] = 0;
    delete[] tab;
    if(error != CL_SUCCESS)return false;
    error = clBuildProgram (program, (cl_uint)deviceIds.size(), deviceIds.data(), nullptr, nullptr, nullptr);
     // build info
    cl_build_status status;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &status, NULL);
    
    // check build log
    size_t logSize;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
    char programLog[logSize+1];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize+1, programLog, NULL);
    if(logSize > 1)
        printf("Build info: error=%d, status=%d, programLog:\n\n%s", error, status, programLog);
    if (error != CL_SUCCESS) return false;
    
    kernel = clCreateKernel (program, funcName.c_str(), &error);
    if(error != CL_SUCCESS)return false;
    
    return true;
}
void OpenCL::compute(initializer_list<size_t> globalWorkSize){
    vector<size_t> sizes;
    for(size_t s: globalWorkSize)
        sizes.push_back(s);
    if(globalWorkSize.size()==0)return;
    for(int i = 0;i<mems.size();i++)
        clSetKernelArg (kernel, i, sizeof(cl_mem), &mems[i].mem);
    clEnqueueNDRangeKernel (queue, kernel, (cl_uint)sizes.size(), 0, sizes.data(), 0, 0, nullptr, nullptr);
}
void OpenCL::readData(){
    for(Mem mem:mems)
        if(mem.write){
            clEnqueueReadBuffer(queue, mem.mem, CL_TRUE, 0, mem.length, mem.data, 0, nullptr, nullptr);
        }
}

bool OpenCL::addMem(void *data, size_t length, string readWrite){
    cl_int error;
    cl_mem_flags flag = 0;
    Mem m;
    m.data = data;
    m.length = length;
    if(readWrite == "r"){
        flag |= CL_MEM_READ_ONLY;
        m.read = true;
    }else if(readWrite == "w"){
        flag |= CL_MEM_WRITE_ONLY;
        m.write = true;
    }else{
        flag |= CL_MEM_READ_WRITE;
        m.read = m.write = true;
    }
    
    cl_mem buffer = clCreateBuffer (context, flag, m.length, NULL, &error);
    if(error != CL_SUCCESS){
        std::cout << "error : " << error << std::endl;
        return false;
    }
    error = clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, m.length, data, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        std::cout << "write error : " << error << std::endl;
        return false;
    }
    m.mem = buffer;
    mems.push_back(m);
    
    return true;
}

bool OpenCL::setMem(int index, void *data){
    Mem &m = mems[index];
    cl_int error = clEnqueueWriteBuffer(queue, m.mem, CL_TRUE, 0, m.length, data, 0, NULL, NULL);
    if(error != CL_SUCCESS){
        std::cout << "write error : " << error << std::endl;
        return false;
    }
    return true;
}