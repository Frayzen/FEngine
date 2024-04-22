#pragma once

#include "shader.hh"
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

class Render : public Shader {
    public:
        Render(std::string vertexFilePath, std::string fragmentFilePath);
    private:
        void setupShaders();
        const std::string fragmentFilePath_;
        const std::string vertexFilePath_;
        GLuint vertexShader_;
        GLuint fragmentShader_;
};
