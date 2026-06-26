#include <stdio.h>
#include <glad/gl.h>
#include "glfw3.h"

/* compile - gcc main.c window.c glad/src/gl.c 
-Iglad/include -o main -L. -lglfw3 -lopengl32 -lgdi32 */

GLFWwindow* windowcreate(int width, int height, const char* title) {

    if(!glfwInit()) {
        printf("Failed to initialize GLFW.\n");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    if(window == NULL) {
        printf("Unable to create window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("Failed to load OpenGL\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);

    return window;

}

void windowdestroy(GLFWwindow* window) {

    glfwDestroyWindow(window);
    glfwTerminate();

}