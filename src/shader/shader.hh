#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>

std::string get_file_content(const char *filename);

class Shader {
    public:
        ~Shader();
        void activate();
        GLuint getProgram();
    protected:
        GLuint createShader(int shaderType, const char *text);
        GLuint program_;
};
