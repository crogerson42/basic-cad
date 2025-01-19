#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// standard libraries
#include <vector>

// additional libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>

// project libraries
#include "g_plane.hpp"
#include "g_sketch.hpp"
#include "g_extrude.hpp"
#include "Camera.hpp"
#include "ui_gui.hpp"

struct Global {
	// Shader: unique ID for graphics pipeline program object
	GLuint gGraphicsPipelineShaderProgram = 0;

	std::string gFilename;

	// Screen dimensions
	int gScreenWidth = 960; // 1600; // 640;
	int gScreenHeight = 720; // 900; // 480;
	SDL_Window* gGraphicsApplicationWindow = nullptr;
	SDL_GLContext gOpenGLContext = nullptr;
	
	bool gQuit = false;		// Main loop termination condition
	bool logging = true;	// Enable log file

	GLenum gPolygonMode = GL_FILL;	// Draw wireframe mode
	bool gCursorVisible = false;
	
	int planeDrawMode = 1;
	/*
	* 0 = none
	* 1 = coordinate planes
	* 2 = all
	*/

	float gZoom = 1.0f;

	// Camera
	Camera gCamera;

	// UI
	gui gui;

	// Objects
	// TO DO

	// Coordinate Planes
	Plane plane_xy;
	Plane plane_xz;
	Plane plane_yz;

	// All Planes
	std::vector<Plane> gPlanes;

	std::vector<Sketch> gSketches;

	std::vector<Extrude> gExtrudes;

	void AddPlane(Plane& newPlane);
	void AddSketch(Sketch& newSketch);
	void AddExtrude(Extrude& newExtrude);

	static void CreateNewExtrusion();

	int gCurrent = -1;
	static void CycleSelection();
	static void ChangeSelection(int i);

	int editMode = 0;
	static void CycleEditMode();

	static void LoadGeometry();
	static void SaveGeometry();

	static void TogglePolygons();
	static void TogglePlanes();

	static void DoNothing();
};

// external linkage to make globally available
extern Global g;

#endif