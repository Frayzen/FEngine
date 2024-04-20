#include "mesh.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define CHECK_V(Line) ((Line)[0] == 'v' && (Line)[1] == ' ')
#define CHECK_VT(Line) ((Line)[0] == 'v' && (Line)[1] == 't' && (line)[2] == ' ')
#define CHECK_TRI(Line) ((Line)[0] == 'f' && (Line)[1] == ' ')

int parse_tri_ids(char *str, int *v, int *vt)
{
    *v = -1;
    *vt = -1;
    int i = 0;
    while(str[i] && !isspace(str[i]))
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            char *next = NULL;
            int val = strtol(str + i, &next, 10) - 1;
            if (*v == -1)
                *v = val;
            else if (*vt != -1)
                *vt = val;
            i = next - str;
        }
        else
            i++;
    }
    while(str[i] && isspace(str[i]))
        i++;
    return i;
}

#define MAX_VERT_PER_FACE 32
void parse_tri(char *line, mesh *m)
{
    line++;
    while(isspace(*line))
        line++;
    int v[MAX_VERT_PER_FACE];
    int vt[MAX_VERT_PER_FACE];
    int cur = 0;
    while (*line)
    {
        line += parse_tri_ids(line, v + cur, vt + cur);
        if (cur >= 2)
        {
            int id = m->tris_count;
            memcpy(m->v_ids + id, v, 3 * sizeof(unsigned int));
            memcpy(m->vt_ids + id, vt, 3 * sizeof(unsigned int));
            m->tris_count++;
        }
        cur++;
    }
}

void count_lines(char *line, size_t len, mesh *m)
{
    if (len < 3)
        return;
    if (CHECK_V(line))
        m->v_count++;
    if (CHECK_VT(line))
        m->vt_count++;
    if (CHECK_TRI(line))
    {
        int i = 0;
        while (line[i])
        {
            while(line[i] && !isspace(line[i])) 
                i++;
            while(line[i] && isspace(line[i])) 
                i++;
            m->tris_count++;
        }
        m->tris_count--;
    }
}

void parse(FILE *f, lineParser parser, mesh *m)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t r;
    while ((r = getline(&line, &len, f)) != -1)
    {
        line[len - 1] = '\0';
        parser(line, len - 1, m);
    }
}

void parse_vertex(char *line, mesh *m)
{
    vec3 *v = m->v + m->v_count;
    if (sscanf(line, "v %f %f %f", &v->x, &v->y, &v->z) != 3)
        return;
    m->v_count++;
}

void parse_vtext(char *line, mesh *m)
{
    vec2 *vt = m->vt + m->vt_count;
    if (sscanf(line, "vt %f %f", &vt->x, &vt->y) != 2)
        return;
    m->vt_count++;
}

void parse_lines(char *line, size_t len, mesh *m)
{
    if (len < 3)
        return;
    if (CHECK_V(line))
        parse_vertex(line, m);
    if (CHECK_VT(line))
        parse_vtext(line, m);
    if (CHECK_TRI(line))
        parse_tri(line, m);
}

mesh *createMeshFromObj(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
        errx(1, "Could not read %s", path);
    mesh *m = calloc(1, sizeof(mesh));
    m->transform.scale = VEC3(1, 1, 1);
    parse(f, count_lines, m);

    // allocate
    m->v = malloc(sizeof(vec3) * m->v_count);
    m->vt = malloc(sizeof(vec3) * m->vt_count);
    m->v_ids = malloc(sizeof(vec3i) * m->tris_count);
    m->vt_ids = malloc(sizeof(vec2i) * m->tris_count);

    // reset
    m->v_count = 0;
    m->vt_count = 0;
    m->tris_count = 0;
    fseek(f, 0, SEEK_SET);
    parse(f, parse_lines, m);
    return m;
}