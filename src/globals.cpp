#include "globals.hpp"
#include "file_io.hpp"
#include "util.hpp"

// Initialize global object
Global g;

void Global::AddPlane(Plane& newPlane) {
	if (newPlane.getName() == "") {
		newPlane.name = "plane_";
		newPlane.name.append(std::to_string(g.gPlanes.size()));
	}
	newPlane.show(false);
	g.gPlanes.push_back(newPlane);
}

void Global::AddSketch(Sketch& newSketch) {
	if (newSketch.getName() == "") {
		newSketch.name = "sketch_";
		newSketch.name.append(std::to_string(g.gSketches.size()));
	}
	g.gSketches.push_back(newSketch);
}

void Global::AddExtrude(Extrude& newExtrude) {
	if (newExtrude.getName() == "") {
		newExtrude.name = "extrude_";
		newExtrude.name.append(std::to_string(g.gExtrudes.size()));
	}
	g.gExtrudes.push_back(newExtrude);
}

void Global::CreateNewExtrusion() {
	// create new sketch
	Sketch* newSketch = new Sketch(g.plane_xz);
	newSketch->addVertex(2, 2);
	newSketch->addVertex(-2, 2);
	newSketch->addVertex(-2, -2);
	newSketch->addVertex(2, -2);
	g.AddSketch(*newSketch);

	// create new extrude
	Extrude* newExtrude = new Extrude(*newSketch, 4);
	g.AddExtrude(*newExtrude);

	// switch to new object as current selection
	g.ChangeSelection(g.gExtrudes.size() - 1);
}

void Global::LoadGeometry() {
	LoadObjectFromFile(g.gFilename);
}

void Global::SaveGeometry() {
	std::string out_file = g.gFilename;
	std::size_t extension = out_file.rfind('.');
	std::string filename_modifier = "_output";

	if (extension != std::string::npos && out_file[extension + 1] != '/') {
		out_file.insert(extension, filename_modifier);
	}
	else {
		out_file.append(filename_modifier);
	}

	logfile("#Saved geometry from " + g.gFilename + "\n", false, out_file);

	for (int p = 3; p < g.gPlanes.size(); p++) {
		// skip xy, xz, and yz planes (start at p=3)
		logfile(g.gPlanes[p].GetSaveDataString(), true, out_file);
	}
	
	for (int s = 0; s < g.gSketches.size(); s++) {
		logfile(g.gSketches[s].GetSaveDataString(), true, out_file);
	}

	for (int e = 0; e < g.gExtrudes.size(); e++) {
		logfile(g.gExtrudes[e].GetSaveDataString(), true, out_file);
	}
}

void Global::TogglePolygons() {
	if (g.gPolygonMode == GL_FILL) {
		g.gPolygonMode = GL_LINE;
	}
	else {
		g.gPolygonMode = GL_FILL;
	}
}

void Global::TogglePlanes() {
	g.planeDrawMode = (g.planeDrawMode + 1) % 3;

	for (int i = 0; i < g.gPlanes.size(); i++) {
		if (g.planeDrawMode == 1 && i < 3) { // draw only coordinate frame planes (mode = 1)
			g.gPlanes[i].show(true);
		}
		else if (g.planeDrawMode == 2) { // draw all planes (mode = 2)
			g.gPlanes[i].show(true);
		}
		else {
			g.gPlanes[i].show(false); // draw no planes (mode = 0)
		}
	}
}

void Global::CycleSelection() {
	// de-select
	if (g.gCurrent != -1) {
		g.gExtrudes[g.gCurrent].selected = false;
		g.gExtrudes[g.gCurrent].MakeMesh();
	}
	
	// increment selection
	g.gCurrent = (g.gCurrent + 1);
	if (g.gCurrent == g.gExtrudes.size()) {
		g.gCurrent = -1; // no selection
	}

	// select new
	if (g.gCurrent != -1) {
		g.gExtrudes[g.gCurrent].selected = true;
		g.gExtrudes[g.gCurrent].MakeMesh();
	}
}

void Global::ChangeSelection(int newSelection) {
	// de-select
	if (g.gCurrent != -1) {
		g.gExtrudes[g.gCurrent].selected = false;
		g.gExtrudes[g.gCurrent].MakeMesh();
	}

	// update selection if valid
	// CONSIDER ONLY DE-SELECTING IF THIS CONDITION IS MET, TOO
	// DEPENDING ON DESIRED BEHAVIOR FOR IMPLEMENTING FUTURE FEATURES
	if (newSelection >= -1 && newSelection < g.gExtrudes.size()) {
		g.gCurrent = newSelection;
	}
	else {
		g.gCurrent = -1;
	}

	// select new
	if (g.gCurrent != -1) {
		g.gExtrudes[g.gCurrent].selected = true;
		g.gExtrudes[g.gCurrent].MakeMesh();
	}
}

void Global::CycleEditMode() {
	g.editMode = 1 - g.editMode;
}

void Global::DoNothing() {
	// used as dummy function (primarily for initializing buttons to do nothing)
	logfile("Technically, printing this is something");
}