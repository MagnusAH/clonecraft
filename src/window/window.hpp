#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* m_window;

	int m_wwidth, m_wheight;
	bool m_wresized;

	int m_fwidth, m_fheight;
	bool m_fresized;

public:
	Window(int versionMajor, int versionMinor);
	~Window();

	void update();
	void setSize(int width, int height);
	void setTitle(const char* title);
	inline void show() { glfwShowWindow(m_window); }
	inline void hide() { glfwHideWindow(m_window); }
	inline void makeCurrent() { glfwMakeContextCurrent(m_window); }

	inline int windowWidth() { return m_wwidth; }
	inline int windowHeight() { return m_wheight; }
	inline bool windowResized() { return m_wresized; }

	inline int frameWidth() { return m_fwidth; }
	inline int frameHeight() { return m_fheight; }
	inline bool frameResized() { return m_fresized; }

	inline bool closeRequested() { return glfwWindowShouldClose(m_window); }

	inline GLFWwindow* getContext() { return m_window; }

};