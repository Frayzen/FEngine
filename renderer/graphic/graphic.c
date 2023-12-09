#include "graphic.h"

#include <stdlib.h>

#include "renderer/renderer.h"
#include "renderer/world/mesh/mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

unsigned int createTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        fprintf(stderr, "Something went wrong while creating the texture %s\n",
                path);
        return 0;
    }
    // set the texture wrapping/filtering options (on the currently bound
    // texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return 1;
}

void attachTextures(graphic *g, const char **paths)
{
    // get number of attachTextures
    int nbTextures = 0;
    while (paths[nbTextures])
        nbTextures++;
    // create textures
    g->textArray = malloc(sizeof(GLuint) * nbTextures);
    glGenTextures(nbTextures, g->textArray);
    // attach textures
    renderer *r = GET_RENDERER;
    for (int i = 0; i < nbTextures; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, g->textArray[i]);
        createTexture(paths[i]);
        // set texture uniform
        glUniform1i(glGetUniformLocation(r->shaderProgram, "texture1"), 0);
    }
}

GLint createBuffer(void)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void destroyGraphic(graphic *g)
{
    if (!g)
        return;
    unsigned int nbTextures = 0;
    if (g->textArray)
    {
        while (g->textArray[nbTextures])
            nbTextures++;
    }
    if (g->textArray)
        glDeleteTextures(nbTextures, g->textArray);
    glDeleteBuffers(1, &g->VBO);
    glDeleteBuffers(1, &g->VNBO);
    glDeleteBuffers(1, &g->VTBO);
    glDeleteBuffers(1, &g->EBO);
    glDeleteVertexArrays(1, &g->VAO);
    free(g);
}

graphic *createMeshGraphic(mesh *m)
{
    graphic *mg = malloc(sizeof(graphic));

    mg->VBO = createBuffer();
    mg->VNBO = createBuffer();
    mg->EBO = createBuffer();
    mg->textArray = 0;

    // create VAO
    mg->VAO = createBuffer();
    glGenVertexArrays(1, &mg->VAO);
    glBindVertexArray(mg->VAO);

    // begin VAO
    // create VBO
    glBindBuffer(GL_ARRAY_BUFFER, mg->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->v_count, m->v,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // create EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mg->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned int) * 3 * m->tris_count, m->v_ids,
                 GL_STATIC_DRAW);

    // create VNBO
    glBindBuffer(GL_ARRAY_BUFFER, mg->VNBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->vn_count, m->vn,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    // create VTBO
    glBindBuffer(GL_ARRAY_BUFFER, mg->VTBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m->vt_count, m->vt,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(2);

    // attachTextures
    if (m->texturesPath)
        attachTextures(mg, m->texturesPath);
    // end VAO
    glBindVertexArray(0);
    return mg;
}
