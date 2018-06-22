//
//  OpenCL.hpp
//  OpenCL
//
//  Created by Nathan Touroux on 27/03/2016.
//  Copyright © 2016 Nathan Touroux. All rights reserved.
//

#ifndef OpenCL_hpp
#define OpenCL_hpp

#include <iostream>
#include <vector>
#include <fstream>

#include <OpenCL/opencl.h>

struct Device{
    std::string name;
    cl_device_type type;
};

struct Mem{
    cl_mem mem;
    void *data;
    size_t length;
    bool read = false;
    bool write = false;
};

class OpenCL{
private:
    std::vector<std::string> platforms;
    std::vector<Device> devices;
    std::vector<cl_device_id> deviceIds;
    std::vector<Mem> mems;
    cl_context context;
    cl_program program;
    cl_kernel kernel;
    cl_device_id device;
    
    cl_command_queue queue;
    
public:
    OpenCL();
    ~OpenCL();
    
    bool init(cl_device_type type = CL_DEVICE_TYPE_GPU);// CL_DEVICE_TYPE_GPU CL_DEVICE_TYPE_CPU
    bool buildProgram(std::string source, std::string funcName);
    void compute(std::initializer_list<size_t> globalWorkSize);
    void readData();
    
    //add memories in the right order (parametre order)
    template<typename T>
    bool addMem(std::vector<T> & data, std::string readWrite = "");
    template<typename T>
    bool addMem(T & data, std::string readWrite = "");
    bool addMem(void *data, size_t length, std::string readWrite = "");
    bool setMem(int index, void *data);
};


template<typename T>
bool OpenCL::addMem(std::vector<T> & data, std::string readWrite){
    return addMem(data.data(), data.size()*sizeof(T), readWrite);
}
template<typename T>
bool OpenCL::addMem(T & data, std::string readWrite){
    return addMem(&data, sizeof(T), readWrite);
}

#endif /* OpenCL_hpp */
