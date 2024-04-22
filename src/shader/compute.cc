#include "compute.hh"
#include "tools.hh"
#include "shader.hh"

Compute::Compute(std::string computeFilePath) {
    std::string text = get_file_content(computeFilePath.c_str());
    GLuint cmpShader = createShader(GL_COMPUTE_SHADER, text.c_str());
    program_ = glCreateProgram();
    glAttachShader(program_, cmpShader);
    glLinkProgram(program_);
}

void Compute::setupData(void *data, unsigned int element_count,
                        unsigned int element_size, unsigned int bindingPosition,
                        GLenum usage) {
    FAIL_ON(bindingPosition > MAX_INPUTS, "The input position is too high");
    FAIL_ON(buffers_[bindingPosition], "Reassigning binding position");
    glUseProgram(program_);
    GLuint buf;
    glGenBuffers(1, &buf);
    buffers_[bindingPosition] = buf;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, element_count * element_size, data,
                 usage);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPosition, buf);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Compute::dispatch(GLuint amount) {
    glUseProgram(program_);
    glDispatchCompute(amount, 1, 1);
}

const void *&Compute::retrieveData(unsigned int bindingPosition) {
    glUseProgram(program_);
    GLuint buf = buffers_[bindingPosition];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
    if (retrieved_)
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    retrieved_ = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    return retrieved_;
}

Compute::~Compute() {
    if (retrieved_)
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    Shader::~Shader();
}
