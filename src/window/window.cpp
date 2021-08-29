#include "window.hpp"

Window::Window(int versionMajor, int versionMinor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_window = glfwCreateWindow(1, 1, "", NULL, NULL);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::update()
{
	m_wresized = false;
	m_fresized = false;
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	if (width != m_wwidth || height != m_wheight) {
		m_wresized = true;
		m_wwidth = width;
		m_wheight = height;
	}
	glfwGetFramebufferSize(m_window, &width, &height);
	if (width != m_fwidth || height != m_fheight) {
		m_fresized = true;
		m_fwidth = width;
		m_fheight = height;
	}
	glfwSwapBuffers(m_window);
}

void Window::setSize(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

void Window::setTitle(const char* title)
{
	glfwSetWindowTitle(m_window, title);
}