// standard libraries
#include <string>

// additional libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <SDL2/SDL.h>

// project libraries
#include "globals.hpp"

#ifndef UI_BACKGROUND_HPP
#define UI_BACKGROUND_HPP

struct Background {
	float x, y, width, height;
	std::string label;
	// std::function<void()> onClick;

	Background();
	Background(float x, float y, float width, float height, std::string label); // , std::function<void()> function);
	~Background();

	void InitializeGL();
	void PreDraw();
	void Draw();

	GLuint mVAO, mVBO;
};

#endif