#ifndef UTIL_HPP
#define UTIL_HPP

// standard libraries
#include <fstream>
#include <iostream>
#include <string>

// additional libraries
#include <glad/glad.h>

// vvvvvvvvvvvvv Error handling vvvvvvvvvvvvv
void GLClearAllErrors();

// Returns true if error occured
bool GLCheckErrorStatus(const char* function, int line);

#define GLCheck(x) GLClearAllErrors(); x; GLCheckErrorStatus(#x,__LINE__):
// ^^^^^^^^^^^^^ Error handling ^^^^^^^^^^^^^

std::string LoadShaderAsString(const std::string& filename);

GLuint CompileShader(GLuint type, const std::string& source);

GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

void getOpenGLVersionInfo();

void logfile(const std::string& output, const bool append = true, const std::string& filename = "log.txt");

#endif