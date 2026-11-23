#ifndef RECOMPVR_VULKAN_H
#define RECOMPVR_VULKAN_H

#include <vulkan/vulkan.h>
#include <openxr/openxr.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Requires XR_KHR_vulkan_enable2 and a Vulkan instance created for this system. */
XrResult recompvr_vulkan_graphics_device(XrInstance instance, XrSystemId system,
                                        VkInstance vulkan, VkPhysicalDevice *physical);

#ifdef __cplusplus
}
#endif
#endif
