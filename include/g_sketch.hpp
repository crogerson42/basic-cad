// standard libraries
#include <string>
#include <vector>

// additional libraries
#include <glm/glm.hpp>

// project libraries
#include "g_plane.hpp"

#ifndef G_SKETCH_HPP
#define G_SKETCH_HPP

class Sketch {
public:
	Sketch(Plane& plane, std::string name = "");

	std::string GetSaveDataString();
	
	Plane& getPlane();
	std::vector<glm::vec3>& getVertices();

	void addVertex(glm::vec3 vertex);
	void addVertex(float x, float y, float z);

	void addVertex(glm::vec2 vertex);
	void addVertex(float x, float y);
	
	bool removeVertex(glm::vec3 vertex);
	bool removeVertex(float x, float y, float z);
	bool removeVertex(int index);

	bool isVisible();
	void show(bool visible = true);

	std::string getName();
	std::string name;

private:
	Plane& plane;
	std::vector<glm::vec3> vertices;
	bool visible;

	static Plane DrawingPlane;
};

#endif