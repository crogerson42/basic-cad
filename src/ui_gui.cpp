// standard libraries
#include <iostream>
#include <filesystem>
#include <functional>

// project libraries
#include "ui_gui.hpp" // IMPLEMENTS
#include "ui_button.hpp"

GLuint gui::guiButtonShaderProgram = 0;
GLuint gui::guiBackgroundShaderProgram = 0;

glm::vec3 ui_color_dark(0.0, 0.278, 0.294);

gui::gui() {
    mTexture = new Texture();

    std::vector<std::string> button_names = {
        "b_save", "b_load", "b_wireframe", "b_plane", "b_extrude" // "b_export"
    };
    std::vector<int> sprite_pos = {
        0, 1, 4, 3, 5 // 2
    };
    std::vector<std::function<void()>> functions = {
        g.SaveGeometry, g.LoadGeometry, g.TogglePolygons, g.TogglePlanes, g.CreateNewExtrusion
    };

    float button_width = 68.0;
    float button_height = 68.0;
    float button_spacing = 10.0f;

    float button_offset = 8.0f;

    for (int i = 0; i < button_names.size(); i++) {
        float x = i * (button_width + button_spacing) + button_offset;
        float y = button_offset;
        buttons[button_names[i]] = Button(x, y, button_width, button_height, button_names[i], mTexture, sprite_pos[i], functions[i]);
    }

    bg_objects["top_menu"] = Background(0, 0, g.gScreenWidth, button_height + button_offset * 2, "top_menu");

    logfile("GUI initialized!");
}

gui::~gui() {
    // buttons delete themselves (and their buffers)
    // deleting mTexture explicitly causes crashes
}

glm::mat4 gui::GetProjectionMatrix() const {
	return this->projection;
}

void gui::ProcessMouseClick(int x, int y) {
    // for all buttons
    for (std::unordered_map<std::string, Button>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
        it->second.isClicked(x, y);
    }
}

void gui::CreateGraphicsPipelineGUI() {
    std::string vertexShaderSource = LoadShaderAsString("./shaders/ui_button_vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/ui_button_frag.glsl");
    gui::guiButtonShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    vertexShaderSource = LoadShaderAsString("./shaders/ui_bg_vert.glsl");
    fragmentShaderSource = LoadShaderAsString("./shaders/ui_bg_frag.glsl");
    gui::guiBackgroundShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void gui::InitializeGL() {
    // initialize orthogonal projection matrix
    gui::projection = glm::ortho(
        0.0f, static_cast<float>(g.gScreenWidth),
        static_cast<float>(g.gScreenHeight), 0.0f,
        -1.0f, 1.0f);

    // load texture used for UI elements
    std::string imageFilePath = "./assets/texture_ui.ppm";
    mTexture->LoadTexture(imageFilePath);

    this->made_projection = true;
}


//void gui::PreDraw() {
//    // Bind our texture to slot number 0
//    int ui_texture_slot = 0;
//
//    mTexture->Bind(ui_texture_slot);
//
//    // Setup our uniform for texture
//    GLint u_textureLocation = glGetUniformLocation(guiButtonShaderProgram, "u_Texture");
//    std::cout << u_textureLocation << std::endl;
//    if (u_textureLocation >= 0) {
//        // Setup the slot for the texture
//        glUniform1i(u_textureLocation, ui_texture_slot);
//    }
//    else {
//        std::cout << "Could not find u_Texture, maybe a misspelling?" << std::endl;
//        logfile("Could not find u_Texture, maybe a misspelling?");
//        exit(EXIT_FAILURE);
//    }
//}


void gui::Draw() {
    if (!this->made_projection) {
        InitializeGL();
    }

    // PreDraw();

    glDisable(GL_DEPTH_TEST);

    // draw all background objects
    for (std::unordered_map<std::string, Background>::iterator it = bg_objects.begin(); it != bg_objects.end(); ++it) {
        it->second.PreDraw();
        it->second.Draw();
    }

    // draw all buttons
    for (std::unordered_map<std::string, Button>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
        it->second.PreDraw();
        it->second.Draw();
    }

    glEnable(GL_DEPTH_TEST);
}
