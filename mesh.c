#include <glad/gl.h>
#include "mesh.h"

Mesh mesh_create(float *vertices, int vertex_count) {

    Mesh mesh;
    mesh.vertex_count = vertex_count;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertex_count * 6 * sizeof(float),
        vertices,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        6 * sizeof(float), 
        (void *)0
    );

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *) (3 * sizeof(float))
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return mesh;

}

void mesh_draw(Mesh *mesh) {
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_POINTS, 0, mesh->vertex_count);
    glBindVertexArray(0);
}

void mesh_destroy(Mesh *mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
}