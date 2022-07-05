#pragma once
#include "../Cmake.h"
#if CMAKE_COMPILER_NUMBER == 1
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wunused-function"
#endif
#if CMAKE_COMPILER_NUMBER == 2
#pragma warning( push )
#pragma warning( disable : 4505)
#pragma warning( disable : 4430)
#pragma warning( disable : 4324)
#pragma warning( disable : 4189)
#pragma warning( disable : 4127)
#pragma warning( disable : 4100)
#pragma warning( disable : 4083)
#endif
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_profiles.hpp>
#if CMAKE_COMPILER_NUMBER == 1
#pragma GCC diagnostic pop
#endif
#if CMAKE_COMPILER_NUMBER == 2
#pragma warning( pop )
#endif
#if CMAKE_SYSTEM_NUMBER == 0
#include "../Linux/Window.h"
#include <vulkan/vulkan_wayland.h>
#define VK_KHR_WSI_SURFACE_EXTENSION_NAME VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#endif
#if CMAKE_SYSTEM_NUMBER == 1
#include "../Windows/Window.h"
#include <vulkan/vulkan_win32.h>
#define VK_KHR_WSI_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif