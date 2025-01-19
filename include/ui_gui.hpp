// standard libraries
#include <string>
#include <vector>
#include <unordered_map>

// additional libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <SDL2/SDL.h>

// project libraries
#include "globals.hpp"
#include "util.hpp"
#include "Texture.hpp"

// gui libraries
#include "ui_button.hpp"
#include "ui_background.hpp"

#ifndef UI_GUI_HPP
#define UI_GUI_HPP

class gui {
public:
	gui();
	~gui();

	glm::mat4 GetProjectionMatrix() const;

	void ProcessMouseClick(int x, int y);

	std::unordered_map<std::string, Button> buttons;
	std::unordered_map<std::string, Background> bg_objects;

	// Graphics functions
	static GLuint guiButtonShaderProgram;
	static GLuint guiBackgroundShaderProgram;

	Texture* mTexture;

	static void CreateGraphicsPipelineGUI();

	//void PreDraw();
	void Draw();

private:
	// Graphics functions
	void InitializeGL();

	bool made_projection = false;
	glm::mat4 projection;
};

#endif