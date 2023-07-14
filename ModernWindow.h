#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "iostream"

namespace MEngine
{
	class GModernWindow
	{
	public:
		GModernWindow(int w, int h, const std::string& Name);
		

		GModernWindow(const GModernWindow&) = delete;
		GModernWindow& operator=(const GModernWindow&) = delete;
		~GModernWindow();
		bool ShouldClose();

		VkExtent2D GetExtend() {return {static_cast<uint32_t>(Width), static_cast<uint32_t>(Heigth)};}

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	private:

		void InitWindow();

		int Width;
		int Heigth;

		std::string WindowName;

		GLFWwindow* Window;
	};
}


