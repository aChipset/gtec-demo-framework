/****************************************************************************************************************************************************
* Copyright (c) 2016 Freescale Semiconductor, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*    * Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*
*    * Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*
*    * Neither the name of the Freescale Semiconductor, Inc. nor the names of
*      its contributors may be used to endorse or promote products derived from
*      this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************************************************************************************/

#include <FslUtil/Vulkan1_0/Util/SwapchainKHRUtil.hpp>
#include <FslUtil/Vulkan1_0/Util/PhysicalDeviceKHRUtil.hpp>
#include <FslUtil/Vulkan1_0/Exceptions.hpp>
#include <FslBase/Log/Log.hpp>
#include <RapidVulkan/Check.hpp>
#include <cassert>

namespace Fsl
{
  namespace Vulkan
  {
    namespace SwapchainKHRUtil
    {
      VUSwapchainKHR CreateSwapchain(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSwapchainCreateFlagsKHR flags,
                                     const VkSurfaceKHR surface, const uint32_t minImageCount, const uint32_t imageArrayLayers,
                                     const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount,
                                     const uint32_t* queueFamilyIndices, const VkCompositeAlphaFlagBitsKHR compositeAlpha,
                                     const VkBool32 clipped, const VkSwapchainKHR oldSwapchain)
      {
        return CreateSwapchain(physicalDevice, device, flags, surface, minImageCount, imageArrayLayers, imageUsage, imageSharingMode,
                               queueFamilyIndexCount, queueFamilyIndices, compositeAlpha, VK_PRESENT_MODE_FIFO_KHR, clipped, oldSwapchain);
      }


      VUSwapchainKHR CreateSwapchain(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSwapchainCreateFlagsKHR flags,
                                     const VkSurfaceKHR surface, const uint32_t minImageCount, const uint32_t imageArrayLayers,
                                     const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount,
                                     const uint32_t* queueFamilyIndices, const VkCompositeAlphaFlagBitsKHR compositeAlpha,
                                     const VkPresentModeKHR presentMode, const VkBool32 clipped, const VkSwapchainKHR oldSwapchain)
      {
        if (physicalDevice == VK_NULL_HANDLE || device == VK_NULL_HANDLE)
          throw std::invalid_argument("physicalDevice and device can not be VK_NULL_HANDLE");

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        RAPIDVULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities));

        VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        if (!(surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR))
        {
          preTransform = surfaceCapabilities.currentTransform;
        }

        const auto surfacePresentModes = PhysicalDeviceKHRUtil::GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface);
        const auto surfaceFormats = PhysicalDeviceKHRUtil::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface);

        VkFormat imageFormat = surfaceFormats[0].format;
        VkColorSpaceKHR imageColorSpace = surfaceFormats[0].colorSpace;

        VkSwapchainCreateInfoKHR swapchainCreateInfo{};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.flags = flags;
        swapchainCreateInfo.surface = surface;
        swapchainCreateInfo.minImageCount = minImageCount;
        swapchainCreateInfo.imageFormat = imageFormat;
        swapchainCreateInfo.imageColorSpace = imageColorSpace;
        swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
        swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
        swapchainCreateInfo.imageUsage = imageUsage;
        swapchainCreateInfo.imageSharingMode = imageSharingMode;
        swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
        swapchainCreateInfo.preTransform = preTransform;
        swapchainCreateInfo.compositeAlpha = compositeAlpha;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = clipped;
        swapchainCreateInfo.oldSwapchain = oldSwapchain;

        return VUSwapchainKHR(device, swapchainCreateInfo);
      }

      std::vector<VkImage> GetSwapchainImagesKHR(const VkDevice device, const VkSwapchainKHR swapchain)
      {
        uint32_t count;
        RAPIDVULKAN_CHECK2(vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr), "could not get swap chain images size");

        std::vector<VkImage> result(count);
        RAPIDVULKAN_CHECK2(vkGetSwapchainImagesKHR(device, swapchain, &count, result.data()), "could not get swap chain image info");
        return result;
      }
    }
  }
}