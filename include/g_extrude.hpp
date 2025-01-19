// standard libraries
#include <string>
#include <vector>

// additional libraries
#include <glm/glm.hpp>

// project libraries
#include "g_plane.hpp"
#include "g_sketch.hpp"

#ifndef G_EXTRUDE_HPP
#define G_EXTRUDE_HPP

class Extrude {
public:
	Extrude(Sketch& sketch, float length, float length_2 = 0, std::string name = "");
	~Extrude();
	void FreeBuffers();

	std::string GetSaveDataString();

	std::vector<glm::vec3> getVertices();

	std::string getName();
	std::string name;

	/*
	This can be improved in the future to add more features
	or be replaced with sketch modifications directly
	(current implementation's changes likely not preserved on save)
	*/
	void ModifyBase(int operation);

	// Graphics functions
	// No need to create/store specific ShaderProgram
	//  -> 3D geometry uses default/primary graphics pipeline
	std::vector<int> TessellateFace(std::vector<glm::vec3> vertices, glm::vec3 faceNormal);

	void PreDraw();
	void Draw();

	float length;
	void MakeMesh();	

	bool selected;

private:
	glm::vec3 color;
	Sketch& sketch;
	
	float length_2;
	std::vector<glm::vec3> vertices;
	bool visible;

	// Graphics functions
	

	GLuint mVAO, mVBO, mEBO;
};

#endif