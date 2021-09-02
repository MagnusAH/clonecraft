#include "stb.hpp"

#include <GL/gl3w.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "graphics/renderer.hpp"
#include "window/input.hpp"
#include "game/player.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <chrono>

#include "game/game.hpp"
#include "game/client/mesher.hpp"
#include "game/server/server.hpp"
#include "game/client/client.hpp"

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

// TEXTURE TEST

	{
		int width, height, channels;
		unsigned char* im_dirt = stbi_load("textures/dirt.png", &width, &height, &channels, 0);
		unsigned char* im_grass_dirt = stbi_load("textures/grass_dirt.png", &width, &height, &channels, 0);
		unsigned char* im_grass = stbi_load("textures/grass.png", &width, &height, &channels, 0);

		uint64_t image_size = width * height * channels;

		unsigned char* atlas = (unsigned char*) malloc(width * height * 4 * channels);
		memcpy(atlas, im_dirt, image_size);
		memcpy(atlas + image_size, im_grass_dirt, image_size);
		memcpy(atlas + (image_size * 2), im_grass, image_size);

		stbi_image_free(im_dirt);
		stbi_image_free(im_grass_dirt);
		stbi_image_free(im_grass);

		GLuint atlasID;
		glGenTextures(1, &atlasID);
		glBindTexture(GL_TEXTURE_2D, atlasID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5); // 2^5 = 32 (tile size)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height * 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas);
		glGenerateMipmap(GL_TEXTURE_2D);


	}

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
