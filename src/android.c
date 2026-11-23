#include "recompvr/android.h"

#include <stddef.h>
#if defined(XR_USE_GRAPHICS_API_OPENGL_ES)
#include <EGL/egl.h>
#endif
#if defined(XR_USE_GRAPHICS_API_VULKAN)
#include <vulkan/vulkan.h>
#endif

#ifndef XR_USE_PLATFORM_ANDROID
#define XR_USE_PLATFORM_ANDROID
#endif
#include <openxr/openxr_platform.h>

XrResult recompvr_initialize_android_loader(JavaVM *vm, jobject activity)
{
    PFN_xrVoidFunction address = NULL;
    const XrResult result = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR", &address);
    if (XR_FAILED(result))
        return result;
    if (!address)
        return XR_ERROR_FUNCTION_UNSUPPORTED;

    const XrLoaderInitInfoAndroidKHR info = { .type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR,
        .applicationVM = vm, .applicationContext = activity };
    return ((PFN_xrInitializeLoaderKHR)address)((const XrLoaderInitInfoBaseHeaderKHR *)&info);
}
