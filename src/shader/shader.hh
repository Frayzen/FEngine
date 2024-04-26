#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/fwd.hpp>
#include <string>

std::string get_file_content(const char *filename);

class Shader {
    public:
        ~Shader();
        void activate();
        GLuint getProgram();
        void setInt(std::string name, const int val);
        void setFloat(std::string name, const float val);
        void setVec3(std::string name, const glm::vec3& val);
    protected:
        GLuint createShader(int shaderType, const char *text);
        GLuint program_;
};
