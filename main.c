#include <stdio.h>
#include <glad/gl.h>
#include "glfw3.h"
#include "window.h"
#include "renderer.h"
#include "shader.h"
#include "mesh.h"
#include "orbital.h"
#include <stdlib.h>
#include "time.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int n = 5;
int l = 4;
int m = 2;
float alpha = 0.5f;
unsigned char type = 1;
int point_count = 50000;



float last_mouse_x;
float last_mouse_y;
int is_mouse_pressed;
int need_geometry_update = 0;


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
   if (button == GLFW_MOUSE_BUTTON_LEFT) {
      if (action == GLFW_PRESS) {
         is_mouse_pressed = 1;
         double mouse_x, mouse_y;
         glfwGetCursorPos(window, &mouse_x, &mouse_y);
         last_mouse_x = (float)mouse_x;
         last_mouse_y = (float)mouse_y;
         printf("Click, coords: %f, %f\n", last_mouse_x, last_mouse_y);
      } else if (action == GLFW_RELEASE) {
         is_mouse_pressed = 0;
         printf("HyiHitlera\n");
      }
   }
}

float cam_rot_x = 0.0f;
float cam_rot_y = 0.0f;
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
   if (is_mouse_pressed == 1) {
      double dx = xpos - last_mouse_x;
      double dy = ypos - last_mouse_y;
      cam_rot_x -= (float)dy * 0.005f;
      cam_rot_y += (float)dx * 0.005f;
      last_mouse_x = (float)xpos;
      last_mouse_y = (float)ypos;
      printf("Angles: x = %f, y = %f\n", cam_rot_x, cam_rot_y);
   }
}

int read_int(const char *name, int current, int min, int max) {
   char input[64];
   int value;
   char extra;
   
   printf("%s = [%d]\n", name, current);
   printf("New %s: ", name);
   fflush(stdout);

   if (!fgets(input, sizeof(input), stdin)) {
      return current;
   }

   if (input[0] == '\n') {
      return current;
   }

   if (sscanf(input, " %d %c", &value, &extra) != 1) {
      printf("Error. Enter only number\n");
      return current;
   }

   if (value < min || value > max) {
      printf("Error. %s must be from %d to %d\n", name, min, max);
      return current;
   }

   need_geometry_update = 1;
   return value;
}

float read_float(const char *name, float current, float min, float max) {
   char input[64];
   float value;
   char extra;

   printf("%s = [%f]\n", name, current);
   printf("New %s: ", name);
   fflush(stdout);

   if (!fgets(input, sizeof(input), stdin)) {
      return current;
   }

   if (input[0] == '\n') {
      return current;
   }

   if (sscanf(input, " %f %c", &value, &extra) != 1) {
      printf("Error. Enter only number\n");
      return current;
   }

   if (value < min || value > max) {
      printf("Error. %s must be from %.2f to %.2f\n", name, min, max);
      return current;
   }

   need_geometry_update = 1;
   return value;
}

void clear_console(void) {
   printf("\033[2J\033[H");
   fflush(stdout);
}

void settings_menu(void) {
   char input[64];
   int choice;
   char extra;

   while (1) {
      clear_console();

      printf("===== Settings =====\n\n");
      printf("1. N          = %d\n", n);
      printf("2. L          = %d\n", l);
      printf("3. M          = %d\n", m);
      printf("4. Alpha      = %f\n", alpha);
      printf("5. Color type = %d\n", type);
      printf("\n0. Exit\n");

      printf("\nChoose option: \n");
      fflush(stdout);

      if (!fgets(input, sizeof(input), stdin)) {
         return;
      }

      if (sscanf(input, " %d %c", &choice, &extra) != 1) {
         printf("Error. Enter menu number\n");
         printf("Press Enter.");
         fgets(input, sizeof(input), stdin);
         continue;
      }

      clear_console();

      switch (choice) {
         case 1:
            n = read_int("N", n, 1, 10000);
            break;

         case 2: 
            l = read_int("L", l, 0, (n - 1));
            break;

         case 3:
            m = read_int("M", m, -l, l);
            break;

         case 4:
            alpha = read_float("Alpha", alpha, 0.0f, 1.0f);
            break;

         case 5:
            type = read_int("Color type", type, 1, 2);
            break;

         case 0:
            return;

         default:
            printf("Unknown option\n");
            break;
      }

      printf("\nPress Enter.");
      fgets(input, sizeof(input), stdin);
   }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      settings_menu();
   }
}

int main() {
   srand(time(NULL));
   GLFWwindow *window = windowcreate(640, 480, "Atom");

   if (window == NULL) {
    printf("Unable to create window.\n");
    return 1;
   }

   glfwSetMouseButtonCallback(window, mouse_button_callback);
   glfwSetCursorPosCallback(window, cursor_position_callback);
   glfwSetKeyCallback(window, key_callback);

   //gen points
   
   Particle *particles = malloc(point_count * sizeof(Particle));
   float *vertices = malloc(point_count * 6 * sizeof(float));
   glPointSize(2.0f);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    BUILD_ORBITAL(vertices, point_count, 4, 3, 1);
// ORBITAL_SPHERE_TEST(vertices, point_count);
   


   //make shader
   const char *vertex_source =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "layout (location = 1) in vec3 aColor;\n"
      "out vec3 ourColor;\n"
      "uniform float u_angleX;\n"
      "uniform float u_angleY;\n"
      "uniform float u_aspect;\n"
      "void main() {\n"

      "  float x1 = aPos.x * cos(u_angleY) - aPos.z * sin(u_angleY);\n"
      "  float z1 = aPos.x * sin(u_angleY) + aPos.z * cos(u_angleY);\n"

      "  float y2 = aPos.y * cos(u_angleX) - z1 * sin(u_angleX);\n"
      "  float z2 = aPos.y * sin(u_angleX) + z1 * cos(u_angleX);\n"
      "  gl_Position = vec4(x1 / u_aspect, y2, z2, 1.0);\n"
      "  ourColor = aColor;\n"
      "}\n";

   const char *fragment_source = 
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "in vec3 ourColor;\n"
      "uniform float alpha;"
      "void main() {\n"
      "  FragColor = vec4(ourColor, alpha);\n"
      "}\n";

   unsigned int shader = shader_create(
      vertex_source, 
      fragment_source
   );

   // INIT_ORBITAL_PARTICLES(particles, point_count, n, l, m);
   INIT_ORBITAL_PARTICLES_DIF(particles, point_count, n, l, m);

   double max_r = (double)(n * n + 5 * n);
   float scale = 1.0f / (float)max_r;
   for (int i = 0; i < point_count; i++) {
       float r = particles[i].r;
       float theta = particles[i].theta;
       float phi = particles[i].phi;

       float x = r * sinf(theta) * cosf(phi);
       float y = r * cosf(theta);
       float z = r * sinf(theta) * sinf(phi);

       vertices[i * 6 + 0] = x * scale;
       vertices[i * 6 + 1] = y * scale;
       vertices[i * 6 + 2] = z * scale;
       vertices[i * 6 + 3] = particles[i].r_col;
       vertices[i * 6 + 4] = particles[i].g_col;
       vertices[i * 6 + 5] = particles[i].b_col;
   }



   Mesh points = mesh_create(vertices, point_count);

   while (glfwWindowShouldClose(window) == 0) {
    glfwPollEvents();
    renderer_clear();

      if (need_geometry_update == 1 && type == 1) {
         double new_max_r = (double)(n * n + 5 * n);
         scale = 0.85f / (float)new_max_r;
         INIT_ORBITAL_PARTICLES_DIF(particles, point_count, n, l, m);
         need_geometry_update = 0;
         printf("Orbital changed\n");
      }

      if (need_geometry_update == 1 && type == 2) {
         double new_max_r = (double)(n * n + 5 * n);
         scale = 0.85f / (float)new_max_r;
         INIT_ORBITAL_PARTICLES(particles, point_count, n, l, m);
         need_geometry_update = 0;
         printf("Orbital changed\n");
      }

    for (int i = 0; i < point_count; i++) {

      particles[i].phi += particles[i].phi_speed;

      if (particles[i].phi > 2.0f * M_PI) {
         particles[i].phi -= 2.0f * M_PI;

      }
    

      float r = particles[i].r;
      float theta = particles[i].theta;
      float phi = particles[i].phi;

      float x = r * sinf(theta) * cosf(phi);
      float y = r * cosf(theta);
      float z = r * sinf(theta) * sinf(phi);

      vertices[i * 6] = x * scale;
      vertices[i * 6 + 1] = y * scale;
      vertices[i * 6 + 2] = z * scale;

      vertices[i * 6 + 3] = particles[i].r_col;
      vertices[i * 6 + 4] = particles[i].g_col;
      vertices[i * 6 + 5] = particles[i].b_col;
   }
   glUniform1f(glGetUniformLocation(shader, "alpha"), alpha);
   glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
   glBufferSubData(GL_ARRAY_BUFFER, 0, point_count * 6 * sizeof(float), vertices);
   glBindBuffer(GL_ARRAY_BUFFER, 0);


   shader_use(shader);

  // float time_angle = (float)glfwGetTime() * 0.5f;

   int ax_location = glGetUniformLocation(shader, "u_angleX");
   glUniform1f(ax_location, cam_rot_x);
   int ay_location = glGetUniformLocation(shader, "u_angleY");
   glUniform1f(ay_location, cam_rot_y);

   float aspect = 640.0f / 480.0f;
   int aspect_location = glGetUniformLocation(shader, "u_aspect");
   glUniform1f(aspect_location, aspect);

   mesh_draw(&points);
   glfwSwapBuffers(window);

   }
   

   free(particles);
   free(vertices);
   windowdestroy(window);

   return 0;

   /* how to compile = gcc main.c 
   window.c shader.c mesh.c renderer.c 
   glad/src/gl.c -Iglad/include -o main -L. 
   -lglfw3 -lopengl32 -lgdi32 */

}