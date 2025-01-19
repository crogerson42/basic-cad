// standard libraries
#include <fstream>
#include <string>
#include <vector>

// additional libraries
#include "glm/glm.hpp"
#include <SDL2/SDL.h>

// project libraries
#include "g_plane.hpp"
#include "g_sketch.hpp"
#include "g_extrude.hpp"

#ifndef TESTS_HPP
#define TESTS_HPP

void testSimpleExtrude() {
    Plane plane = Plane(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    Sketch sketch = Sketch(plane);
    sketch.addVertex(0, 0, 0);
    sketch.addVertex(0, 1, 0);
    sketch.addVertex(1, 1, 0);
    sketch.addVertex(1, 0, 0);

    Extrude extrude = Extrude(sketch, 2);

    std::ofstream outFile("testSimpleExtrude.txt");
    std::vector<glm::vec3> vertices = extrude.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        outFile << vertices[i].x << "," << vertices[i].y << "," << vertices[i].z << std::endl;
    }
}

void testAngleExtrude() {
    Plane plane = Plane(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    Sketch sketch = Sketch(plane);
    sketch.addVertex(0, 0, 0);
    sketch.addVertex(0, 1, 0);
    sketch.addVertex(1, 1, 0);
    sketch.addVertex(1, 0, 0);

    Extrude extrude = Extrude(sketch, 2);

    std::ofstream outFile("testAngleExtrude.txt");
    std::vector<glm::vec3> vertices = extrude.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        outFile << vertices[i].x << "," << vertices[i].y << "," << vertices[i].z << std::endl;
    }
}


#endif