#ifndef MESH_H
#define MESH_H

typedef struct {
    unsigned int vao;
    unsigned int vbo;
    int vertex_count;
} Mesh;

Mesh mesh_create(float *vertices, int vertex_count);
void mesh_draw(Mesh *mesh);
void mesh_destroy(Mesh *mesh);

#endif