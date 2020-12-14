//
//  main.cpp
//  3dGLFW
//
//  Created by Syritx on 2020-12-04.
//

#include "libs.h"
#include "Plain.h"

#include <iostream>
using namespace std;

void keyboard(GLFWwindow* window, Plain plain);

vec3 position = vec3(0,0,0),
     eye = vec3(0,0,-1),
     up = vec3(0,1,0);
float xrot, yrot;

int main(int argc, const char * argv[]) {
    
    if (!glfwInit()) glfwTerminate();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(1000, 720, "hello", NULL, NULL);
    if (!window) glfwTerminate();
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    
    Plain plain = Plain();
        
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);
        
        keyboard(window, plain);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 1;
}


float clamp(float v, float min, float max) {
    if (v > max) return max;
    if (v < min) return min;
    
    return v;
}

void rotate(float direction) {
    yrot += direction*2;
    xrot = clamp(xrot, -89.9f, 89.9f);
    cout << direction << "\n";
    
    eye.x = cos(radians(xrot)) * cos(radians(yrot));
    eye.y = sin(radians(xrot));
    eye.z = cos(radians(xrot)) * sin(radians(yrot));
    
    eye = normalize(eye);
}

void keyboard(GLFWwindow* window, Plain plain) {
    
    float xDir = 0;
    // vertical
         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += eye * .1f;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= eye * .1f;
    
    // horizontal
         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) xDir = -1.f;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) xDir =  1.f;
    rotate(xDir);
    
    plain.render(position, eye, up);
}
