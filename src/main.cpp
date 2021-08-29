#include "stb.hpp"

#include <GL/gl3w.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "util/io.hpp"

#include "graphics/shader.hpp"
#include "graphics/renderer.hpp"

#include "math/matrix4f.hpp"
#include "math/vector4f.hpp"

#include "math/frustum.hpp"

#include "window/input.hpp"

#include "game/mesh.hpp"
#include "game/player.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <chrono>

#include "game/game.hpp"
#include "game/client/mesher.hpp"
#include "game/server/server.hpp"
#include "game/client/client.hpp"

inline void printF4(float* f)
{
	printf("[%f, %f, %f, %f]\n", f[0], f[1], f[2], f[3]);
}

void printMatrix(Matrix4f m)
{
	printf("-=MATRIX=-\n");
	printF4(m.mat);
	printF4(&m.mat[4]);
	printF4(&m.mat[8]);
	printF4(&m.mat[12]);
}

#include <string>
#include <sstream>

int main(void)
{

    if (!glfwInit()) {
        printf("GLFW failed to initialize");
    }

	Window* window = new Window(4, 5);
	window->setSize(1280, 720);
	window->makeCurrent();

    glfwSwapInterval(1);

    if (gl3wInit()) {
		printf("OpenGL could not be initialized, is there a bound context?\n");
		return -1;
	}

	glClearColor(0.7f, 0.8f, 0.9f, 0.0f);
	glViewport(0, 0, 1280, 720);

	Input::init();
	Input::attachWindow(window, INPUT_HANDLE_ALL);

	Renderer::init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

// 	===============  //
// 	GAME STUFF HERE  //
// 	===============  //

	World::seed = 14.6096f;
	World::scale = 0.0027f;

	initGamePools();

	Server::init();
	Server::load_radius = 10;

	Client::init();
	Client::setViewDistance(4);

	Client::player->y = 8;
	Client::connect();

	int frames = 0;
	int fps = 0;
	auto lastTime = std::chrono::high_resolution_clock::now();

	window->show();
    while(!window->closeRequested())
	{
		if (window->frameResized()) {
			glViewport(0, 0, window->frameWidth(), window->frameHeight());
			Renderer::setProjection(window, 1.92f, 0.1f, 1024.0f);
		}

		// mouse grab
		if (Input::buttonPressed(GLFW_MOUSE_BUTTON_1)) {
			Input::grabMouse(window);
		} else if (Input::keyPressed(GLFW_KEY_ESCAPE)) {
			Input::releaseMouse(window);
		}

		if (Input::keyPressed(GLFW_KEY_1) && Client::getViewDistance() < Server::load_radius - 1) {
			Client::setViewDistance(Client::getViewDistance() + 1);
		} else if (Input::keyPressed(GLFW_KEY_2) && Client::getViewDistance() > 0) {
			Client::setViewDistance(Client::getViewDistance() - 1);
		}

		// player control
		if (Input::mouseGrabbed()) {
			Client::updatePlayer();
		}

		// sync client player with server player
		Client::sync();
		// update chunks, load unloaded chunks within range of players and unload loaded chunks outside of range of players
		Server::update();
		// update
		Client::update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Renderer::drawClientChunks();

        window->update();
		Input::update();
		frames++;

		if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-lastTime)).count() >= 1000) {
			fps = frames;
			frames = 0;
			lastTime = std::chrono::high_resolution_clock::now();
		}

		std::ostringstream ss;
		ss << " FPS = " << fps << " | x = " << (round(Client::player->x)) << ", y = " << (round(Client::player->y)) << ", z = " << (round(Client::player->z));
		window->setTitle(ss.str().c_str());

    }

	Input::destroy();
	delete window;

    glfwTerminate();

    return 0;
}

	// auto start = std::chrono::high_resolution_clock::now();
	// auto end = std::chrono::high_resolution_clock::now();
	// duration = (std::chrono::duration_cast<std::chrono::microseconds>(end-start)).count();

	// GLint maxTex, maxFTex;
	// glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTex);
	// glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxFTex);

	// printf("Max combined texture image units = %i\n", maxTex);
	// printf("Max fragment texture image units = %i\n", maxFTex);
