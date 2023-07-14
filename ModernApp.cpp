#include "ModernApp.h"
#include <array>

MEngine::GModernApp::GModernApp()
{
	CreatePipelineLayout();
	CreatePipeline();
	CreateCommandBuffers();
	
	
}

MEngine::GModernApp::~GModernApp()
{
	vkDestroyPipelineLayout(ModernDevice.device(), PipelineLayout, nullptr);
}

void MEngine::GModernApp::Run()
{

	// Engine loop
	while (!ModernWindow.ShouldClose())
	{
		// Handle GLFW default events
		glfwPollEvents();
		DrawFrame();
	}

	vkDeviceWaitIdle(ModernDevice.device());
	
}

void MEngine::GModernApp::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo PipelineLayoutInfo{};

	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 0;
	PipelineLayoutInfo.pSetLayouts = nullptr;
	PipelineLayoutInfo.pushConstantRangeCount = 0;
	PipelineLayoutInfo.pPushConstantRanges = nullptr;

	const VkResult Result = vkCreatePipelineLayout(ModernDevice.device(), &PipelineLayoutInfo, nullptr, &PipelineLayout);

	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error ("Failed to create Pipeline Layout");
	}
}

void MEngine::GModernApp::CreatePipeline()
{
	
	auto PipelineConfig = GModernPipeline::DefaultPipelineConfigInfo(ModernSwapChain.width(), ModernSwapChain.height());

	
	// Get render pass from swap chain
	PipelineConfig.RenderPass = ModernSwapChain.getRenderPass();
	PipelineConfig.PipelineLayout = PipelineLayout;

	// Initialize a pipeline
	ModernPipeline = std::make_unique<GModernPipeline>(ModernDevice, "Shader/SimpleShader.vert.spv", "Shader/SimpleShader.frag.spv", PipelineConfig);

	
}

void MEngine::GModernApp::CreateCommandBuffers()
{
	CommandBuffer.resize(ModernSwapChain.imageCount());

	VkCommandBufferAllocateInfo AllocInfo {};

	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandPool = ModernDevice.getCommandPool();
	AllocInfo.commandBufferCount = static_cast<uint32_t>(CommandBuffer.size());

	const VkResult Result = vkAllocateCommandBuffers(ModernDevice.device(), &AllocInfo, CommandBuffer.data());

	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffer");
	}

	for (int i = 0; i < CommandBuffer.size(); ++i)
	{
		VkCommandBufferBeginInfo BeginInfo {};
		BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(CommandBuffer[i], &BeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer");
		}

		// fill render pass info
		VkRenderPassBeginInfo RenderPassBeginInfo {};
		RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBeginInfo.renderPass = ModernSwapChain.getRenderPass();
		RenderPassBeginInfo.framebuffer = ModernSwapChain.getFrameBuffer(i);

		RenderPassBeginInfo.renderArea.offset = {0, 0};
		RenderPassBeginInfo.renderArea.extent = ModernSwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> ClearValues{};
		ClearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
		ClearValues[1].depthStencil = {1.0f, 0};
		RenderPassBeginInfo.clearValueCount = static_cast<uint32_t>(ClearValues.size());
		RenderPassBeginInfo.pClearValues = ClearValues.data();

		vkCmdBeginRenderPass(CommandBuffer[i], &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		ModernPipeline->BindPipeline(CommandBuffer[i]);
		vkCmdDraw(CommandBuffer[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(CommandBuffer[i]);
		if (vkEndCommandBuffer(CommandBuffer[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer");
		}
		
	}
}

void MEngine::GModernApp::DrawFrame()
{
	uint32_t ImageIndex;

	auto Result = ModernSwapChain.acquireNextImage(&ImageIndex);


	if (Result != VK_SUCCESS && Result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to acquire swap chain");
	}

	Result = ModernSwapChain.submitCommandBuffers(&CommandBuffer[ImageIndex], &ImageIndex);

	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit command buffers");
	}
}

