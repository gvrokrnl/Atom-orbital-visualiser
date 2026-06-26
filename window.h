#ifndef WINDOW_H
#define WINDOW_H
#include "glfw3.h"

GLFWwindow *windowcreate(int width, int height, const char *title);
void windowdestroy(GLFWwindow *window);

#endif