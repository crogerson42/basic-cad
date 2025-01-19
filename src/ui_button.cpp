// standard libraries
#include <functional>
#include <string>

// additional libraries
#include <glm/glm.hpp>

// project libraries
#include "ui_button.hpp"
#include "util.hpp"

Button::Button() : x(0), y(0), width(0), height(0), label("") {
    this->mVAO = 0;
    this->mVBO = 0;
    this->mTexture = nullptr;
    this->sprite_id = 0;
    this->wasClicked = 0;
}

Button::Button(float x, float y, float width, float height, std::string label, Texture* texture, int sprite_id, std::function<void()> function)
    : x(x), y(y), width(width), height(height), label(label) { 
    this->mVAO = 0;
    this->mVBO = 0;
    this->mTexture = texture;
    this->sprite_id = sprite_id;
    this->wasClicked = 0;
    this->onClick = function;
}

Button::~Button() {
    if (mVBO != 0) {
        glDeleteBuffers(1, &mVBO);
    }
    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
    }
}

void Button::doNothing() {
    logfile("Technically this is doing something");
}

bool Button::isClicked(int mouseX, int mouseY) {
	bool clicked =
		mouseX >= this->x &&
		mouseX <= this->x + this->width &&
		mouseY >= this->y &&
		mouseY <= this->y + this->height;
    if (clicked) {
        logfile(this->label);
        this->wasClicked = 20;
        this->onClick();
    }
	return clicked;
}

void Button::InitializeGL() {
    GLfloat tex = 70.0 / 512.0;

    GLfloat click_offset = 0.0f;
    if (this->wasClicked > 0) {
        click_offset += tex;
    }

    GLfloat tx = sprite_id * tex;
    GLfloat ty = click_offset;
    GLfloat tw = tex;
    GLfloat th = tex;
    
    // Define vertices for quad
    GLfloat vertices[] = {
        // Positions           // Textures
        x,         y,          tx,      ty,      // Bottom-left corner
        x + width, y,          tx + tw, ty,      // Bottom-right corner
        x + width, y + height, tx + tw, ty + th, // Top-right corner

        x,         y,          tx,      ty,      // Bottom-left corner
        x + width, y + height, tx + tw, ty + th, // Top-right corner
        x,         y + height, tx,      ty + th   // Top-left corner
    };

    // Use existing buffers if only refreshing; create if not initialized
    if (mVAO == 0) {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
    }

    glBindVertexArray(mVAO);

    // Populate buffers
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

    // Texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

    // Unbind currently bound VAO
    glBindVertexArray(0);

    // Disable previously opened attributes - not sure if necessary
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);
}

void Button::PreDraw() {
    // Initialize graphics data only if necessary (not yet done, or changed)
    bool refresh = false;
    // Drawing for first time, or just clicked
    if (this->mVBO == 0 or this->wasClicked == 10) {
        refresh = true;
    }
    // Count down frames from time of click; re-draw in default state if countdown reaches 0
    if (this->wasClicked > 0) {
        this->wasClicked--;
        if (this->wasClicked == 0) {
            refresh = true;
        }
    }
    // Refresh graphics data
    if (refresh) {
        InitializeGL();
    }

    // Bind shader program
    glUseProgram(g.gui.guiButtonShaderProgram);

    // Transformation matrices (model, projection)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection = g.gui.GetProjectionMatrix(); // orthogonal projection matrix for ui

    // Bind uniforms for transformation
    // Model Matrix
    GLuint u_ModelMatrixLocation = glGetUniformLocation(g.gui.guiButtonShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ModelMatrix; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // Projection Matrix
    GLuint u_ProjectionLocation = glGetUniformLocation(g.gui.guiButtonShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection; maybe a mispelling?\n";
        logfile("Could not find u_Projection; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // Texture
    int ui_texture_slot = 0;
    g.gui.mTexture->Bind(ui_texture_slot);

    // Setup our uniform for texture
    GLint u_textureLocation = glGetUniformLocation(g.gui.guiButtonShaderProgram, "u_Texture");
    std::cout << u_textureLocation << std::endl;
    if (u_textureLocation >= 0) {
        // Setup the slot for the texture
        glUniform1i(u_textureLocation, ui_texture_slot);
    }
    else {
        std::cout << "Could not find u_Texture, maybe a misspelling?" << std::endl;
        logfile("Could not find u_Texture, maybe a misspelling?");
        exit(EXIT_FAILURE);
    }
}

void Button::Draw() {
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
}