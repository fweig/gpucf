#pragma once

#include <gpucf/algorithms/ClusterFinderConfig.h>

#include <CL/cl2.hpp>

#include <args/args.hxx>
#include <filesystem/path.h>

#include <memory>
#include <string>
#include <vector>


namespace gpucf
{

class ClEnv 
{

public:
    class Flags 
    {

    public:
        args::ValueFlag<std::string> clSrcDir;    
        args::ValueFlag<size_t> gpuId;

        Flags(args::Group &required, args::Group &optional)
            : clSrcDir(required, "clsrc", "Base directory of cl source files.",
                    {'s', "src"}) 
            , gpuId(optional, "gpuid", "Id of the gpu device.", 
                    {'g', "gpu"}, 0)
        {
        }

    };


    ClEnv(const filesystem::path &srcDir, ClusterFinderConfig cfg, size_t gpuid=0);
    ClEnv(Flags &flags, ClusterFinderConfig cfg) 
        : ClEnv(args::get(flags.clSrcDir), cfg, args::get(flags.gpuId)) 
    {
    }

    cl::Context getContext() const 
    { 
        return context; 
    }

    cl::Program getProgram() const
    {
        return program;
    }

    cl::Device  getDevice()  const 
    { 
        return devices[gpuId]; 
    }


private:
    static const std::vector<filesystem::path> srcs;

    std::vector<cl::Platform> platforms;
    std::vector<cl::Device> devices;

    std::vector<std::string> defines;

    size_t gpuId;

    cl::Context context;
    cl::Program program;

    filesystem::path sourceDir;


    cl::Program buildFromSrc();
            
    cl::Program::Sources loadSrc(const std::vector<filesystem::path> &srcFiles);

    void addDefine(const std::string &);
};

}

// vim: set ts=4 sw=4 sts=4 expandtab:
