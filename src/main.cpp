#include <iostream>
#include <stdio.h>

#define VOLK_IMPLEMENTATION
#include <volk.h>

#include "VkBootstrap.h"

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
    VkInstance vkInstance = vkbInstance.instance;
    VkDebugUtilsMessengerEXT vkDebugMessenger = vkbInstance.debug_messenger;

    volkLoadInstance(vkInstance);

    printf("HELLO WORLD\n");
    return 0;
}