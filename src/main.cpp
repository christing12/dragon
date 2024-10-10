#include <iostream>
#include <stdio.h>
#include <vector>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "VkBootstrap.h"


#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

static VkInstance m_vkInstance;
static VkDevice m_vkDevice;
static VkSurfaceKHR m_windowSurface;
static VkPhysicalDevice m_vkPhysGPU;
static VkQueue m_gfxQueue;
static VkSwapchainKHR m_swapchain;
static std::vector<VkImageView> m_swapchainViews;
static VkDebugUtilsMessengerEXT m_vkDebugMessenger;

static GLFWwindow* m_glfwWindow;

static void glfwErrorCallback(int error, const char* msg) {
	printf("Error: %s\n", msg);
}


void cleanup() {
    vkDestroySwapchainKHR(m_vkDevice, m_swapchain, nullptr);
    for (VkImageView& view : m_swapchainViews) {
        vkDestroyImageView(m_vkDevice, view, nullptr);
    }

    vkDestroySurfaceKHR(m_vkInstance, m_windowSurface, nullptr);
    vkDestroyDevice(m_vkDevice, nullptr);

    vkb::destroy_debug_utils_messenger(m_vkInstance, m_vkDebugMessenger, nullptr);
    vkDestroyInstance(m_vkInstance, nullptr);

    glfwDestroyWindow(m_glfwWindow);
    printf("Finished Cleaning Up\n");
}

int main() {
    VkResult result = volkInitialize();
    //VkInstance instance;
    //volkLoadInstance(instance);

    vkb::InstanceBuilder builder;
    auto instanceBuilderResult = builder.set_app_name("Vulkan Dragon")
        .request_validation_layers()
        .use_default_debug_messenger()
        .require_api_version(1, 3, 0)
        .build();

    vkb::Instance vkbInstance = instanceBuilderResult.value();
    m_vkInstance = vkbInstance.instance;
    m_vkDebugMessenger = vkbInstance.debug_messenger;

	volkLoadInstance(m_vkInstance);


	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    int width;
    int height;
    m_glfwWindow = glfwCreateWindow(1920, 1080, "Test", NULL, NULL);

    VkResult createSurfaceResult = glfwCreateWindowSurface(m_vkInstance, m_glfwWindow, NULL, &m_windowSurface);
    if (createSurfaceResult != VK_SUCCESS) {
        printf("Error creating GLFW window surface for vulkan\n");
    }
	//platformHandle = glfwGetWin32Window(handle);

	//glfwSetWindowUserPointer(handle, this);
	glfwGetFramebufferSize(m_glfwWindow, &width, &height);



	//vulkan 1.3 features
	VkPhysicalDeviceVulkan13Features features13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
	features13.dynamicRendering = true;
	features13.synchronization2 = true;

	//vulkan 1.2 features
	VkPhysicalDeviceVulkan12Features features12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

    vkb::PhysicalDeviceSelector vkbPhysDeviceSelector(vkbInstance);
    vkb::PhysicalDevice vkbPhysDevice = vkbPhysDeviceSelector.set_minimum_version(1, 3)
        .set_required_features_12(features12)
        .set_required_features_13(features13)
        .set_surface(m_windowSurface)
        .select()
        .value();

    vkb::DeviceBuilder vkbDeviceBuilder(vkbPhysDevice);
    auto logicalDeviceBuilderResult = vkbDeviceBuilder.build();
    if (!logicalDeviceBuilderResult) {
        printf("Error: Building Logical Vulkan Device failed\n");
        return 0;
    }

    vkb::Device vkbDevice = logicalDeviceBuilderResult.value();

    m_vkDevice = vkbDevice.device;
    m_vkPhysGPU = vkbPhysDevice.physical_device;

    auto getQueue = vkbDevice.get_queue(vkb::QueueType::graphics);
    if (!getQueue) {
        printf("Error getting default graphics queue from device\n");
        return 0;
    }

    m_gfxQueue = getQueue.value();



    vkb::SwapchainBuilder vkbSwapchainBuilder(m_vkPhysGPU, m_vkDevice, m_windowSurface);
	vkb::Swapchain vkbSwapchain = vkbSwapchainBuilder
		//.use_default_format_selection()
		.set_desired_format(VkSurfaceFormatKHR{ .format = VK_FORMAT_R8G8B8A8_UNORM, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
		//use vsync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(width, height)
		.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		.build()
		.value();

    m_swapchain = vkbSwapchain.swapchain;
    m_swapchainViews = vkbSwapchain.get_image_views().value();


    
    printf("HELLO WORLD\n");

    while (!glfwWindowShouldClose(m_glfwWindow)) {
        glfwPollEvents();
    }
    cleanup();
    return 0;
}