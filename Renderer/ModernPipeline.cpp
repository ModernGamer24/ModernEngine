#include "ModernPipeline.h"

#include <assert.h>
#include <cinttypes>
#include <fstream>
#include <ios>
#include <iostream>
#include <vulkan/vulkan_core.h>


MEngine::GModernPipeline::GModernPipeline(GModernDevice& Device, const std::string& VertShaderFilePath, const std::string& FragShaderFilePath, const FPipelineConfigInfo& PipelineConfig) : ModernDevice(Device)
{
    CreateGraphicsPipeline(VertShaderFilePath, FragShaderFilePath, PipelineConfig);
}

void MEngine::GModernPipeline::BindPipeline(VkCommandBuffer CommandBuffer)
{
    vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline);
}

MEngine::GModernPipeline::~GModernPipeline()
{
    //Destroy graphics pipeline when class destroys
    vkDestroyShaderModule(ModernDevice.device(), VertShaderModule, nullptr);
    vkDestroyShaderModule(ModernDevice.device(), FragShaderModule, nullptr);
    vkDestroyPipeline(ModernDevice.device(), GraphicsPipeline, nullptr);
}


MEngine::FPipelineConfigInfo MEngine::GModernPipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
    FPipelineConfigInfo СonfigInfo{};

    СonfigInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    СonfigInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    СonfigInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    СonfigInfo.Viewport.x = 0.0f;
    СonfigInfo.Viewport.y = 0.0f;
    СonfigInfo.Viewport.width = static_cast<float>(width);
    СonfigInfo.Viewport.height = static_cast<float>(height);
    СonfigInfo.Viewport.minDepth = 0.0f;
    СonfigInfo.Viewport.maxDepth = 1.0f;

    СonfigInfo.Scissor.offset = {0, 0};
    СonfigInfo.Scissor.extent = {width, height};
    

    СonfigInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    СonfigInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
    СonfigInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    СonfigInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    СonfigInfo.RasterizationInfo.lineWidth = 1.0f;
    СonfigInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    СonfigInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    СonfigInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
    СonfigInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    СonfigInfo.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    СonfigInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    СonfigInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    СonfigInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
    СonfigInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    СonfigInfo.MultisampleInfo.minSampleShading = 1.0f;           // Optional
    СonfigInfo.MultisampleInfo.pSampleMask = nullptr;             // Optional
    СonfigInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    СonfigInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;

    СonfigInfo.ColorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
      VK_COLOR_COMPONENT_A_BIT;
    СonfigInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
    СonfigInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    СonfigInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    СonfigInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    СonfigInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    СonfigInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    СonfigInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
 
    СonfigInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    СonfigInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
    СonfigInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    СonfigInfo.ColorBlendInfo.attachmentCount = 1;
    СonfigInfo.ColorBlendInfo.pAttachments = &СonfigInfo.ColorBlendAttachment;
    СonfigInfo.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    СonfigInfo.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    СonfigInfo.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    СonfigInfo.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    СonfigInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    СonfigInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
    СonfigInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
    СonfigInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    СonfigInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    СonfigInfo.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
    СonfigInfo.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    СonfigInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
    СonfigInfo.DepthStencilInfo.front = {};  // Optional
    СonfigInfo.DepthStencilInfo.back = {};   // Optional
    
    

    return СonfigInfo;
}

std::vector<char> MEngine::GModernPipeline::ReadFile(const std::string& FilePath)
{
    std::ifstream ReadedFile {FilePath, std::ios::ate | std::ios::binary};

    //Check is valid FilePath
    if (!ReadedFile.is_open())
    {
        throw std::runtime_error("Failed to open file" + FilePath);
    }

    size_t FileSize = static_cast<size_t>(ReadedFile.tellg());

    std::vector<char> Buffer(FileSize);

    ReadedFile.seekg(0);
    ReadedFile.read(Buffer.data(), FileSize);

    ReadedFile.close();

    return Buffer;
}

void MEngine::GModernPipeline::CreateGraphicsPipeline(const std::string& VertShaderFilePath,
    const std::string& FragShaderFilePath, const FPipelineConfigInfo& PipelineConfig)
{
    //assert(PipelineConfig.PipelineLayout != VK_NULL_HANDLE && "Cannot create graphivs pipeline:: no pipelineLayout provided in config info");
    //assert(PipelineConfig.PipelineLayout != VK_NULL_HANDLE && "Cannot create graphivs pipeline:: no renderPass provided in config info");

    
    // Read Shader Code
    const auto VertCode = ReadFile(VertShaderFilePath);
    const auto FragCode = ReadFile(FragShaderFilePath);

    if (VertCode.size() > 0 && FragCode.size() > 0)
    {
        std::cout << "Shaders load success";
    }

    // load shaders
    CreateShaderModel(VertCode, &VertShaderModule);
    CreateShaderModel(FragCode, &FragShaderModule);
    

    // Create array of shaders
    VkPipelineShaderStageCreateInfo ShaderStages[2];

    // add Vertex shader
    ShaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ShaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    ShaderStages[0].module = VertShaderModule;
    ShaderStages[0].pName = "main";
    ShaderStages[0].flags = 0;
    ShaderStages[0].pSpecializationInfo = nullptr;
    ShaderStages[0].pNext = nullptr;

    //add Frag shader
    ShaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    ShaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    ShaderStages[1].module = FragShaderModule;
    ShaderStages[1].pName = "main";
    ShaderStages[1].flags = 0;
    ShaderStages[1].pSpecializationInfo = nullptr;

    // setup vertex shader input state
    VkPipelineVertexInputStateCreateInfo VertexInputInfo{};
    VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    VertexInputInfo.vertexAttributeDescriptionCount = 0;
    VertexInputInfo.vertexBindingDescriptionCount = 0;
    VertexInputInfo.pVertexBindingDescriptions = nullptr;
    VertexInputInfo.pVertexAttributeDescriptions = nullptr;

    VkPipelineViewportStateCreateInfo ViewportInfo{};
    ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    ViewportInfo.viewportCount = 1;
    ViewportInfo.pViewports = &PipelineConfig.Viewport;
    ViewportInfo.scissorCount = 1;
    ViewportInfo.pScissors = &PipelineConfig.Scissor;

    // setup pipeline
    VkGraphicsPipelineCreateInfo PipelineInfo{};
    PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    PipelineInfo.stageCount = 2;
    PipelineInfo.pStages = ShaderStages;
    PipelineInfo.pVertexInputState = &VertexInputInfo;
    PipelineInfo.pInputAssemblyState = &PipelineConfig.InputAssemblyInfo;
    PipelineInfo.pViewportState = &ViewportInfo;
    PipelineInfo.pRasterizationState = &PipelineConfig.RasterizationInfo;
    PipelineInfo.pMultisampleState = &PipelineConfig.MultisampleInfo;
    PipelineInfo.pColorBlendState = &PipelineConfig.ColorBlendInfo;
    PipelineInfo.pDepthStencilState = &PipelineConfig.DepthStencilInfo;
    PipelineInfo.pDynamicState = nullptr; // for future use

    PipelineInfo.layout = PipelineConfig.PipelineLayout;
    PipelineInfo.renderPass = PipelineConfig.RenderPass;
    PipelineInfo.subpass = PipelineConfig.Subpass;

    // can be used ti optimize performance
    PipelineInfo.basePipelineIndex = -1;
    PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    
    
    const VkResult Result = vkCreateGraphicsPipelines(ModernDevice.device(), VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &GraphicsPipeline);
    
    if (Result != VK_SUCCESS)
    {
        throw std::runtime_error ("Failed to create pipeline");
    }
    
}


void MEngine::GModernPipeline::CreateShaderModel(const std::vector<char>& ShaderCode, VkShaderModule* ShaderModule)
{
    //Imitialize a structure for shader creation
    VkShaderModuleCreateInfo CreateInfo{};
    CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    CreateInfo.codeSize = ShaderCode.size();
    CreateInfo.pCode = reinterpret_cast<const uint32_t*>(ShaderCode.data());

    const VkResult Result = vkCreateShaderModule(ModernDevice.device(), &CreateInfo,  nullptr, ShaderModule);

    if (Result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module");
    }

    
    
}
