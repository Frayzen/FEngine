#pragma once

#include "shader.hh"
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>

#define MAX_INPUTS 16
class Compute : public Shader {
    public:
        Compute(std::string computeFilePath);
        void setupData(void* data, unsigned int element_count, unsigned int element_size, unsigned int bindingPosition, GLenum usage);
        void dispatch(GLuint amount);
        const void*& retrieveData(unsigned int bindingPosition);
        ~Compute();
    private:
        GLuint buffers_[MAX_INPUTS];
        const void* retrieved_ = nullptr;
};
