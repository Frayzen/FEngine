#include "shader.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int loadShader(GLenum shaderType, const char *source)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(shaderType);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    return vertexShader;
}

GLint createShader(GLenum shaderType, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        errx(1, "Could not open the shader '%s'\n", path);
    fseek(f, 0, SEEK_END);
    unsigned long size = ftell(f);
    char *content = malloc(size);
    rewind(f);
    fread(content, size, 1, f);
    int res = loadShader(shaderType, content);
    if (res == -1)
        errx(1, "Could not open the shader '%s'\n", path);
    free(content);
    return res;
}
