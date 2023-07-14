#include "ModernWindow.h"

MEngine::GModernWindow::GModernWindow(int w, int h, const std::string& Name) : Width{ w }, Heigth{ h }, WindowName{ Name }
{
	InitWindow();
}

MEngine::GModernWindow::~GModernWindow()
{
	glfwDestroyWindow(Window);
	glfwTerminate();
}

bool MEngine::GModernWindow::ShouldClose()
{
	return glfwWindowShouldClose(Window);
}

void MEngine::GModernWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, Window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window surface instance");
	}
}

void MEngine::GModernWindow::InitWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	Window = glfwCreateWindow(Width, Heigth, WindowName.c_str(), nullptr, nullptr);
}




