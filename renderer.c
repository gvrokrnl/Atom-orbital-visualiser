#include <glad/gl.h>
#include "glfw3.h"
#include <stdio.h>
#include "renderer.h"

void renderer_clear(void) {

    glClearColor(0.0509f, 0.0509f, 0.1294f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}