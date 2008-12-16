#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "OpenCL/cl.h"

#include <vector>
#include <string>

struct DeviceInfo
{
   cl_uint type;                //CL_DEVICE_TYPE

   cl_uint vendorId;              //CL_DEVICE_VENDOR_ID
   cl_uint maxComputeUnits;       //CL_DEVICE_MAX_COMPUTE_UNITS
   cl_uint maxWorkItemDimensions; //CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS
   std::vector<size_t> maxWorkItemSizes; //CL_DEVICE_MAX_WORK_ITEM_SIZES
   size_t maxWorkGroupSize;      //CL_DEVICE_MAX_WORK_GROUP_SIZE
   cl_uint preferredVectorWidthChar; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
   cl_uint preferredVectorWidthShort; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
   cl_uint preferredVectorWidthInt; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
   cl_uint preferredVectorWidthLong; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
   cl_uint preferredVectorWidthFloat; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
   cl_uint preferredVectorWidthDouble; //CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE

   cl_uint maxClockFrequency;   //CL_DEVICE_MAX_CLOCK_FREQUENCY
   cl_bitfield addressBits;     //CL_DEVICE_ADDRESS_BITS
   cl_uint maxReadImageArgs;    //CL_DEVICE_MAX_READ_IMAGE_ARGS
   cl_uint maxWriteImageArgs;   //CL_DEVICE_MAX_WRITE_IMAGE_ARGS
   cl_ulong maxMemAllocSize;    //CL_DEVICE_MAX_MEM_ALLOC_SIZE

   size_t image2dMaxWidth;     //CL_DEVICE_IMAGE2D_MAX_WIDTH
   size_t image2dMaxHeight;    //CL_DEVICE_IMAGE2D_MAX_HEIGHT
   size_t image3dMaxWidth;     //CL_DEVICE_IMAGE3D_MAX_WIDTH
   size_t image3dMaxHeight;    //CL_DEVICE_IMAGE3D_MAX_HEIGHT
   size_t image3dMaxDepth;     //CL_DEVICE_IMAGE3D_MAX_DEPTH
   cl_bool imageSupport;       //CL_DEVICE_IMAGE_SUPPORT

   size_t maxParameterSize;       //CL_DEVICE_MAX_PARAMETER_SIZE
   cl_uint maxSamplers;            //CL_DEVICE_MAX_SAMPLERS
   cl_uint memBaseAddrAlign;       //CL_DEVICE_MEM_BASE_ADDR_ALIGN
   cl_uint minDataTypeAlignSize;   //CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
   cl_device_fp_config singleFpConfig; //CL_DEVICE_SINGLE_FP_CONFIG
   cl_device_mem_cache_type globalMemCacheType; //CL_DEVICE_GLOBAL_MEM_CACHE_TYPE
   cl_uint globalMemCachelineSize; //CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE
   cl_ulong globalMemCacheSize;     //CL_DEVICE_GLOBAL_MEM_CACHE_SIZE
   cl_ulong globalMemSize;          //CL_DEVICE_GLOBAL_MEM_SIZE
   cl_ulong maxConstantBufferSize;  //CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE
   cl_uint maxConstantArgs;        //CL_DEVICE_MAX_CONSTANT_ARGS
   cl_device_local_mem_type localMemType; //CL_DEVICE_LOCAL_MEM_TYPE
   cl_ulong localMemSize;           //CL_DEVICE_LOCAL_MEM_SIZE
   cl_bool errorCorrectionSupport; //CL_DEVICE_ERROR_CORRECTION_SUPPORT
   size_t profilingTimerResolution; //CL_DEVICE_PROFILING_TIMER_RESOLUTION
   cl_bool entianLittle;           //CL_DEVICE_ENDIAN_LITTLE
   cl_bool available;              //CL_DEVICE_AVAILABLE
   cl_bool compilerAvailable;      //CL_DEVICE_COMPILER_AVAILABLE
   cl_device_exec_capabilities executionCapabilities;  //CL_DEVICE_EXECUTION_CAPABILITIES
   cl_command_queue_properties queueProperties; //CL_DEVICE_QUEUE_PROPERTIES

   std::string name;            //CL_DEVICE_NAME
   std::string vendor;          //CL_DEVICE_VENDOR
   std::string driverVersion;   //CL_DRIVER_VERSION
   std::string profile;         //CL_DEVICE_PROFILE
   std::string version;         //CL_DEVICE_VERSION
   std::string extensions;      //CL_DEVICE_EXTENSIONS
};

#endif
