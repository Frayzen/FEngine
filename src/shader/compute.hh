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
    void setupData(void *data, unsigned int element_count,
                   unsigned int element_size, unsigned int bindingPosition,
                   GLenum usage);
    void updateData(void *data, unsigned int bindingPosition);
    void dispatch(GLuint amount);
    const void *retrieveData(unsigned int bindingPosition);

    GLuint getUniformLoc(std::string name);
    ~Compute();

  private:
    GLuint buffers_[MAX_INPUTS];
    void *retrieved_ = nullptr;
};
