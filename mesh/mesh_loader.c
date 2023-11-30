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
            int val = strtol(str + i, &next, 10);
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
void parse_tri(char *line, struct mesh *m)
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
            int id = m->tris_nb;
            if (m->tris_nb < 10)
            {
                printf("heijn %d\n", v[0]);
                printf("%d\n", id);
            }
            m->tris[id].v_ids[0] = v[0];
            m->tris[id].v_ids[1] = v[cur - 1];
            m->tris[id].v_ids[2] = v[cur];
            m->tris[id].vt_ids[0] = vt[0];
            m->tris[id].vt_ids[1] = vt[cur - 1];
            m->tris[id].vt_ids[2] = vt[cur];
            m->tris_nb++;
        }
        cur++;
    }
}

void count_lines(char *line, size_t len, struct mesh *m)
{
    if (len < 3)
        return;
    if (CHECK_V(line))
        m->v_nb++;
    if (CHECK_VT(line))
        m->vt_nb++;
    if (CHECK_TRI(line))
    {
        int i = 0;
        while (line[i])
        {
            while(line[i] && !isspace(line[i])) 
                i++;
            while(line[i] && isspace(line[i])) 
                i++;
            m->tris_nb++;
        }
        m->tris_nb--;
    }
}

void parse(FILE *f, lineParser parser, struct mesh *m)
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

void parse_vertex(char *line, struct mesh *m)
{
    float *v = m->v + m->v_nb * 3;
    if (sscanf(line, "v %f %f %f", v, v+1, v+2) != 3)
        return;
    m->v_nb++;
}

void parse_vtext(char *line, struct mesh *m)
{
    float *vt = m->vt + m->vt_nb * 2;
    if (sscanf(line, "vt %f %f", vt, vt+1) != 2)
        return;
    m->vt_nb++;
}

void parse_lines(char *line, size_t len, struct mesh *m)
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

struct mesh*create_mesh_from_obj(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
        errx(1, "Could not read %s", path);
    struct mesh *m = calloc(1, sizeof(struct mesh));
    parse(f, count_lines, m);
    m->v = malloc(sizeof(float) * 3 * m->v_nb);
    m->vt = malloc(sizeof(float) * 2 * m->vt_nb);
    m->tris = malloc(sizeof(struct tri) * m->tris_nb);
    m->v_nb = 0;
    m->vt_nb = 0;
    m->tris_nb = 0;
    fseek(f, 0, SEEK_SET);
    parse(f, parse_lines, m);
    return m;
}
