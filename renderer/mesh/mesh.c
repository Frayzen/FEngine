#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>

void destroyMesh(mesh *mesh)
{
    if (!mesh)
        return;
    free(mesh->v);
    free(mesh->vt);
    free(mesh->v_ids);
    free(mesh->vt_ids);
    free(mesh);
}

GLint createBuffer(void)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void createVAO(mesh *m)
{
    mesh_graphic *mg = &m->graphic;

    mg->VBO = createBuffer();
    mg->EBO = createBuffer();
    
    //create VAO
    mg->VAO = createBuffer();
    glGenVertexArrays(1, &mg->VAO);
    glBindVertexArray(mg->VAO);

    //begin VAO
    glBindBuffer(GL_ARRAY_BUFFER, mg->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m->v_nb, m->v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mg->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * m->tris_nb, m->v_ids, GL_STATIC_DRAW);
    //end VAO
}

void printMesh(mesh* m)
{
    printf("NBV %d\n", m->v_nb);
    printf("NBVT %d\n", m->vt_nb);
    printf("NBTRI %d\n", m->tris_nb);
    printf("====\n");
    for (size_t i = 0; i < m->v_nb; i++) {
        printf("%f %f %f\n", m->v[3 * i], m->v[3 * i + 1], m->v[3 * i + 2]);
    }
    printf("====\n");
    for (size_t i = 0; i < m->tris_nb; i++) {
        printf("%d\n", m->v_ids[i*3+0]);
        printf("%d\n", m->v_ids[i*3+1]);
        printf("%d\n", m->v_ids[i*3+2]);
        printf("====\n");
    }
}
