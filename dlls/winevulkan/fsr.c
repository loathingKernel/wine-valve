#if 0
#pragma makedep unix
#endif

#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>

#include "vulkan_private.h"

#include "fsr_blit_comp_spv.h"
#include "fsr_easu_comp_spv.h"
#include "fsr_easu_fp16_comp_spv.h"
#include "fsr_easu_lite_comp_spv.h"
#include "fsr_easu_lite_fp16_comp_spv.h"
#include "fsr_rcas_comp_spv.h"
#include "fsr_rcas_fp16_comp_spv.h"

WINE_DEFAULT_DEBUG_CHANNEL(vulkan);

static void destroy_pipeline(VkDevice device, struct fs_comp_pipeline *pipeline)
{
    device->funcs.p_vkDestroyPipeline(device->device, pipeline->pipeline, NULL);
    pipeline->pipeline = VK_NULL_HANDLE;

    device->funcs.p_vkDestroyPipelineLayout(device->device, pipeline->pipeline_layout, NULL);
    pipeline->pipeline_layout = VK_NULL_HANDLE;
}

static VkResult create_pipeline(VkDevice device, struct VkSwapchainKHR_T *swapchain,
                                const uint32_t *code, uint32_t code_size, uint32_t push_size, struct fs_comp_pipeline *pipeline)
{
    #if defined(USE_STRUCT_CONVERSION)
    VkComputePipelineCreateInfo_host pipelineInfo = {0};
    #else
    VkComputePipelineCreateInfo pipelineInfo = {0};
    #endif
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {0};
    VkShaderModuleCreateInfo shaderInfo = {0};
    VkPushConstantRange pushConstants;
    VkShaderModule shaderModule = 0;
    VkResult res;

    pipeline->push_size = push_size;

    pushConstants.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstants.offset = 0;
    pushConstants.size = push_size;

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &swapchain->descriptor_set_layout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstants;

    res = device->funcs.p_vkCreatePipelineLayout(device->device, &pipelineLayoutInfo, NULL, &pipeline->pipeline_layout);
    if(res != VK_SUCCESS)
    {
        ERR("vkCreatePipelineLayout: %d\n", res);
        goto fail;
    }

    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = code_size;
    shaderInfo.pCode = code;

    res = device->funcs.p_vkCreateShaderModule(device->device, &shaderInfo, NULL, &shaderModule);
    if(res != VK_SUCCESS)
    {
        ERR("vkCreateShaderModule: %d\n", res);
        goto fail;
    }

    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    pipelineInfo.stage.module = shaderModule;
    pipelineInfo.stage.pName = "main";
    pipelineInfo.layout = pipeline->pipeline_layout;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    res = device->funcs.p_vkCreateComputePipelines(device->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline->pipeline);
    if(res == VK_SUCCESS)
        goto out;

    ERR("vkCreateComputePipelines: %d\n", res);

    fail:
    destroy_pipeline(device, pipeline);

    out:
    device->funcs.p_vkDestroyShaderModule(device->device, shaderModule, NULL);

    return res;
}

static VkResult create_descriptor_set(VkDevice device, struct VkSwapchainKHR_T *swapchain, struct fs_hack_image *hack)
{
    VkResult res;
    #if defined(USE_STRUCT_CONVERSION)
    VkDescriptorSetAllocateInfo_host descriptorAllocInfo = {0};
    VkWriteDescriptorSet_host descriptorWrites[2] = {{0}, {0}};
    VkDescriptorImageInfo_host userDescriptorImageInfo = {0}, realDescriptorImageInfo = {0};
    #else
    VkDescriptorSetAllocateInfo descriptorAllocInfo = {0};
    VkWriteDescriptorSet descriptorWrites[2] = {{0}, {0}};
    VkDescriptorImageInfo userDescriptorImageInfo = {0}, realDescriptorImageInfo = {0};
    #endif

    descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorAllocInfo.descriptorPool = swapchain->descriptor_pool;
    descriptorAllocInfo.descriptorSetCount = 1;
    descriptorAllocInfo.pSetLayouts = &swapchain->descriptor_set_layout;

    res = device->funcs.p_vkAllocateDescriptorSets(device->device, &descriptorAllocInfo, &hack->descriptor_set);
    if(res != VK_SUCCESS){
        ERR("vkAllocateDescriptorSets: %d\n", res);
        return res;
    }

    userDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    userDescriptorImageInfo.imageView = hack->user_view;
    userDescriptorImageInfo.sampler = swapchain->sampler;

    realDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    realDescriptorImageInfo.imageView = hack->fsr_view;

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = hack->descriptor_set;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pImageInfo = &userDescriptorImageInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = hack->descriptor_set;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &realDescriptorImageInfo;

    device->funcs.p_vkUpdateDescriptorSets(device->device, 2, descriptorWrites, 0, NULL);

    res = device->funcs.p_vkAllocateDescriptorSets(device->device, &descriptorAllocInfo, &hack->fsr_set);
    if (res != VK_SUCCESS)
    {
        ERR("vkAllocateDescriptorSets: %d\n", res);
        return res;
    }

    userDescriptorImageInfo.imageView = hack->fsr_view;

    realDescriptorImageInfo.imageView = hack->swapchain_view;

    descriptorWrites[0].dstSet = hack->fsr_set;
    descriptorWrites[1].dstSet = hack->fsr_set;

    device->funcs.p_vkUpdateDescriptorSets(device->device, 2, descriptorWrites, 0, NULL);

    return VK_SUCCESS;
}

static VkFormat srgb_to_unorm(VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_R8G8B8A8_SRGB: return VK_FORMAT_R8G8B8A8_UNORM;
        case VK_FORMAT_B8G8R8A8_SRGB: return VK_FORMAT_B8G8R8A8_UNORM;
        case VK_FORMAT_R8G8B8_SRGB: return VK_FORMAT_R8G8B8_UNORM;
        case VK_FORMAT_B8G8R8_SRGB: return VK_FORMAT_B8G8R8_UNORM;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
        default: return format;
    }
}

static VkResult init_compute_state(VkDevice device, struct VkSwapchainKHR_T *swapchain)
{
    VkResult res;
    VkSamplerCreateInfo samplerInfo = {0};
    VkDescriptorPoolSize poolSizes[2] = {{0}, {0}};
    VkDescriptorPoolCreateInfo poolInfo = {0};
    VkDescriptorSetLayoutBinding layoutBindings[2] = {{0}, {0}};
    VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo = {0};
    VkDeviceSize fsrMemTotal = 0, offs;
    VkImageCreateInfo imageInfo = {0};
    #if defined(USE_STRUCT_CONVERSION)
    VkMemoryRequirements_host fsrMemReq;
    VkMemoryAllocateInfo_host allocInfo = {0};
    VkPhysicalDeviceMemoryProperties_host memProperties;
    VkImageViewCreateInfo_host viewInfo = {0};
    #else
    VkMemoryRequirements fsrMemReq;
    VkMemoryAllocateInfo allocInfo = {0};
    VkPhysicalDeviceMemoryProperties memProperties;
    VkImageViewCreateInfo viewInfo = {0};
    #endif
    uint32_t fsr_memory_type = -1, i;

    VkPhysicalDeviceVulkan12Features vulkan12Features = {0};
    VkPhysicalDeviceFeatures2 features2 = {0};

    vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &vulkan12Features;
    device->phys_dev->instance->funcs.p_vkGetPhysicalDeviceFeatures2(device->phys_dev->phys_dev, &features2);
    swapchain->upscaler.fsr.fp16 = vulkan12Features.shaderFloat16 && features2.features.shaderInt16;
    if (swapchain->upscaler.fsr.fp16)
        WARN("Device supports fp16\n");
    else
        WARN("Device DOES NOT support fp16\n");

    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = swapchain->fs_hack_filter;
    samplerInfo.minFilter = swapchain->fs_hack_filter;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    res = device->funcs.p_vkCreateSampler(device->device, &samplerInfo, NULL, &swapchain->sampler);
    if(res != VK_SUCCESS)
    {
        WARN("vkCreateSampler failed, res=%d\n", res);
        return res;
    }

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = swapchain->n_images;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[1].descriptorCount = swapchain->n_images;

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 2;
    poolInfo.pPoolSizes = poolSizes;
    poolInfo.maxSets = swapchain->n_images;

    poolSizes[0].descriptorCount *= 2;
    poolSizes[1].descriptorCount *= 2;
    poolInfo.maxSets *= 2;

    res = device->funcs.p_vkCreateDescriptorPool(device->device, &poolInfo, NULL, &swapchain->descriptor_pool);
    if(res != VK_SUCCESS){
        ERR("vkCreateDescriptorPool: %d\n", res);
        goto fail;
    }

    layoutBindings[0].binding = 0;
    layoutBindings[0].descriptorCount = 1;
    layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBindings[0].pImmutableSamplers = NULL;
    layoutBindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    layoutBindings[1].binding = 1;
    layoutBindings[1].descriptorCount = 1;
    layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    layoutBindings[1].pImmutableSamplers = NULL;
    layoutBindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    descriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayoutInfo.bindingCount = 2;
    descriptorLayoutInfo.pBindings = layoutBindings;

    res = device->funcs.p_vkCreateDescriptorSetLayout(device->device, &descriptorLayoutInfo, NULL, &swapchain->descriptor_set_layout);
    if(res != VK_SUCCESS){
        ERR("vkCreateDescriptorSetLayout: %d\n", res);
        goto fail;
    }

    res = create_pipeline(device, swapchain, fsr_blit_comp_spv, sizeof(fsr_blit_comp_spv), 4 * sizeof(float) /* 2 * vec2 */, &swapchain->fsr_blit_pipeline);
    if(res != VK_SUCCESS)
        goto fail;

    if (swapchain->upscaler.fsr.fp16) {
        if (swapchain->upscaler.fsr.lite)
            res = create_pipeline(device, swapchain, fsr_easu_lite_fp16_comp_spv, sizeof(fsr_easu_lite_fp16_comp_spv), 16 * sizeof(uint32_t) /* 4 * uvec4 */, &swapchain->fsr_easu_pipeline);
        else
            res = create_pipeline(device, swapchain, fsr_easu_fp16_comp_spv, sizeof(fsr_easu_fp16_comp_spv), 16 * sizeof(uint32_t) /* 4 * uvec4 */, &swapchain->fsr_easu_pipeline);
    }else{
        if (swapchain->upscaler.fsr.lite)
            res = create_pipeline(device, swapchain, fsr_easu_lite_comp_spv, sizeof(fsr_easu_lite_comp_spv), 16 * sizeof(uint32_t) /* 4 * uvec4 */, &swapchain->fsr_easu_pipeline);
        else
            res = create_pipeline(device, swapchain, fsr_easu_comp_spv, sizeof(fsr_easu_comp_spv), 16 * sizeof(uint32_t) /* 4 * uvec4 */, &swapchain->fsr_easu_pipeline);
    }
    if (res != VK_SUCCESS)
        goto fail;

    if (swapchain->upscaler.fsr.fp16)
        res = create_pipeline(device, swapchain, fsr_rcas_fp16_comp_spv, sizeof(fsr_rcas_fp16_comp_spv), 8 * sizeof(uint32_t) /* uvec4 + ivec4 */, &swapchain->fsr_rcas_pipeline);
    else
        res = create_pipeline(device, swapchain, fsr_rcas_comp_spv, sizeof(fsr_rcas_comp_spv), 8 * sizeof(uint32_t) /* uvec4 + ivec4 */, &swapchain->fsr_rcas_pipeline);
    if (res != VK_SUCCESS)
        goto fail;

    /* create intermediate fsr images */
    for(i = 0; i < swapchain->n_images; ++i){
        struct fs_hack_image *hack = &swapchain->fs_hack_images[i];

        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = swapchain->blit_dst.extent.width;
        imageInfo.extent.height = swapchain->blit_dst.extent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        res = device->funcs.p_vkCreateImage(device->device, &imageInfo, NULL, &hack->fsr_image);
        if(res != VK_SUCCESS){
            ERR("vkCreateImage failed: %d\n", res);
            goto fail;
        }

        device->funcs.p_vkGetImageMemoryRequirements(device->device, hack->fsr_image, &fsrMemReq);

        offs = fsrMemTotal % fsrMemReq.alignment;
        if(offs)
            fsrMemTotal += fsrMemReq.alignment - offs;

        fsrMemTotal += fsrMemReq.size;
    }

    /* allocate backing memory */
    device->phys_dev->instance->funcs.p_vkGetPhysicalDeviceMemoryProperties(device->phys_dev->phys_dev, &memProperties);

    for(i = 0; i < memProperties.memoryTypeCount; i++){
        if((memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT){
            if(fsrMemReq.memoryTypeBits & (1 << i)){
                fsr_memory_type = i;
                break;
            }
        }
    }

    if(fsr_memory_type == -1){
        ERR("unable to find suitable memory type\n");
        res = VK_ERROR_OUT_OF_HOST_MEMORY;
        goto fail;
    }

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = fsrMemTotal;
    allocInfo.memoryTypeIndex = fsr_memory_type;

    res = device->funcs.p_vkAllocateMemory(device->device, &allocInfo, NULL, &swapchain->fsr_image_memory);
    if(res != VK_SUCCESS){
        ERR("vkAllocateMemory: %d\n", res);
        goto fail;
    }

    /* bind backing memory and create imageviews */
    fsrMemTotal = 0;
    for(i = 0; i < swapchain->n_images; ++i){
        struct fs_hack_image *hack = &swapchain->fs_hack_images[i];

        device->funcs.p_vkGetImageMemoryRequirements(device->device, hack->fsr_image, &fsrMemReq);

        offs = fsrMemTotal % fsrMemReq.alignment;
        if(offs)
            fsrMemTotal += fsrMemReq.alignment - offs;

        res = device->funcs.p_vkBindImageMemory(device->device, hack->fsr_image, swapchain->fsr_image_memory, fsrMemTotal);
        if(res != VK_SUCCESS){
            ERR("vkBindImageMemory: %d\n", res);
            goto fail;
        }

        fsrMemTotal += fsrMemReq.size;
    }

    /* create imageviews */
    for (i = 0; i < swapchain->n_images; ++i)
    {
        struct fs_hack_image *hack = &swapchain->fs_hack_images[i];

        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = hack->fsr_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        res = device->funcs.p_vkCreateImageView(device->device, &viewInfo, NULL, &hack->fsr_view);
        if(res != VK_SUCCESS)
        {
            ERR("vkCreateImageView(blit): %d\n", res);
            goto fail;
        }
    }


    /* create imageviews */
    for(i = 0; i < swapchain->n_images; ++i){
        struct fs_hack_image *hack = &swapchain->fs_hack_images[i];

        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = hack->swapchain_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = srgb_to_unorm(swapchain->format);
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        res = device->funcs.p_vkCreateImageView(device->device, &viewInfo, NULL, &hack->swapchain_view);
        if(res != VK_SUCCESS){
            ERR("vkCreateImageView(blit): %d\n", res);
            goto fail;
        }

        res = create_descriptor_set(device, swapchain, hack);
        if(res != VK_SUCCESS)
            goto fail;
    }

    return VK_SUCCESS;

    fail:
    for(i = 0; i < swapchain->n_images; ++i){
        struct fs_hack_image *hack = &swapchain->fs_hack_images[i];

        device->funcs.p_vkDestroyImageView(device->device, hack->fsr_view, NULL);
        hack->fsr_view = VK_NULL_HANDLE;

        device->funcs.p_vkDestroyImageView(device->device, hack->swapchain_view, NULL);
        hack->swapchain_view = VK_NULL_HANDLE;

        device->funcs.p_vkDestroyImage(device->device, hack->fsr_image, NULL);
        hack->fsr_image = VK_NULL_HANDLE;
    }

    destroy_pipeline(device, &swapchain->fsr_blit_pipeline);
    destroy_pipeline(device, &swapchain->fsr_easu_pipeline);
    destroy_pipeline(device, &swapchain->fsr_rcas_pipeline);

    device->funcs.p_vkDestroyDescriptorSetLayout(device->device, swapchain->descriptor_set_layout, NULL);
    swapchain->descriptor_set_layout = VK_NULL_HANDLE;

    device->funcs.p_vkDestroyDescriptorPool(device->device, swapchain->descriptor_pool, NULL);
    swapchain->descriptor_pool = VK_NULL_HANDLE;

    device->funcs.p_vkFreeMemory(device->device, swapchain->fsr_image_memory, NULL);
    swapchain->fsr_image_memory = VK_NULL_HANDLE;

    device->funcs.p_vkDestroySampler(device->device, swapchain->sampler, NULL);
    swapchain->sampler = VK_NULL_HANDLE;

    return res;
}

static void bind_pipeline(VkDevice device, VkCommandBuffer cmd, struct fs_comp_pipeline *pipeline, VkDescriptorSet set, void *push_data)
{
    device->funcs.p_vkCmdBindPipeline(cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->pipeline);

    device->funcs.p_vkCmdBindDescriptorSets(cmd,
        VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->pipeline_layout,
        0, 1, &set, 0, NULL);

    device->funcs.p_vkCmdPushConstants(cmd,
        pipeline->pipeline_layout, VK_SHADER_STAGE_COMPUTE_BIT,
        0, pipeline->push_size, push_data);
}

#if defined(USE_STRUCT_CONVERSION)
static void init_barrier(VkImageMemoryBarrier_host *barrier)
#else
static void init_barrier(VkImageMemoryBarrier *barrier)
#endif
{
    barrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier->pNext = NULL;
    barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier->subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier->subresourceRange.baseMipLevel = 0;
    barrier->subresourceRange.levelCount = 1;
    barrier->subresourceRange.baseArrayLayer = 0;
    barrier->subresourceRange.layerCount = 1;
}

static VkResult record_compute_cmd(VkDevice device, struct VkSwapchainKHR_T *swapchain, struct fs_hack_image *hack)
{
    #if defined(USE_STRUCT_CONVERSION)
    VkImageMemoryBarrier_host barriers[3] = {{0}};
    VkCommandBufferBeginInfo_host beginInfo = {0};
    #else
    VkImageMemoryBarrier barriers[3] = {{0}};
    VkCommandBufferBeginInfo beginInfo = {0};
    #endif
    union
    {
        uint32_t uint[16];
        float    fp[16];
    } c;
    VkResult result;

    TRACE("recording compute command\n");

    init_barrier(&barriers[0]);
    init_barrier(&barriers[1]);
    init_barrier(&barriers[2]);

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    device->funcs.p_vkBeginCommandBuffer(hack->cmd, &beginInfo);

    /* 1st pass (easu) */
    /* transition user image from PRESENT_SRC to SHADER_READ */
    barriers[0].oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barriers[0].image = hack->user_image;
    barriers[0].srcAccessMask = 0;
    barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    /* storage image... */
    /* transition fsr image from whatever to GENERAL */
    barriers[1].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barriers[1].newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barriers[1].image = hack->swapchain_image;
    barriers[1].srcAccessMask = 0;
    barriers[1].dstAccessMask = 0;

    device->funcs.p_vkCmdPipelineBarrier(
        hack->cmd,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        0,
        0, NULL,
        0, NULL,
        2, barriers
    );

    /* perform easu shader */

    c.fp[0] = swapchain->user_extent.width * (1.0f / swapchain->blit_dst.extent.width);
    c.fp[1] = swapchain->user_extent.height * (1.0f / swapchain->blit_dst.extent.height);
    c.fp[2] = 0.5f * c.fp[0] - 0.5f;
    c.fp[3] = 0.5f * c.fp[1] - 0.5f;
    // Viewport pixel position to normalized image space.
    // This is used to get upper-left of 'F' tap.
    c.fp[4] = 1.0f / swapchain->user_extent.width;
    c.fp[5] = 1.0f / swapchain->user_extent.height;
    // Centers of gather4, first offset from upper-left of 'F'.
    //      +---+---+
    //      |   |   |
    //      +--(0)--+
    //      | b | c |
    //  +---F---+---+---+
    //  | e | f | g | h |
    //  +--(1)--+--(2)--+
    //  | i | j | k | l |
    //  +---+---+---+---+
    //      | n | o |
    //      +--(3)--+
    //      |   |   |
    //      +---+---+
    c.fp[6] =  1.0f * c.fp[4];
    c.fp[7] = -1.0f * c.fp[5];
    // These are from (0) instead of 'F'.
    c.fp[8] = -1.0f * c.fp[4];
    c.fp[9] =  2.0f * c.fp[5];
    c.fp[10] =  1.0f * c.fp[4];
    c.fp[11] =  2.0f * c.fp[5];
    c.fp[12] =  0.0f * c.fp[4];
    c.fp[13] =  4.0f * c.fp[5];
    c.uint[14] = swapchain->blit_dst.extent.width;
    c.uint[15] = swapchain->blit_dst.extent.height;

    bind_pipeline(device, hack->cmd, &swapchain->fsr_easu_pipeline, hack->descriptor_set, c.uint);

    /* local sizes in shader are 8 */
    device->funcs.p_vkCmdDispatch(hack->cmd, ceil(swapchain->blit_dst.extent.width / 8.),
                                  ceil(swapchain->blit_dst.extent.height / 8.), 1);

    /* transition user image from SHADER_READ back to PRESENT_SRC */
    barriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barriers[0].newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barriers[0].image = hack->user_image;
    barriers[0].srcAccessMask = 0;
    barriers[0].dstAccessMask = 0;

    /* transition fsr image from GENERAL to SHADER_READ */
    barriers[1].oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barriers[1].image = hack->swapchain_image;
    barriers[1].srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    /* transition swapchain image from whatever to GENERAL */
    barriers[2].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barriers[2].newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barriers[2].image = hack->swapchain_image;
    barriers[2].srcAccessMask = 0;
    barriers[2].dstAccessMask = 0;

    device->funcs.p_vkCmdPipelineBarrier(
        hack->cmd,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0,
        0, NULL,
        0, NULL,
        3, barriers
    );

    /* 2nd pass (rcas) */

    c.fp[0] = exp2f(-swapchain->upscaler.fsr.sharpness);
    c.uint[2] = swapchain->blit_dst.extent.width;
    c.uint[3] = swapchain->blit_dst.extent.height;
    c.uint[4] = swapchain->blit_dst.offset.x;
    c.uint[5] = swapchain->blit_dst.offset.y;
    c.uint[6] = swapchain->blit_dst.offset.x + swapchain->blit_dst.extent.width;
    c.uint[7] = swapchain->blit_dst.offset.y + swapchain->blit_dst.extent.height;

    bind_pipeline(device, hack->cmd, &swapchain->fsr_rcas_pipeline, hack->fsr_set, c.uint);

    /* local sizes in shader are 8 */
    device->funcs.p_vkCmdDispatch(hack->cmd, ceil(swapchain->real_extent.width / 8.),
                                  ceil(swapchain->real_extent.height / 8.), 1);

    /* transition swapchain image from GENERAL to PRESENT_SRC */
    barriers[0].oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barriers[0].image = hack->swapchain_image;
    barriers[0].srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    barriers[0].dstAccessMask = 0;

    device->funcs.p_vkCmdPipelineBarrier(
        hack->cmd,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
        0,
        0, NULL,
        0, NULL,
        1, barriers
    );

    result = device->funcs.p_vkEndCommandBuffer(hack->cmd);
    if (result != VK_SUCCESS)
    {
        ERR("vkEndCommandBuffer: %d\n", result);
        return result;
    }

    return VK_SUCCESS;
}

static void destroy_compute_state(VkDevice device, struct VkSwapchainKHR_T *swapchain)
{
    destroy_pipeline(device, &swapchain->fsr_blit_pipeline);
    destroy_pipeline(device, &swapchain->fsr_easu_pipeline);
    destroy_pipeline(device, &swapchain->fsr_rcas_pipeline);
    device->funcs.p_vkFreeMemory(device->device, swapchain->fsr_image_memory, NULL);
}

struct upscaler_implementation fsr_upscaler = {
    init_compute_state,
    record_compute_cmd,
    destroy_compute_state
};
