#include "util.hpp"
#include "globals.hpp"

// vvvvvvvvvvvvv Error handling vvvvvvvvvvvvv
void GLClearAllErrors() {
	while (glGetError() != GL_NO_ERROR) {
	}
}

bool GLCheckErrorStatus(const char* function, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "OpenGL Error: " << error
			<< "\tLine: " << line
			<< "\tFunction: " << function << std::endl;
		return true;
	}
	return false;
}

// #define GLCheck(x) defined in util.hpp
// ^^^^^^^^^^^^^ Error handling ^^^^^^^^^^^^^

std::string LoadShaderAsString(const std::string& filename) {
	std::string result = "";

	std::string line = "";
	std::ifstream inFile(filename.c_str());

	if (inFile.is_open()) {
		while (std::getline(inFile, line)) {
			result += line + '\n';
		}
		inFile.close();
	}

	return result;
}

GLuint CompileShader(GLuint type, const std::string& source) {
	GLuint shaderObject;

	if (type == GL_VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char* src = source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	int result;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char* errorMessage = new char[length];
		glGetShaderInfoLog(shaderObject, length, &length, errorMessage);

		if (type == GL_VERTEX_SHADER) {
			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessage << "\n";
			logfile("ERROR: GL_VERTEX_SHADER compilation failed!");
		}
		else if (type == GL_FRAGMENT_SHADER) {
			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessage << "\n";
			logfile("ERROR: GL_FRAGMENT_SHADER compilation failed!");
		}

		delete[] errorMessage; // clear memory
		glDeleteShader(shaderObject); // delete broken shader

		return 0;
	}

	return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {	
	GLuint programObject = glCreateProgram();
	GLuint newVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint newFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(programObject, newVertexShader);
	glAttachShader(programObject, newFragmentShader);
	glLinkProgram(programObject);
	glValidateProgram(programObject);

	glDetachShader(programObject, newVertexShader);
	glDetachShader(programObject, newFragmentShader);
	glDeleteShader(newVertexShader);
	glDeleteShader(newFragmentShader);

	return programObject;
}

void getOpenGLVersionInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

void logfile(const std::string& output, const bool append, const std::string& filename) {
	if (g.logging and append) {
		std::ofstream outFile(filename, std::ios::app);
		outFile << output << std::endl;
	}
	else if (g.logging) {
		std::ofstream outFile(filename);
		outFile << output << std::endl;
	}
}