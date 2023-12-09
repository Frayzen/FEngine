#include "mesh.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer/renderer.h"

mesh *createMesh(void)
{
    mesh *m = calloc(sizeof(mesh), 1);
    m->meshID = -1;
    return m;
}

void destroyMesh(mesh *mesh)
{
    if (!mesh)
        return;
    free(mesh->v);
    free(mesh->v_ids);
    free(mesh);
}

void addTexture(mesh *m, const char *path)
{
    if (!m->texturesPath)
    {
        m->texturesPath = malloc(sizeof(char *) * 2);
        m->texturesPath[0] = path;
        m->texturesPath[1] = NULL;
        return;
    }
    size_t size = 0;
    while (m->texturesPath[size])
        size++;
    m->texturesPath = realloc(m->texturesPath, sizeof(char *) * (size + 2));
    m->texturesPath[size] = path;
    m->texturesPath[size + 1] = NULL;
}

void printMesh(mesh *m, bool showVertices)
{
    printf("NBV %d\n", m->v_count);
    printf("NBVT %d\n", m->vt_count);
    printf("NBVN %d\n", m->vn_count);
    printf("NBTRI %d\n", m->tris_count);
    printf("====\n");
    if (!showVertices)
        return;
    for (size_t i = 0; i < m->v_count; i++)
    {
        printf("%f %f %f\n", m->v[i].x, m->v[i].y, m->v[i].z);
    }
    printf("====\n");
    for (size_t i = 0; i < m->tris_count; i++)
    {
        printf("%d\n", m->v_ids[i].x);
        printf("%d\n", m->v_ids[i].y);
        printf("%d\n", m->v_ids[i].z);
        printf("====\n");
    }
}
