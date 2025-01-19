#include <string>
#include <vector>

#include <glm/glm.hpp>

struct GeoData_Plane {
	// std::string name;
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 color;

	GeoData_Plane()
		: point(0), normal(0), color(0) {
	}
};

struct GeoData_Sketch {
	// std::string name;
	std::string plane;
	std::vector<glm::vec2> vertices;

	GeoData_Sketch()
		: vertices() {
	}
};

struct GeoData_Extrude {
	// std::string name;
	std::string sketch;
	float length;

	GeoData_Extrude()
		: sketch(""), length(0.0f) {
	}
};