//
//  Plain.h
//  3dGLFW
//
//  Created by Syritx on 2020-12-04.
//

#ifndef Plain_h
#define Plain_h

#include "libs.h"
#include "Shader.h"

#include <iostream>
#include <glm/gtc/noise.hpp>
using namespace std;

const char* vertexShaderSource =
"#version 100 \n"
"precision mediump float;"
"varying vec3 fragColor;"
"attribute vec3 vertexPosition;"
"attribute vec3 vertexColor;"
"uniform mat4 mWorld;"
"uniform mat4 mView;"
"uniform mat4 mProj;"
"\n"
"void main() {"
"    fragColor = vertexColor;"
"    gl_Position = mProj * mView * mWorld * vec4(vertexPosition, 1.0);"
"}";

const char* fragmentShaderSource =
"#version 100\n"
"precision mediump float;"
"varying vec3 fragColor;"
"\n"
"void main() {"
"    gl_FragColor = vec4(fragColor, 1.0);"
"}";

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

float size = 10.5f;
float vertices[] = {
    -1.5f*size, -1.5f,  1.5f*size,  1.f, 0.f, 0.f,
     1.5f*size, -1.5f,  1.5f*size,  1.f, 1.f, 0.f,
     1.5f*size, -1.5f, -1.5f*size,  1.f, 1.f, 1.f,
    -1.5f*size, -1.5f, -1.5f*size,  1.f, 0.f, 1.f,
};

unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
};
class Plain {
private:
    GLuint ibo, vbo, vao;
public:
    Shader shader = Shader(vertexShaderSource, fragmentShaderSource);
    
    Plain() {
        
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*sizeof(float), vertices, GL_STATIC_DRAW);
        
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), indices, GL_STATIC_DRAW);
        
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        
        int positionAttrib = glGetAttribLocation(shader.program, "vertexPosition");
        int colorAttrib = glGetAttribLocation(shader.program, "vertexColor");
        
        glVertexAttribPointer(
        positionAttrib,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float), nullptr);
        
        glVertexAttribPointer(
        colorAttrib,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        BUFFER_OFFSET(3 * sizeof(float)));
        
        glEnableVertexAttribArray(positionAttrib);
        glEnableVertexAttribArray(colorAttrib);
        
        shader.use_shader();
        
        int worldUniformLocation = glGetUniformLocation(shader.program, "mWorld"),
            viewUniformLocation = glGetUniformLocation(shader.program, "mView"),
            projectionUniformLocation = glGetUniformLocation(shader.program, "mProj");
        
        mat4 worldMatrix = mat4(1.0f);
        mat4 viewMatrix = lookAt(vec3(0,0,0), vec3(0,0,0), vec3(0,1,0));
        mat4 projection = perspective(radians(90.f), 1000.f/720.f, .1f, 2000.f);
        
        shader.set_mat4(worldUniformLocation, worldMatrix);
        shader.set_mat4(viewUniformLocation, viewMatrix);
        shader.set_mat4(projectionUniformLocation, projection);
    }
    
    void render(vec3 position, vec3 eye, vec3 up) {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.use_shader();
        
        int worldUniformLocation = glGetUniformLocation(shader.program, "mWorld"),
            viewUniformLocation = glGetUniformLocation(shader.program, "mView"),
            projectionUniformLocation = glGetUniformLocation(shader.program, "mProj");
        
        mat4 worldMatrix = mat4(1.0f);
        mat4 viewMatrix = lookAt(position, position+eye, up);
        mat4 projection = perspective(radians(90.f), 1000.f/720.f, .1f, 2000.f);
        
        shader.set_mat4(worldUniformLocation, worldMatrix);
        shader.set_mat4(viewUniformLocation, viewMatrix);
        shader.set_mat4(projectionUniformLocation, projection);
        
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};

#endif /* Plain_h */
