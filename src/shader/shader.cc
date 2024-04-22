#include "shader.hh"
#include "tools.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

GLuint Shader::createShader(int shaderType, const char *text) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);
    // Check for compilation errors
    checkGLError("Failed to compile shader");
    std::cout << "Compilation succesful !" << std::endl;
    return shader;
}

void Shader::activate() { glUseProgram(program_); }

std::string get_file_content(const char *filename) {
    std::ifstream file(filename);
    FAIL_ON(!file.is_open(), "Failed to open file: " << filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
Shader::~Shader() { glDeleteProgram(program_); }
GLuint Shader::getProgram() { return program_; }
