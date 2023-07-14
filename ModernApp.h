#pragma once

#include "ModernWindow.h"
#include "Renderer/ModernPipeline.h"
#include "SwapChain/ModernSwapChain.h"


namespace MEngine
{
	class GModernApp
	{
	public:

		static constexpr int WIDTH = 1600;
		static constexpr int HEIGHT = 900;

		GModernApp();
		~GModernApp();

		GModernApp(const GModernApp&) = delete;
		GModernApp &operator=(const GModernApp&) = delete;
		

		void Run();



	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();
		
		GModernWindow ModernWindow{WIDTH, HEIGHT, "Modern Engine"};
		GModernDevice ModernDevice {ModernWindow};
		GModernSwapChain ModernSwapChain {ModernDevice, ModernWindow.GetExtend()};
		std::unique_ptr<GModernPipeline> ModernPipeline;
		VkPipelineLayout PipelineLayout;
		std::vector<VkCommandBuffer> CommandBuffer;
	};
}

