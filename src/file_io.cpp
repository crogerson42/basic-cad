// standard libraries
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// project libraries
#include "file_io.hpp" // IMPLEMENTS
#include "g_structs.hpp"
#include "globals.hpp"
#include "util.hpp"

void LoadObjectFromFile(std::string modelFilePath) {
    // locate file; return nothing if cannot be found
    std::ifstream inputFile(modelFilePath);
    if (!inputFile) {
        logfile("Error opening file: " + modelFilePath);
        return;
    }

    std::string object_type = "";
    std::string object_name = "";

    std::vector<GeoData_Plane> imported_planes;
    std::vector<GeoData_Sketch> imported_sketches;
    std::vector<GeoData_Extrude> imported_extrudes;

    GeoData_Plane* newPlane = nullptr;
    GeoData_Sketch* newSketch = nullptr;
    GeoData_Extrude* newExtrude = nullptr;

    // read each line of the file
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#') { // ignore blank lines and comments
            continue;
        }

        // convert each line to a stream and check the first "word" to check incoming data type
        std::istringstream stream(line);
        std::string token;
        stream >> token;  // get first word

        // seek next object
        if (object_type == "") {
            if (token == "plane") {
                object_type = token;
                stream >> object_name;
                newPlane = new GeoData_Plane();
            }
            else if (token == "sketch") {
                object_type = token;
                stream >> object_name;
                newSketch = new GeoData_Sketch();
            }
            else if (token == "extrude") {
                object_type = token;
                stream >> object_name;
                newExtrude = new GeoData_Extrude();
            }
        }

        // read plane data (point, normal, <optional>color)
        else if (object_type == "plane") {
            if (token == "/plane") {
                // done reading data; create new plane
                if (newPlane) {
                    Plane newPlaneObject = Plane(newPlane->point, newPlane->normal, newPlane->color, object_name);
                    g.AddPlane(newPlaneObject);
                    delete newPlane; // Free allocated memory
                    newPlane = nullptr;
                }  
                object_type = "";
            }
            else if (token == "p") {
                stream >> newPlane->point.x >> newPlane->point.y >> newPlane->point.z; // Plane reference point
            }
            else if (token == "n") {
                stream >> newPlane->normal.x >> newPlane->normal.y >> newPlane->normal.z; // Plane normal vector
            }
            else if (token == "c") {
                stream >> newPlane->color.r >> newPlane->color.g >> newPlane->color.b; // Plane color
            }
        }

        // read sketch data (vertices x,y)
        else if (object_type == "sketch") {
            if (token == "/sketch") {
                // done reading data; create new sketch
                int parent_plane = -1;
                for (int i = 0; i < g.gPlanes.size(); i++) {
                    const auto& plane = g.gPlanes[i];
                    if (plane.getName() == newSketch->plane) {
                        parent_plane = static_cast<int>(i);
                        break;
                    }
                }
                if (parent_plane != -1) {
                    // create sketch
                    Sketch newSketchObject = Sketch(g.gPlanes[parent_plane], object_name);
                    // add all vertices
                    for (int i = 0; i < newSketch->vertices.size(); i++) {
                        newSketchObject.addVertex(newSketch->vertices[i]);
                    }
                    // add to application
                    g.AddSketch(newSketchObject);
                }
                // else reference not found; ignore
                // cleanup
                delete newSketch;
                newSketch = nullptr;

                object_type = "";
            }
            else if (token == "p") {
                // plane id
                stream >> newSketch->plane;
            }
            else if (token == "v") {
                glm::vec2 p; // Vertex p;
                stream >> p.x >> p.y; // coordinates
                newSketch->vertices.push_back(p); // add vertex to sketch
            }
        }

        // read extrude data (sketch_id, length)
        else if (object_type == "extrude") {
            if (token == "/extrude") {
                // done reading data; create new extrude
                int parent_sketch = -1;
                for (int i = 0; i < g.gSketches.size(); i++) {
                    if (g.gSketches[i].getName() == newExtrude->sketch) {
                        parent_sketch = i;
                        break;
                    }
                }
                if (parent_sketch != -1) {
                    Extrude newExtrudeObject = Extrude(g.gSketches[parent_sketch], newExtrude->length);
                    g.AddExtrude(newExtrudeObject);
                }
                delete newExtrude;
                newExtrude = nullptr;
                
                object_type = "";
            }
            else if (token == "s") {
                // sketch id
                stream >> newExtrude->sketch;
            }
            else if (token == "l") {
                // length
                stream >> newExtrude->length;
            }
            else if (token == "l2") {
                // future support for two-directional extrudes
            }
        }
    }

    inputFile.close();
}