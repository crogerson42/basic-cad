// standard libraries
#include <functional>
#include <string>

// additional libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <SDL2/SDL.h>

// project libraries
#include "globals.hpp"
#include "Texture.hpp"

#ifndef UI_BUTTON_HPP
#define UI_BUTTON_HPP

struct Button {
	float x, y, width, height;
	int sprite_id;
	std::string label;
	std::function<void()> onClick;

	static void doNothing();

	Button();
	Button(float x, float y, float width, float height, std::string label, Texture* texture, int sprite_id, std::function<void()> function = doNothing);
	~Button();
	bool isClicked(int mouseX, int mouseY);
	int wasClicked;

	void InitializeGL();
	void PreDraw();
	void Draw();

	GLuint mVAO, mVBO;

	Texture* mTexture;
};

#endif