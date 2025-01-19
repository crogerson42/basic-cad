// standard libraries
#include <string>
#include <sstream>

// additional libaries
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

// project libraries
#include "g_plane.hpp" // IMPLEMENTS
#include "globals.hpp"
#include "util.hpp"

GLuint Plane::planeShaderProgram = 0;

void Plane::CreateGraphicsPipelinePlane() {
    std::string vertexShaderSource = LoadShaderAsString("./shaders/g_plane_vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/g_plane_frag.glsl");
    Plane::planeShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

Plane::Plane() : Plane(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, "") {
	// Default constructor
}

Plane::Plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color, std::string name) {
	this->point = point;
	this->normal = glm::normalize(normal);
	this->color = color;
    this->visible = true;
    this->name = name;

    this->mVAO = 0;
    this->mVBO = 0;
    this->mEBO = 0;
}

Plane::Plane(float x, float y, float z, float nx, float ny, float nz, std::string name) {
    // also used by default constructor
	this->point = glm::vec3(x, y, z);
	this->normal = glm::normalize(glm::vec3(nx, ny, nz));
	this->color = glm::vec3(0, 0, 0); // black
    this->visible = true;
    this->name = name;

    this->mVAO = 0;
    this->mVBO = 0;
    this->mEBO = 0;
}

Plane::Plane(const Plane& ref_plane, float offset, std::string name) {
	this->normal = ref_plane.getNormal();
	this->point = ref_plane.getPoint() + offset * normal;
    this->color = glm::vec3(0, 0, 0); // black
    this->visible = true;
    this->name = name;

    this->mVAO = 0;
    this->mVBO = 0;
    this->mEBO = 0;
}

Plane::~Plane() {

}

void Plane::FreeBuffers() {
    // only free OpenGL buffers if removing object from main/global datastructures
    // this avoids OpenGL errors with copying objects
    // logfile("Freeing VAO " + std::to_string(mVAO) + " and deleting <Plane> " + name);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteVertexArrays(1, &mVAO);

    this->mVAO = 0;
    this->mVBO = 0;
    this->mEBO = 0;
}

std::string Plane::GetSaveDataString() {
    std::stringstream data;
    data << "plane " << name <<
        "\np " << point.x << " " << point.y << " " << point.z <<
        "\nn " << normal.x << " " << normal.y << " " << normal.z <<
        "\nc " << color.r << " " << color.g << " " << color.b <<
        "\n/plane\n";
    return data.str();
}

void Plane::InitializeGL() {
    // Define vertices for quad (plane) in 3D

    // float zoom = glm::length(g.gCamera.GetCameraPos() - this->point) / 10; // for drawing coordinate planes at constant size
    float zoom = 3.0f;

    glm::vec3 p1_xy(-zoom, zoom, 0.0f);    // Top-left
    glm::vec3 p2_xy(zoom, zoom, 0.0f);     // Top-right
    glm::vec3 p3_xy(zoom, -zoom, 0.0f);    // Bottom-right
    glm::vec3 p4_xy(-zoom, -zoom, 0.0f);   // Bottom-left

    glm::vec3 p1 = Plane::transformPointBetweenPlanes(p1_xy, g.plane_xy, *this);    // Top-left
    glm::vec3 p2 = Plane::transformPointBetweenPlanes(p2_xy, g.plane_xy, *this);     // Top-right
    glm::vec3 p3 = Plane::transformPointBetweenPlanes(p3_xy, g.plane_xy, *this);    // Bottom-right
    glm::vec3 p4 = Plane::transformPointBetweenPlanes(p4_xy, g.plane_xy, *this);   // Bottom-left

    GLfloat vertices[] = {
        // Positions      // Normals                    // Colors
        p1.x, p1.y, p1.z, normal.x, normal.y, normal.z, color.r, color.g, color.b,  // Top-left
        p2.x, p2.y, p2.z, normal.x, normal.y, normal.z, color.r, color.g, color.b,  // Top-right
        p3.x, p3.y, p3.z, normal.x, normal.y, normal.z, color.r, color.g, color.b,  // Bottom-right
        p4.x, p4.y, p4.z, normal.x, normal.y, normal.z, color.r, color.g, color.b   // Bottom-left
    };

    // Indices for the quad (two triangles)
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    if (this->mVAO == 0) {
        glGenVertexArrays(1, &mVAO);
    }
    if (this->mVBO == 0) {
        glGenBuffers(1, &mVBO);
    }
    if (this->mEBO == 0) {
        glGenBuffers(1, &mEBO);
    }

    glBindVertexArray(mVAO);

    // Populate buffers
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

std::string Plane::getName() const {
    return this->name;
}

glm::vec3 Plane::getPoint() const {
	return this->point;
}

glm::vec3 Plane::getNormal() const {
	return this->normal;
}

bool Plane::isVisible() {
	return this->visible;
}

void Plane::show(bool visible) {
	this->visible = visible;
}

glm::vec3 Plane::transformPointBetweenPlanes(const glm::vec3& pointA, const Plane& src, const Plane& dest) {

    const glm::vec3& pointOnPlaneA = src.getPoint();
    const glm::vec3& normalA = src.getNormal();
    const glm::vec3& pointOnPlaneB = dest.getPoint();
    const glm::vec3& normalB = dest.getNormal();

    // Step 1: Translate point to Plane A's local origin
    glm::vec3 localPointA = pointA - pointOnPlaneA;

    // Step 2: Compute rotation to align Plane A's normal to Plane B's normal
    glm::vec3 rotationAxis = glm::cross(normalA, normalB);
    float rotationAngle = acos(glm::dot(normalA, normalB) / (glm::length(normalA) * glm::length(normalB)));

    // Avoid rotation if the planes are parallel
    glm::vec3 rotatedPoint;
    if (glm::length(rotationAxis) > 0.0f) {
        rotatedPoint = glm::rotate(localPointA, rotationAngle, glm::normalize(rotationAxis));
    }
    else {
        rotatedPoint = localPointA;
    }

    // Step 3: Translate the point to Plane B's coordinate system
    glm::vec3 pointB = rotatedPoint + pointOnPlaneB;

    return pointB;
}

void Plane::PreDraw() {
    // Initialize graphics data only if necessary (not yet done, or changed)
    if (this->mVBO == 0) {
        InitializeGL();
    }

    // Only render if set to visible
    if (!visible) {
        return;
    }

    // Bind shader program
    glUseProgram(planeShaderProgram);

    // Transformation matrices (model, view, projection)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 view = g.gCamera.GetViewMatrix();
    glm::mat4 projection = g.gCamera.GetProjectionMatrix();

    // Bind uniforms for transformation
    // Model Matrix
    GLuint u_ModelMatrixLocation = glGetUniformLocation(planeShaderProgram, "u_ModelMatrix");
    if (u_ModelMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }
    else {
        std::cout << "Could not find u_ModelMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ModelMatrix; maybe a mispelling?\n");
        //exit(EXIT_FAILURE);
    }

    // View Matrix
    GLuint u_ViewMatrixLocation = glGetUniformLocation(planeShaderProgram, "u_ViewMatrix");
    if (u_ViewMatrixLocation >= 0) {
        glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &view[0][0]);
    }
    else {
        std::cout << "Could not find u_ViewMatrix; maybe a mispelling?\n";
        logfile("Could not find u_ViewMatrix; maybe a mispelling?\n");
        //exit(EXIT_FAILURE);
    }

    // Projection Matrix
    GLuint u_ProjectionLocation = glGetUniformLocation(planeShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    }
    else {
        std::cout << "Could not find u_Projection; maybe a mispelling?\n";
        logfile("Could not find u_Projection; maybe a mispelling?\n");
        //exit(EXIT_FAILURE);
    }
}

void Plane::Draw() {
    // Only render if set to visible
    if (!visible) {
        return;
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
}

