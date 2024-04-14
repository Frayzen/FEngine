#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

std::string get_file_content(const char *filename);

class Shader {
    public:
        Shader(std::string vertexFilePath, std::string fragmentFilePath);
        ~Shader();
        void activate();
        GLuint getProgram();
    private:
        void setupShaders();
        GLuint createShader(int shaderType, const char *text);
        const std::string fragmentFilePath_;
        const std::string vertexFilePath_;
        GLuint vertexShader_;
        GLuint fragmentShader_;
        GLuint program_;
};
