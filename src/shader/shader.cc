#include "shader.hh"
#include "tools.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>

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

void Shader::setInt(std::string name, const int val) {
    glUniform1i(glGetUniformLocation(program_, name.c_str()), val);
}
void Shader::setFloat(std::string name, const float val) {
    glUniform1f(glGetUniformLocation(program_, name.c_str()), val);
}
void Shader::setVec3(std::string name, const glm::vec3 &val) {
    glUniform3fv(glGetUniformLocation(program_, name.c_str()), 1, &val[0]);
}

void Shader::setMat4(std::string name, const glm::mat4 &val) {
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1,
                       GL_FALSE, &val[0][0]);
}
