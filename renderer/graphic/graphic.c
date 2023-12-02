#include "graphic.h"
#include "renderer/world/mesh/mesh.h"
#include <stdlib.h>

GLint createBuffer(void)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

graphic *createMeshGraphic(mesh *m)
{
    graphic *mg = malloc(sizeof(graphic));

    mg->VBO = createBuffer();
    mg->VNBO = createBuffer();
    mg->EBO = createBuffer();
    
    //create VAO
    mg->VAO = createBuffer();
    glGenVertexArrays(1, &mg->VAO);
    glBindVertexArray(mg->VAO);

    //begin VAO
    glBindBuffer(GL_ARRAY_BUFFER, mg->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->v_count, m->v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mg->VNBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->vn_count, m->vn, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mg->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * m->tris_count, m->v_ids, GL_STATIC_DRAW);
    return mg;    //end VAO
}

