// standard libraries
#include <string>
#include <sstream>

// additional libraries
#include <glm/gtx/transform.hpp>

// project libraries
#include "g_extrude.hpp" // IMPLEMENTS
#include "globals.hpp"
#include "util.hpp"

Extrude::Extrude(Sketch& sketch, float length, float length_2, std::string name)
    : sketch(sketch) {
	// geometry properties
	this->length = length;
	this->length_2 = length_2;
    this->name = name;
    
    // visual properties
    this->color = glm::vec3(0.5, 0.5, 0.5);
    this->visible = true;
    this->selected = false;

	// this->MakeMesh();

	this->mVAO = 0;
	this->mVBO = 0;
	this->mEBO = 0;
}

Extrude::~Extrude() {

}

std::string Extrude::GetSaveDataString() {
    std::stringstream data;
    data << "extrude " << name <<
        "\ns " << this->sketch.name <<
        "\nl " << length <<
        "\n/extrude\n";
    return data.str();
}

void Extrude::FreeBuffers() {
    // only free OpenGL buffers if removing object from main/global datastructures
    // this avoids OpenGL errors with copying objects
    // logfile("Freeing VAO " + std::to_string(mVAO) + " and deleting <Extrude> " + name);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);

    this->mVAO = 0;
    this->mVBO = 0;
    this->mEBO = 0;
}

void Extrude::ModifyBase(int operation) {
    std::vector<glm::vec3>& vertices = sketch.getVertices(); // Reference to the vertices vector

    const glm::vec3& normal = sketch.getPlane().getNormal(); // Reference to the sketch plane normal

    // Modify vertices based on the operation
    for (auto& vertex : vertices) {
        if (operation == 0) {
            vertex += normal; // Move vertex along the plane's normal
        }
        else if (operation == 1) {
            vertex -= normal; // Move vertex against the plane's normal
        }

        // move laterally in x-z plane -> change to sketch plane in future
        else if (operation == 2) {
            vertex += glm::vec3(0,0,-1); // Move vertex against the plane's normal
        }
        else if (operation == 3) {
            vertex += glm::vec3(0,0,1); // Move vertex against the plane's normal
        }
        else if (operation == 4) {
            vertex += glm::vec3(-1,0,0); // Move vertex against the plane's normal
        }
        else if (operation == 5) {
            vertex += glm::vec3(1,0,0); // Move vertex against the plane's normal
        }
    }
}

std::vector<int> Extrude::TessellateFace(std::vector<glm::vec3> vertices, glm::vec3 faceNormal) {
    std::vector<int> triangle_indices;

    std::vector<int> vertex_order;
    for (int i = 0; i < vertices.size(); i++) {
        vertex_order.push_back(i);
    }

    int i = 0;
    // for (int i = 0; i < vertex_order.size(); i++) {
    while(vertex_order.size() > 2) {
        int prev = vertex_order[(i - 1 + vertex_order.size()) % vertex_order.size()];
        int here = vertex_order[i % vertex_order.size()]; // modulo performed in case list must be traversed twice+ (i++ > remaining size)
        int next = vertex_order[(i + 1) % vertex_order.size()];

        // i = vertex 'b' out of ordered vertices a->b->c
        // to determine if angle abc is convex (make triangle) or concave (ignore),
        // we check if point c is left (inside*) or right (outside*) of line ab
        // * assumes counter-clockwise ordering of vertices on face (can enhance later)
        glm::vec3 edge_ab = vertices[here] - vertices[prev];
        glm::vec3 edge_ac = vertices[next] - vertices[prev];

        glm::vec3 crossP = glm::cross(edge_ab, edge_ac);

        float dotP = glm::dot(crossP, faceNormal);

        if (dotP > 0) {
            // angle < 180: make triangle
            triangle_indices.push_back(prev);
            triangle_indices.push_back(here); // vertex_order[i]
            triangle_indices.push_back(next);

            vertex_order.erase(vertex_order.begin() + i); // remove i from vertex_order
        }
        else if (vertex_order.size() == 3 or i > vertices.size()) {
            // last triangle reached but dot product invalid OR traversed through too many vertices
            // tessellation failed; render hollow
            // THIS SHOULD BE IMPROVED IN A FUTURE VERSION TO ENSURE ALL TESSELLATION SUCCEEDS
            logfile("Tessellation failed; rendering hollow instead");
            return {};
        }
        else {
            // angle > 180: skip
            i++;
        }
    }

    for (i = 0; i < triangle_indices.size(); i++) {
        // logfile(std::to_string(triangle_indices[i]));
    }

    return triangle_indices;
}

void Extrude::MakeMesh() {
	this->vertices.clear();

    std::vector<GLuint> indices;
	std::vector<glm::vec3> baseVertices = this->sketch.getVertices();
	glm::vec3 extrusion_dir = this->sketch.getPlane().getNormal();
	glm::vec3 h_offset_vector = extrusion_dir * this->length;

    std::vector<int> base_indices = Extrude::TessellateFace(baseVertices, extrusion_dir);

    std::vector<GLfloat> vertexData;

	for (int i = 0; i < baseVertices.size(); i++) {
		vertices.push_back(baseVertices[i]);

        vertexData.push_back(baseVertices[i].x);
        vertexData.push_back(baseVertices[i].y);
        vertexData.push_back(baseVertices[i].z);

        // calculate vertex normals using 3 surrounding faces
        // vertices along base
        glm::vec3 vert_prev = baseVertices[(i - 1 + baseVertices.size()) % baseVertices.size()];
        glm::vec3 vert_here = baseVertices[i];
        glm::vec3 vert_next = baseVertices[(i + 1) % baseVertices.size()];

        // edges incident to middle vertex along base (2x)
        glm::vec3 edge_in = vert_here - vert_prev;
        glm::vec3 edge_out = vert_next - vert_here;

        // normals of 3 adjacent faces
        glm::vec3 norm_1 = glm::cross(edge_in, extrusion_dir);
        glm::vec3 norm_2 = glm::cross(edge_out, extrusion_dir);
        glm::vec3 norm_b = -extrusion_dir;

        // overall vertex normal
        glm::vec3 vert_norm = glm::normalize(norm_1 + norm_2 + norm_b);
        vertexData.push_back(vert_norm.x); // normals
        vertexData.push_back(vert_norm.y);
        vertexData.push_back(vert_norm.z);

        if (selected) {
            vertexData.push_back(1.0); // selected = "orange"
            vertexData.push_back(0.5);
            vertexData.push_back(0.0);
        }
        else {
            vertexData.push_back(this->color.r);
            vertexData.push_back(this->color.g);
            vertexData.push_back(this->color.b);
        }

        vertices.push_back(glm::vec3(baseVertices[i] + h_offset_vector));

        vertexData.push_back((baseVertices[i] + h_offset_vector).x);
        vertexData.push_back((baseVertices[i] + h_offset_vector).y);
        vertexData.push_back((baseVertices[i] + h_offset_vector).z);

        // face on extruded-end has normal opposite to 'extrusion direction'
        vert_norm = glm::normalize(norm_1 + norm_2 - norm_b);

        vertexData.push_back(vert_norm.x); // normals
        vertexData.push_back(vert_norm.y);
        vertexData.push_back(vert_norm.z);

        if (selected) {
            vertexData.push_back(1.0); // selected = "orange"
            vertexData.push_back(0.5);
            vertexData.push_back(0.0);
        }
        else {
            vertexData.push_back(this->color.r);
            vertexData.push_back(this->color.g);
            vertexData.push_back(this->color.b);
        }
	}

    for (int i = 0; i < vertices.size(); i = i + 2) {
        int a = i % vertices.size();
        int b = (i + 1) % vertices.size();
        int c = (i + 2) % vertices.size();
        int d = (i + 3) % vertices.size();
        
        // first triangle in face
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
        
        // second triangle in face
        indices.push_back(b);
        indices.push_back(d);
        indices.push_back(c);
    }

    // bottom face
    for (int i = 0; i < base_indices.size(); i++) {
        indices.push_back(base_indices[i] * 2);
    }

    // top face
    for (int i = 0; i < base_indices.size(); i++) {
        indices.push_back(base_indices[i] * 2 + 1);
    }

    // Create buffers if (and only if) mesh is being generated for the first time
    // No need to re-create if mesh is instead being refreshed after a change
    if (mVAO == 0) {
        glGenVertexArrays(1, &mVAO);
    }
    if (mVBO == 0) {
        glGenBuffers(1, &mVBO);
    }
    if (mEBO == 0) {
        glGenBuffers(1, &mEBO);
    }

    glBindVertexArray(mVAO);

    // Populate buffers
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

    // Unbind currently bound VAO
    glBindVertexArray(0);

    // Disable previously opened attributes - not sure if necessary
    //glDisableVertexAttribArray(0);
    //glDisableVertexAttribArray(1);
    //glDisableVertexAttribArray(2);
}

std::vector<glm::vec3> Extrude::getVertices() {
	return this->vertices;
}

std::string Extrude::getName() {
    return this->name;
}

void Extrude::PreDraw() {
    // Only render if set to visible
    if (!visible) {
        return;
    }

    // Initialize graphics data only if necessary (not yet done, or changed)
    if (this->mVBO == 0) {
        MakeMesh();
    }

    // Bind shader program
    glUseProgram(g.gGraphicsPipelineShaderProgram);

    // Transformation matrices (model, view, projection)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 view = g.gCamera.GetViewMatrix();
    glm::mat4 projection = g.gCamera.GetProjectionMatrix();

    glm::vec3 viewPosition = g.gCamera.GetCameraPos(); // also used as light location (for now)

    // Bind uniforms for transformation
    // Model Matrix
    GLuint u_ModelMatrixLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ModelMatrix; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // View Matrix
    GLuint u_ViewMatrixLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if (u_ViewMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &view[0][0]);
    }
    else {
        std::cout << "Could not find u_ViewMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ViewMatrix; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // Projection Matrix
    GLuint u_ProjectionLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection; maybe a mispelling?\n";
        logfile("Could not find u_Projection; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }

    // Light Position
    GLuint u_viewPosition = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_viewPosition");
    if (u_viewPosition >= 0) {
        glUniform3fv(u_viewPosition, 1, &viewPosition[0]);
    }
    else {
        std::cout << "Could not find u_viewPosition; maybe a mispelling?\n";
        logfile("Could not find u_viewPosition; maybe a mispelling?\n");
        exit(EXIT_FAILURE);
    }
}

void Extrude::Draw() {
    // Only render if set to visible
    if (!visible) {
        return;
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, vertices.size() * 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
}