// standard libraries
#include <string>

// additional libraries
#include <glm/glm.hpp>

// project libraries
#include "ui_background.hpp"
#include "util.hpp"

Background::Background() : x(0), y(0), width(0), height(0), label("") {
    this->mVAO = 0;
    this->mVBO = 0;
}

Background::Background(float x, float y, float width, float height, std::string label)
    : x(x), y(y), width(width), height(height), label(label) {
    this->mVAO = 0;
    this->mVBO = 0;
}

Background::~Background() {
    if (mVBO != 0) {
        glDeleteBuffers(1, &mVBO);
    }
    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
    }
}

void Background::InitializeGL() {
    // Define vertices for quad
    GLfloat vertices[] = {
        // Positions           // Textures
        x,         y,          0.0f, 0.0f, // Bottom-left corner
        x + width, y,          0.0f, 0.0f, // Bottom-right corner
        x + width, y + height, 0.0f, 0.0f, // Top-right corner

        x,         y,          0.0f, 0.0f, // Bottom-left corner
        x + width, y + height, 0.0f, 0.0f, // Top-right corner
        x,         y + height, 0.0f, 0.0f, // Top-left corner
    };

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

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

void Background::PreDraw() {
    // Initialize graphics data only if necessary (not yet done, or changed)
    if (this->mVBO == 0) {
        InitializeGL();
    }

    // Bind shader program
    glUseProgram(g.gui.guiBackgroundShaderProgram);

    // Transformation matrices (model, projection)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 projection = g.gui.GetProjectionMatrix(); // orthogonal projection matrix for ui

    // Bind uniforms for transformation
    // Model Matrix
    GLuint u_ModelMatrixLocation = glGetUniformLocation(g.gui.guiBackgroundShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ModelMatrix; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // Projection Matrix
    GLuint u_ProjectionLocation = glGetUniformLocation(g.gui.guiBackgroundShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection; maybe a mispelling?\n";
        logfile("Could not find u_Projection; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }
}

void Background::Draw() {
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
}