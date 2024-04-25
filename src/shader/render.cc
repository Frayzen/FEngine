#include "render.hh"
#include "tools.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void Render::setupShaders() {
    auto vertexContent = get_file_content(vertexFilePath_.c_str());
    auto fragmentContent = get_file_content(fragmentFilePath_.c_str());
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexContent.c_str());
    GLuint fragmentShader =
        createShader(GL_FRAGMENT_SHADER, fragmentContent.c_str());
    program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_);
    GLint linkStatus;
    glGetProgramiv(program_, GL_LINK_STATUS, &linkStatus);
    FAIL_ON(linkStatus == GL_FALSE, "The program could not be linked...");
}

Render::Render(std::string vertexFilePath, std::string fragmentFilePath)
    : fragmentFilePath_(fragmentFilePath), vertexFilePath_(vertexFilePath) {
    setupShaders();
    // Define texture locations
    glUseProgram(program_);
    glUniform1i(glGetUniformLocation(program_, "diffuseText"), 0);
    glUniform1i(glGetUniformLocation(program_, "specularText"), 1);
}

Render::~Render()
{
    glDeleteProgram(program_);
}
