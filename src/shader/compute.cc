#include "compute.hh"
#include "tools.hh"
#include "shader.hh"
#include <cassert>

Compute::Compute(std::string computeFilePath) {
    std::string text = get_file_content(computeFilePath.c_str());
    GLuint cmpShader = createShader(GL_COMPUTE_SHADER, text.c_str());
    program_ = glCreateProgram();
    glAttachShader(program_, cmpShader);
    glLinkProgram(program_);
    checkGLError("Error while linking compute shader");
}

GLuint Compute::getBuffer(unsigned int bindingPosition) {
    glUseProgram(program_);
    assert(buffers_[bindingPosition] != 0);
    return buffers_[bindingPosition];
}
void Compute::setupData(unsigned int bindingPosition, GLuint buffer) {
    glUseProgram(program_);
    buffers_[bindingPosition] = buffer;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPosition, buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
void Compute::setupData(void *data, int datasize, unsigned int bindingPosition,
                        GLenum usage) {
    FAIL_ON(bindingPosition > MAX_INPUTS, "The input position is too high");
    glUseProgram(program_);
    bool exist = buffers_[bindingPosition] != 0;
    GLuint buf = buffers_[bindingPosition];
    if (!exist) {
        glGenBuffers(1, &buf);
        buffers_[bindingPosition] = buf;
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, datasize, data, usage);
    if (!exist)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPosition, buf);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Compute::updateData(void *data, unsigned int bindingPosition) {
    glUseProgram(program_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers_[bindingPosition]);
    GLint size = 0;
    glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
}

GLuint Compute::getUniformLoc(std::string name) {
    glUseProgram(program_);
    return glGetUniformLocation(program_, name.c_str());
}

void Compute::dispatch(GLuint amount) {
    glUseProgram(program_);
    glDispatchCompute(amount, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

bool Compute::isMapped(unsigned int bindingPosition) {
    GLint mapped;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers_[bindingPosition]);
    glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_MAPPED, &mapped);
    return mapped;
}

const void *Compute::retrieveData(unsigned int bindingPosition) {
    glUseProgram(program_);
    GLuint buf = buffers_[bindingPosition];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    glFinish();
    return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
}

Compute::~Compute() { Shader::~Shader(); }
