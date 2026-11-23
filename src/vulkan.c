#include "recompvr/vulkan.h"

#include <stddef.h>
#if defined(XR_USE_GRAPHICS_API_OPENGL_ES)
#include <EGL/egl.h>
#endif
#if defined(XR_USE_PLATFORM_ANDROID)
#include <jni.h>
#endif

#ifndef XR_USE_GRAPHICS_API_VULKAN
#define XR_USE_GRAPHICS_API_VULKAN
#endif
#include <openxr/openxr_platform.h>

XrResult recompvr_vulkan_graphics_device(XrInstance instance, XrSystemId system,
                                        VkInstance vulkan, VkPhysicalDevice *physical)
{
    PFN_xrVoidFunction address = NULL;
    const XrResult result = xrGetInstanceProcAddr(instance, "xrGetVulkanGraphicsDevice2KHR", &address);
    if (XR_FAILED(result))
        return result;
    if (!address)
        return XR_ERROR_FUNCTION_UNSUPPORTED;

    const XrVulkanGraphicsDeviceGetInfoKHR info = { .type = XR_TYPE_VULKAN_GRAPHICS_DEVICE_GET_INFO_KHR,
        .systemId = system, .vulkanInstance = vulkan };
    return ((PFN_xrGetVulkanGraphicsDevice2KHR)address)(instance, &info, physical);
}
