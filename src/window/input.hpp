#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window.hpp"

#define INPUT_DOWN						0b00000010
#define INPUT_PRESSED					0b00000100
#define INPUT_RELEASED					0b00000001

#define INPUT_HANDLE_KEYS				0b00000001
#define INPUT_HANDLE_CHARACTERS			0b00000010
#define INPUT_HANDLE_MOUSE_POSITION		0b00000100
#define INPUT_HANDLE_MOUSE_ENTER		0b00001000
#define INPUT_HANDLE_MOUSE_BUTTONS		0b00010000
#define INPUT_HANDLE_MOUSE_SCROLL		0b00100000

#define INPUT_HANDLE_MOUSE 				(INPUT_HANDLE_MOUSE_BUTTONS | INPUT_HANDLE_MOUSE_ENTER | INPUT_HANDLE_MOUSE_POSITION | INPUT_HANDLE_MOUSE_SCROLL)
#define INPUT_HANDLE_ALL				(INPUT_HANDLE_KEYS | INPUT_HANDLE_CHARACTERS | INPUT_HANDLE_MOUSE)

#ifndef INPUT_KEY_COUNT
#define INPUT_KEY_COUNT GLFW_KEY_LAST
#endif

// can only be defeated by the super ultra mega gamer mouse
#ifndef INPUT_BUTTON_COUNT
#define INPUT_BUTTON_COUNT 32
#endif


// if these values aren't large enough, the program is probably freezing up
#ifndef INPUT_KEY_BUFFER_SIZE
#define INPUT_KEY_BUFFER_SIZE 32
#endif

#ifndef INPUT_CHAR_BUFFER_SIZE
#define INPUT_CHAR_BUFFER_SIZE 32
#endif

// should change this to a namespace but I'm too lazy
class Input
{
private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

public:
	static char* 			m_keys;
	static int*				m_kbuffer;
	static int				m_kbsize;

	static unsigned int*	m_cbuffer;
	static int 				m_cbsize;

	static double			m_x, m_y, m_dx, m_dy;
	static double 			m_xoff, m_yoff;

	static bool				m_grabbed;

	static void init();
	static void destroy();
	static void update();

	static void attachWindow(Window* window, int flags);

	static void grabMouse(Window* window);
	static void releaseMouse(Window* window);

	static inline int getKey() { return m_kbuffer[m_kbsize++]; }
	static inline bool hasKey() { return m_kbsize < INPUT_KEY_BUFFER_SIZE; }

	static inline int getChar() { return m_cbuffer[m_cbsize++]; }
	static inline bool hasChars() { return m_cbsize < INPUT_CHAR_BUFFER_SIZE; }

	static inline char key(int key) { return m_keys[key]; }
	static inline char keyDown(int key) { return m_keys[key] & INPUT_DOWN; }
	static inline char keyPressed(int key) { return m_keys[key] & INPUT_PRESSED; }
	static inline char keyReleased(int key) { return m_keys[key] & INPUT_RELEASED; }

	static inline char button(int button) { return m_keys[button + INPUT_KEY_COUNT]; }
	static inline char buttonDown(int button) { return m_keys[button + INPUT_KEY_COUNT] & INPUT_DOWN; }
	static inline char buttonPressed(int button) { return m_keys[button + INPUT_KEY_COUNT] & INPUT_PRESSED; }
	static inline char buttonReleased(int button) { return m_keys[button + INPUT_KEY_COUNT] & INPUT_RELEASED; }

	static inline bool isKey(int key) { return key < INPUT_KEY_COUNT; }
	static inline bool isButton(int button) { return button >= INPUT_KEY_COUNT; }

	static inline bool mouseGrabbed() { return m_grabbed; }

	static inline double scrollH() { return m_xoff; }
	static inline double scrollV() { return m_yoff; }
};