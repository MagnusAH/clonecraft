#include "input.hpp"

#include <stdlib.h>
#include <stdio.h>

char* 			Input::m_keys;
int*			Input::m_kbuffer;
int				Input::m_kbsize;

unsigned int* 	Input::m_cbuffer;
int				Input::m_cbsize;

double			Input::m_x;
double			Input::m_y;
double			Input::m_dx;
double			Input::m_dy;

double			Input::m_xoff;
double			Input::m_yoff;

bool			Input::m_grabbed;

#define INPUT_KEYS (INPUT_KEY_COUNT + INPUT_BUTTON_COUNT)

void Input::init()
{
	m_keys = (char*)calloc(INPUT_KEYS, sizeof(char));
	m_kbuffer = (int*)calloc(INPUT_KEY_BUFFER_SIZE, sizeof(int));
	m_kbsize = 0;

	m_cbuffer = (unsigned int*)calloc(INPUT_CHAR_BUFFER_SIZE, sizeof(unsigned int));
	m_cbsize = 0;

	m_grabbed = false;
}

void Input::destroy()
{
	free(m_keys);
	free(m_cbuffer);
}

void Input::attachWindow(Window* window, int flags)
{
	if (flags & INPUT_HANDLE_KEYS) {
		glfwSetKeyCallback(window->getContext(), Input::key_callback);
	}
	if (flags & INPUT_HANDLE_CHARACTERS) {
		glfwSetCharCallback(window->getContext(), Input::character_callback);
	}
	if (flags & INPUT_HANDLE_MOUSE_POSITION) {
		glfwSetCursorPosCallback(window->getContext(), Input::cursor_position_callback);
	}
	if (flags & INPUT_HANDLE_MOUSE_BUTTONS) {
		glfwSetMouseButtonCallback(window->getContext(), Input::mouse_button_callback);
	}
	if (flags & INPUT_HANDLE_MOUSE_SCROLL) {
		glfwSetScrollCallback(window->getContext(), Input::scroll_callback);
	}
	if (flags & INPUT_HANDLE_MOUSE_ENTER) {
		glfwSetCursorEnterCallback(window->getContext(), Input::cursor_enter_callback);
	}
}

void Input::grabMouse(Window* window)
{
	glfwSetInputMode(window->getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(window->getContext(), &m_x, &m_y);
	m_dx = 0;
	m_dy = 0;
	m_grabbed = true;
}

void Input::releaseMouse(Window* window)
{
	glfwSetInputMode(window->getContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwGetCursorPos(window->getContext(), &m_x, &m_y);
	m_dx = 0;
	m_dy = 0;
	m_grabbed = false;
}

void Input::update()
{
	for (int i = 0; i < INPUT_KEYS; i++) {
		m_keys[i] = m_keys[i] & INPUT_DOWN;
	}
	m_kbsize = INPUT_KEY_BUFFER_SIZE;
	m_cbsize = INPUT_CHAR_BUFFER_SIZE;
	m_dx = 0;
	m_dy = 0;
	m_xoff = 0;
	m_yoff = 0;
	glfwPollEvents();
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action) {
		if (m_keys[key] & INPUT_DOWN) {
			m_keys[key] = INPUT_DOWN;
		} else {
			m_keys[key] = INPUT_PRESSED | INPUT_DOWN;
			m_kbuffer[--m_kbsize] = key;
		}
	} else {
		m_keys[key] = INPUT_RELEASED;
	}
}

void Input::character_callback(GLFWwindow* window, unsigned int codepoint)
{
	m_cbuffer[--m_cbsize] = codepoint;
}

void Input::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	m_dx = xpos - m_x;
	m_dy = ypos - m_y;
	m_x = xpos;
	m_y = ypos;

}

void Input::cursor_enter_callback(GLFWwindow* window, int entered)
{
	if (entered) {
		// printf("CURSOR ENTERED\n");
	} else {
		// printf("CURSOR LEFT\n");
	}
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	button += INPUT_KEY_COUNT;
	if (action) {
		if (m_keys[button] & INPUT_DOWN) {
			m_keys[button] = INPUT_DOWN;
		} else {
			m_keys[button] = INPUT_PRESSED | INPUT_DOWN;
			m_kbuffer[--m_kbsize] = button;
		}
	} else {
		m_keys[button] = INPUT_RELEASED;
	}
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_xoff = xoffset;
	m_yoff = yoffset;
}
