// standard libraries
#include <string>

// additional libraries
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <SDL2/SDL.h>

#ifndef G_PLANE_HPP
#define G_PLANE_HPP

class Plane {
public:
	Plane();
	Plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color = glm::vec3(0,0,0), std::string name = "");
	Plane(float x, float y, float z, float nx, float ny, float nz, std::string name = "");
	Plane(const Plane& ref_plane, float offset, std::string name = "");
	~Plane();
	void FreeBuffers();

	std::string GetSaveDataString();

	glm::vec3 getPoint() const;
	glm::vec3 getNormal() const;

	std::string getName() const;
	std::string name;

	bool isVisible();
	void show(bool visible = true);

	static glm::vec3 transformPointBetweenPlanes(const glm::vec3& point, const Plane& src, const Plane& dest);

	// Graphics functions
	static GLuint planeShaderProgram;
	static void CreateGraphicsPipelinePlane();

	void PreDraw();
	void Draw();

private:
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 upVec;
	glm::vec3 color;
	bool visible;

	// Graphics functions
	void InitializeGL();

	GLuint mVAO, mVBO, mEBO;
};

#endif