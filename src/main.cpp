// standard libraries
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// additional libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project libraries
// geometry
#include "g_plane.hpp"
#include "g_sketch.hpp"
#include "g_extrude.hpp"

// functionality
#include "globals.hpp"
#include "file_io.hpp"
#include "ui_gui.hpp"
#include "util.hpp"

// testing, etc.
#include "tests.hpp"

// Globals
GLuint gVertexArrayObjectFloor = 0;
GLuint gVertexBufferObjectFloor = 0;

std::string inputText = "";

/*
* Create graphics pipeline
*/
void CreateGraphicsPipeline() {
    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");
    g.gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    Plane::CreateGraphicsPipelinePlane();
    gui::CreateGraphicsPipelineGUI();
}

/*
* Initialize graphics application: setup window and OpenGL Context
*/
void InitializeProgram(std::string filename) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        logfile("SDL could not initialize!");
        exit(1);
    }

    // Setup OpenGL Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // double buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create application window using OpenGL that supports SDL
    g.gGraphicsApplicationWindow = SDL_CreateWindow(
        "BasiCAD",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        g.gScreenWidth,
        g.gScreenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    // Verify window creation
    if (g.gGraphicsApplicationWindow == nullptr) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
        logfile("Window could not be created!");
        exit(1);
    }

    // Create OpenGL context
    g.gOpenGLContext = SDL_GL_CreateContext(g.gGraphicsApplicationWindow);
    if (g.gOpenGLContext == nullptr) {
        std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
        logfile("OpenGL context could not be created!");
        exit(1);
    }

    // Initialize GLAD library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "GLAD did not initialize!" << std::endl;
        logfile("GLAD did not initializze!");
        exit(1);
    }

    SDL_ShowCursor(SDL_DISABLE);

    g.gFilename = filename;
    g.gui = gui();

    // Initialize objects
    // TO DO
    g.plane_xy = Plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), "plane_xy");
    g.plane_xz = Plane(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), "plane_xz");
    g.plane_yz = Plane(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), "plane_yz");

    g.gPlanes.push_back(g.plane_xy);
    g.gPlanes.push_back(g.plane_xz);
    g.gPlanes.push_back(g.plane_yz);

    //g.plane_xy.show(false);
    //g.plane_xz.show(false);
    //g.plane_yz.show(false);

    g.gPlanes[0].show(true);
    g.gPlanes[1].show(true);
    g.gPlanes[2].show(true);

    // MY DEMO PLANE
    //g.gPlanes.push_back(Plane(glm::vec3(0, 1, 0), glm::vec3(0, 1, 1), glm::vec3(1.0, 0.5, 0.0)));
    //g.gPlanes[3].show(false);

    // MY DEMO EXTRUDE
    //Sketch sketch = Sketch(g.plane_xz);
    //sketch.addVertex(1, 0, 1);
    //sketch.addVertex(1, 0, 2);
    //sketch.addVertex(1.5, 0, 1.6);
    ////sketch.addVertex(1.75, 0, 1.6);
    //sketch.addVertex(2, 0, 2);
    //sketch.addVertex(2, 0, 1);

    //Extrude extrude = Extrude(sketch, 0.1);

    //g.AddSketch(sketch);
    //g.AddExtrude(extrude);

    //LoadObjectFromFile(filename); // <<<<<<<<<<<<<<<<<<<<<<
    
    //g.gSketches.push_back(sketch);
    //g.gExtrudes.push_back(extrude);
}

void PreDraw() {
    // Disable depth test and face culling
    glEnable(GL_DEPTH_TEST); // need to enable
    glDisable(GL_CULL_FACE);

    // Set transparency
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set polygon fill mode
    glPolygonMode(GL_FRONT_AND_BACK, g.gPolygonMode);

    // Initialize background color
    glViewport(0, 0, g.gScreenWidth, g.gScreenHeight);
    glClearColor(0.1f, 4.0f, 7.0f, 1.0f);

    // Clear color buffer and depth buffer
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Draw() {
    // logfile("Mainloop Draw" + std::to_string(g.gPlanes.size()) + std::to_string(g.gSketches.size()) + std::to_string(g.gExtrudes.size()));

    // Planes
    for (int p = 0; p < g.gPlanes.size(); p++) {
        //logfile("Plane" + std::to_string(p));
        g.gPlanes[p].PreDraw();
        //logfile("Plane" + std::to_string(p));
        g.gPlanes[p].Draw();
    }

    // Geometry Features
    for (int f = 0; f < g.gExtrudes.size(); f++) {
        //logfile("Extrude" + std::to_string(f));
        g.gExtrudes[f].PreDraw();
        //logfile("Extrude" + std::to_string(f));
        g.gExtrudes[f].Draw();
    }

    // GUI
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    g.gui.Draw();
    glPolygonMode(GL_FRONT_AND_BACK, g.gPolygonMode);
}

void Input() {
    static int mouseX = g.gScreenWidth / 2;
    static int mouseY = g.gScreenHeight / 2;

    // SDL event handler for I/O events
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::cout << "Goodbye! Quitting application" << std::endl;
            logfile("Goodbye! Quitting application");
            g.gQuit = true;
        }
        if (e.type == SDL_TEXTINPUT) {
            inputText += e.text.text;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                std::cout << "ESC: Goodbye! Quitting application" << std::endl;
                logfile("ESC: Goodbye! Quitting application");
                g.gQuit = true;
            }
            else if (e.key.keysym.sym == SDLK_BACKSPACE && !inputText.empty()) {
                inputText.pop_back();
            }
            else if (e.key.keysym.sym == SDLK_RETURN) {
                logfile(inputText);
                inputText = "";
            }
        }
        if (e.type == SDL_MOUSEMOTION and g.gCursorVisible == false) {
            // Capture mouse position change
            mouseX += e.motion.xrel;
            mouseY += e.motion.yrel;
            g.gCamera.MouseLook(mouseX, mouseY);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                g.gui.ProcessMouseClick(e.button.x, e.button.y);
            }
            else if (e.button.button == SDL_BUTTON_RIGHT) {
                // right click
            }
        }
    }

    // Keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    // TO DO: CAMERA MOTION -> NEED TO REPLACE VIDEOGAME CONTROLS WITH CAD CONTROLS + CURSOR
    if (state[SDL_SCANCODE_W]) {
        g.gCamera.MoveForward(0.1f);
    }
    if (state[SDL_SCANCODE_S]) {
        g.gCamera.MoveBackward(0.1f);
    }
    if (state[SDL_SCANCODE_A]) {
        g.gCamera.MoveLeft(0.1f);
    }
    if (state[SDL_SCANCODE_D]) {
        g.gCamera.MoveRight(0.1f);
    }
    if (state[SDL_SCANCODE_Q]) {
        g.gCamera.MoveUp(0.1f);
    }
    if (state[SDL_SCANCODE_E]) {
        g.gCamera.MoveDown(0.1f);
    }
    
    if (g.editMode == 0) { // modify extrusion length-wise
        // change extrude length
        if (state[SDL_SCANCODE_UP]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].length += 1;
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
        if (state[SDL_SCANCODE_DOWN]) {
            if (g.gCurrent != -1 && g.gExtrudes[g.gCurrent].length >= 1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].length -= 1;
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }

        // change extrude base offset (not preserved on save)
        if (state[SDL_SCANCODE_LEFT]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(1);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            if (g.gCurrent != -1 && g.gExtrudes[g.gCurrent].length >= 1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(0);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
    }
    else { // move extrusion laterally within sketch plane
        if (state[SDL_SCANCODE_UP]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(2);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
        if (state[SDL_SCANCODE_DOWN]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(3);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
        if (state[SDL_SCANCODE_LEFT]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(4);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            if (g.gCurrent != -1) {
                SDL_Delay(100);
                g.gExtrudes[g.gCurrent].ModifyBase(5);
                g.gExtrudes[g.gCurrent].MakeMesh();
            }
        }
    }

    if (state[SDL_SCANCODE_TAB]) {
        SDL_Delay(100);
        // g.TogglePolygons(); // moved to UI button
        g.CycleSelection();
    }

    if (state[SDL_SCANCODE_GRAVE]) {
        SDL_Delay(100);
        g.CycleEditMode();
    }

    if (state[SDL_SCANCODE_N]) { // NEW (Extrusion)
        SDL_Delay(100);
        g.CreateNewExtrusion();
    }

    // SPACEBAR toggles cursor mode vs. look mode
    if (state[SDL_SCANCODE_SPACE]) {
        SDL_Delay(250);
        if (g.gCursorVisible == true) {
            g.gCursorVisible = false;
            SDL_ShowCursor(SDL_DISABLE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        else {
            g.gCursorVisible = true;
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_WarpMouseInWindow(g.gGraphicsApplicationWindow, g.gScreenWidth / 2, g.gScreenHeight / 2);
        }
    }

    // TO DO: CONTROLS
}

void MainLoop() {
    // Re-center mouse on screen
    SDL_WarpMouseInWindow(g.gGraphicsApplicationWindow, g.gScreenWidth / 2, g.gScreenHeight / 2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Run application
    while (!g.gQuit) {
        // Handle input
        Input();

        // Setup before draw calls
        PreDraw();

        // OpenGL draw calls
        Draw();

        // Update window
        SDL_GL_SwapWindow(g.gGraphicsApplicationWindow);
    }
}

void CleanUp() {
    // 'Manually' free buffers, since not freed by destructors to avoid OpenGL errors with copies
    // Planes
    for (int p = 0; p < g.gPlanes.size(); p++) {
        g.gPlanes[p].FreeBuffers();
    }
    // Geometry Features
    for (int f = 0; f < g.gExtrudes.size(); f++) {
        g.gExtrudes[f].FreeBuffers();
    }

    // Destroy SDL2 window
    SDL_DestroyWindow(g.gGraphicsApplicationWindow);
    g.gGraphicsApplicationWindow = nullptr;

    // Delete OpenGL objects
    glDeleteBuffers(1, &gVertexBufferObjectFloor);
    glDeleteVertexArrays(1, &gVertexArrayObjectFloor);

    // Delete graphics pipeline
    glDeleteProgram(g.gGraphicsPipelineShaderProgram);

    // Quit SDL
    SDL_Quit();
}

int main(int argc, char** argv) {
    // testSimpleExtrude();
    // testAngleExtrude();

    std::string file = "";

    logfile("Starting application", false);

    if (argc < 2) {
        std::cout << "No file specified" << std::endl;
        logfile("<No file specified>");
    }
    else {
        std::string temp(argv[1]);
        file = temp;
    }

    std::string modelFilePath(file);

    // Setup program
    InitializeProgram(modelFilePath);

    // Create graphics pipeline
    CreateGraphicsPipeline();

    // Main application loop
    MainLoop();

    // Cleanup upon termination
    CleanUp();

    return 0;
}