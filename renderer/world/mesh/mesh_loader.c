#include "mesh_loader.h"

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maths/vector.h"

#define CHECK_V(Line) ((Line)[0] == 'v' && (Line)[1] == ' ')
#define CHECK_VT(Line)                                                         \
    ((Line)[0] == 'v' && (Line)[1] == 't' && (line)[2] == ' ')
#define CHECK_VN(Line)                                                         \
    ((Line)[0] == 'v' && (Line)[1] == 'n' && (line)[2] == ' ')
#define CHECK_TRI(Line) ((Line)[0] == 'f' && (Line)[1] == ' ')

void countLines(char *line, size_t size, meshBuilder *m)
{
    if (size < 2)
        return;
    if (CHECK_V(line))
        m->v_count++;
    else if (CHECK_VT(line))
        m->vt_count++;
    else if (CHECK_VN(line))
        m->vn_count++;
    else if (CHECK_TRI(line))
        m->tris_count++;
}
void createMeshBuilder(char *line, size_t size, meshBuilder *m)
{}

void parse(FILE *f, lineParser parser, meshBuilder *m)
{
    rewind(f);
    char *line = NULL;
    size_t len = 0;
    ssize_t r;
    while ((r = getline(&line, &len, f)) != -1)
    {
        line[len - 1] = '\0';
        parser(line, len - 1, m);
    }
    free(line);
}

mesh *createMeshFromObj(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        errx(1, "Could not open file %s", path);
    meshBuilder *mb = malloc(sizeof(meshBuilder));
    parse(f, countLines, mb);
    mb->v = malloc(sizeof(vec3) * mb->v_count);
    mb->vt = malloc(sizeof(vec2) * mb->vt_count);
    mb->vn = malloc(sizeof(vec3) * mb->vn_count);
    mb->tris_v = malloc(sizeof(vec3i) * mb->tris_count);
    mb->tris_vt = malloc(sizeof(vec3i) * mb->tris_count);
    mb->tris_vn = malloc(sizeof(vec3i) * mb->tris_count);
    parse(f, createMeshBuilder, mb);
    free(mb);
    mesh *m = malloc(sizeof(mesh));
    return m;
}
