#pragma once
#include <string>
#include <vector>
#include "../Device/ModernDevice.h"

namespace MEngine
{
    struct FPipelineConfigInfo
    {
        VkViewport Viewport;
        VkRect2D Scissor;
        VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo RasterizationInfo;
        VkPipelineMultisampleStateCreateInfo MultisampleInfo;
        VkPipelineColorBlendAttachmentState ColorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
        VkPipelineLayout PipelineLayout = nullptr;
        VkRenderPass RenderPass = nullptr;
        uint32_t Subpass = 0;
    };
    class GModernPipeline
    {
    public:

        GModernPipeline(GModernDevice& Device, const std::string& VertShaderFilePath, const std::string& FragShaderFilePath, const FPipelineConfigInfo& PipelineConfig);

        void BindPipeline(VkCommandBuffer CommandBuffer);

        ~GModernPipeline();

        GModernPipeline(const GModernPipeline&) = delete;
        void operator=(const GModernPipeline&) = delete;

        static FPipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> ReadFile(const std::string& FilePath);

        void CreateGraphicsPipeline(const std::string& VertShaderFilePath, const std::string& FragShaderFilePath, const FPipelineConfigInfo& PipelineConfig);

        void CreateShaderModel(const std::vector<char>& ShaderCode, VkShaderModule * ShaderModule);

        GModernDevice& ModernDevice;
        VkPipeline GraphicsPipeline;
        VkShaderModule VertShaderModule;
        VkShaderModule FragShaderModule;
        
    
    };

}
