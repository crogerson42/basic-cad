// standard libaries
#include <algorithm>
#include <cmath>
#include <sstream>

// project libaries
#include "g_sketch.hpp" // IMPLEMENTS
#include "globals.hpp"

// initialize static drawing plane (xy)
Plane Sketch::DrawingPlane = Plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

Sketch::Sketch(Plane& plane, std::string name)
	: plane(plane), name(std::move(name)) {
	// No assignment in the body; all initialization is done in the initializer list
}

std::string Sketch::GetSaveDataString() {
	std::stringstream data;
	data << "sketch " << name <<
		"\np " << plane.getName() << "\n";
	
	Plane reference;
	bool x_on = true;
	bool y_on = true;
	bool z_on = true;
	int winding_direction;

	if (plane.getNormal().x == 1) { // use yz plane
		reference = g.plane_yz;
		x_on = false;
		winding_direction = 1;
	}
	else if (plane.getNormal().x == -1) {
		reference = Plane(0, 0, 0, -1, 0, 0);
		x_on = false;
		winding_direction = -1;
	}
	if (plane.getNormal().y == 1) { // use xz plane
		reference = g.plane_xz;
		y_on = false;
		winding_direction = 1;
	}
	else if (plane.getNormal().y == -1) {
		reference = Plane(0, 0, 0, 0, -1, 0);
		y_on = false;
		winding_direction = -1;
	}
	if (plane.getNormal().z == 1) { // use xy plane
		reference = g.plane_xy;
		z_on = false;
		winding_direction = 1;
	}
	else if (plane.getNormal().z == -1) {
		reference = Plane(0, 0, 0, 0, 0, -1);
		z_on = false;
		winding_direction = -1;
	}
	else {
		reference = DrawingPlane; // NOT parallel to one of 3 ref planes -> not currently supported
	}
	
	if (winding_direction == -1) { // count up
		// count up
		for (int i = 0; i < vertices.size(); i++) {
			glm::vec3 out_point = Plane::transformPointBetweenPlanes(vertices[i], reference, DrawingPlane);

			if (abs(out_point.x) < std::pow(10, -7)) {
				out_point.x = 0;
			}
			if (abs(out_point.y) < std::pow(10, -7)) {
				out_point.y = 0;
			}
			if (abs(out_point.z) < std::pow(10, -7)) {
				out_point.z = 0;
			}

			// output relevant (two) planar dimensions
			data << "v ";
			if (x_on) {
				data << out_point.x << " ";
			}
			if (y_on) {
				data << out_point.y << " ";
			}
			if (z_on) {
				data << out_point.z << " ";
			}
			data << "\n";
		}
	}
	else { // count down
		for (int i = vertices.size() - 1; i >= 0; --i) {
			glm::vec3 out_point = Plane::transformPointBetweenPlanes(vertices[i], reference, DrawingPlane);

			if (abs(out_point.x) < std::pow(10, -7)) {
				out_point.x = 0;
			}
			if (abs(out_point.y) < std::pow(10, -7)) {
				out_point.y = 0;
			}
			if (abs(out_point.z) < std::pow(10, -7)) {
				out_point.z = 0;
			}

			// output relevant (two) planar dimensions
			data << "v ";
			if (x_on) {
				data << out_point.x << " ";
			}
			if (y_on) {
				data << out_point.y << " ";
			}
			if (z_on) {
				data << out_point.z << " ";
			}
			data << "\n";
		}
	}

	data << "/sketch\n";
	return data.str();
}

Plane& Sketch::getPlane() {
	return this->plane;
}

std::vector<glm::vec3>& Sketch::getVertices() {
	return this->vertices;
}

void Sketch::addVertex(glm::vec3 vertex) {
	vertices.push_back(vertex);
}

void Sketch::addVertex(float x, float y, float z) {
	vertices.push_back(glm::vec3(x, y, z));
}

void Sketch::addVertex(glm::vec2 vertex) {
	vertices.push_back(Plane::transformPointBetweenPlanes(glm::vec3(vertex, 0.0), DrawingPlane, this->plane));
}

void Sketch::addVertex(float x, float y) {
	vertices.push_back(Plane::transformPointBetweenPlanes(glm::vec3(x, y, 0.0), DrawingPlane, this->plane));
}

bool Sketch::removeVertex(glm::vec3 vertex) {
	auto target = std::find(vertices.begin(), vertices.end(), vertex);
	if (target != vertices.end()) {
		vertices.erase(target);
		return true;
	}
	return false;
}

bool Sketch::removeVertex(float x, float y, float z) {
	return removeVertex(glm::vec3(x, y, z));
}

bool Sketch::removeVertex(int index) {
	if (index >= 0 && index < vertices.size()) {
		vertices.erase(vertices.begin() + index);
		return true;
	}
	return false;
}

bool Sketch::isVisible() {
	return visible;
}

void Sketch::show(bool visible) {
	this->visible = visible;
}

std::string Sketch::getName() {
	return this->name;
}