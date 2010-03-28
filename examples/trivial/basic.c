#include <CL/cl.h>
#include <stdio.h>

int main()
{
   cl_device_id   	device;
   int err;
   char deviceName[128],
      deviceVendor[128],
      deviceVersion[128];

   err = clGetDeviceIDs(CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);

   if (err) {
      fprintf(stderr, "clGetDeviceIDs failed\n");
      return -1;
   }

   err = clGetDeviceInfo(device, CL_DEVICE_NAME,
                         sizeof(deviceName), deviceName, NULL);
   err |= clGetDeviceInfo(device, CL_DEVICE_VENDOR,
                          sizeof(deviceVendor), deviceVendor, NULL);
   err |= clGetDeviceInfo(device, CL_DEVICE_VERSION,
                          sizeof(deviceVersion), deviceVersion, NULL);
   if (err) {
      fprintf(stderr, "Unable to obtain device info");
      return -1;
   }
   printf("Compute Device Name = %s\n"
          "Compute Device Vendor = %s\n"
          "Compute Device Version = %s\n",
          deviceName, deviceVendor, deviceVersion);
}
