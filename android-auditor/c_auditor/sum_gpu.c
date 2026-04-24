#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// SPIR-V shader code (would be generated from sum.comp)
// For simplicity, assume we have the SPIR-V binary
// In practice, compile sum.comp with glslangValidator -V sum.comp -o sum.spv
// Then include the binary here.

const uint32_t shaderSPV[] = {
    // Placeholder: actual SPIR-V binary would go here
    // This is not real SPIR-V; you need to compile the GLSL
    0x07230203, 0x00010000, 0x00080001, 0x0000000e,
    // ... (truncated for brevity; real SPIR-V is much larger)
};

#define ARRAY_SIZE 1000000
#define WORKGROUP_SIZE 256

VkInstance instance;
VkDevice device;
VkPhysicalDevice physicalDevice;
VkQueue queue;
uint32_t queueFamilyIndex;
VkCommandPool commandPool;
VkCommandBuffer commandBuffer;
VkBuffer inputBuffer, outputBuffer;
VkDeviceMemory inputMemory, outputMemory;
VkDescriptorSetLayout descriptorSetLayout;
VkPipelineLayout pipelineLayout;
VkPipeline pipeline;
VkDescriptorPool descriptorPool;
VkDescriptorSet descriptorSet;

void createInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Sum GPU";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    const char* extensions[] = {VK_KHR_SURFACE_EXTENSION_NAME}; // Minimal
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;

    vkCreateInstance(&createInfo, NULL, &instance);
}

void pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
    physicalDevice = devices[0]; // Pick first
}

void createDevice() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueFamilyIndex = i;
            break;
        }
    }

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    vkCreateDevice(physicalDevice, &createInfo, NULL, &device);
    vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
}

void createBuffers() {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = ARRAY_SIZE * sizeof(uint32_t);
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(device, &bufferInfo, NULL, &inputBuffer);
    vkCreateBuffer(device, &bufferInfo, NULL, &outputBuffer);

    VkMemoryRequirements memReq;
    vkGetBufferMemoryRequirements(device, inputBuffer, &memReq);

    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

    uint32_t memTypeIndex = UINT32_MAX;
    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
        if ((memReq.memoryTypeBits & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
            memTypeIndex = i;
            break;
        }
    }

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;
    allocInfo.memoryTypeIndex = memTypeIndex;

    vkAllocateMemory(device, &allocInfo, NULL, &inputMemory);
    vkAllocateMemory(device, &allocInfo, NULL, &outputMemory);

    vkBindBufferMemory(device, inputBuffer, inputMemory, 0);
    vkBindBufferMemory(device, outputBuffer, outputMemory, 0);
}

void createDescriptorSet() {
    VkDescriptorSetLayoutBinding bindings[2] = {};
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = bindings;

    vkCreateDescriptorSetLayout(device, &layoutInfo, NULL, &descriptorSetLayout);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout);

    VkDescriptorPoolSize poolSizes[1] = {};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = 2;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = 1;

    vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptorPool);

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSetLayout;

    vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet);

    VkDescriptorBufferInfo bufferInfo[2] = {};
    bufferInfo[0].buffer = inputBuffer;
    bufferInfo[0].offset = 0;
    bufferInfo[0].range = VK_WHOLE_SIZE;

    bufferInfo[1].buffer = outputBuffer;
    bufferInfo[1].offset = 0;
    bufferInfo[1].range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet writes[2] = {};
    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet = descriptorSet;
    writes[0].dstBinding = 0;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[0].pBufferInfo = &bufferInfo[0];

    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet = descriptorSet;
    writes[1].dstBinding = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[1].pBufferInfo = &bufferInfo[1];

    vkUpdateDescriptorSets(device, 2, writes, 0, NULL);
}

void createPipeline() {
    VkShaderModuleCreateInfo shaderInfo = {};
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = sizeof(shaderSPV);
    shaderInfo.pCode = shaderSPV;

    VkShaderModule shaderModule;
    vkCreateShaderModule(device, &shaderInfo, NULL, &shaderModule);

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    pipelineInfo.stage.module = shaderModule;
    pipelineInfo.stage.pName = "main";
    pipelineInfo.layout = pipelineLayout;

    vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline);

    vkDestroyShaderModule(device, shaderModule, NULL);
}

void createCommandBuffer() {
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    vkCreateCommandPool(device, &poolInfo, NULL, &commandPool);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &descriptorSet, 0, NULL);

    uint32_t numWorkgroups = (ARRAY_SIZE + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    vkCmdDispatch(commandBuffer, numWorkgroups, 1, 1);

    vkEndCommandBuffer(commandBuffer);
}

void run() {
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue);
}

uint32_t* mapBuffer(VkDeviceMemory memory) {
    void* data;
    vkMapMemory(device, memory, 0, VK_WHOLE_SIZE, 0, &data);
    return (uint32_t*)data;
}

void unmapBuffer(VkDeviceMemory memory) {
    vkUnmapMemory(device, memory);
}

int main() {
    createInstance();
    pickPhysicalDevice();
    createDevice();
    createBuffers();
    createDescriptorSet();
    createPipeline();
    createCommandBuffer();

    // Fill input buffer
    uint32_t* inputData = mapBuffer(inputMemory);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        inputData[i] = i % 100;
    }
    unmapBuffer(inputMemory);

    clock_t start = clock();
    run();
    clock_t end = clock();
    double gpuTime = (double)(end - start) / CLOCKS_PER_SEC;

    // Read output
    uint32_t* outputData = mapBuffer(outputMemory);
    uint32_t total = 0;
    uint32_t numWorkgroups = (ARRAY_SIZE + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE;
    for (uint32_t i = 0; i < numWorkgroups; i++) {
        total += outputData[i];
    }
    unmapBuffer(outputMemory);

    printf("GPU sum: %u, time: %f\n", total, gpuTime);

    // Cleanup
    vkDestroyBuffer(device, inputBuffer, NULL);
    vkDestroyBuffer(device, outputBuffer, NULL);
    vkFreeMemory(device, inputMemory, NULL);
    vkFreeMemory(device, outputMemory, NULL);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, NULL);
    vkDestroyPipelineLayout(device, pipelineLayout, NULL);
    vkDestroyPipeline(device, pipeline, NULL);
    vkDestroyDescriptorPool(device, descriptorPool, NULL);
    vkDestroyCommandPool(device, commandPool, NULL);
    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);

    return 0;
}