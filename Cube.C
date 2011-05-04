#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <cmath>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_operation.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/inverse_transpose.hpp>

#include "config.h"
#include "Cube.h"
#include "Shader.h"

Cube::Cube()
  : _vao(0),
    _indicesVbo(0),
    _vertsVbo(0),
    _colorsVbo(0),
    _programHandle(0),
    _modelViewMatrix(1.0f),
    _projectionMatrix(1.0f)
{
  // Initialize OpenGL state
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClearDepth(1.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  // Generate Cube geometry and store in _indices, _position3, and _color4.
  static const int position[] =
    { -1.0f, -1.0f,  1.0f,
       1.0f, -1.0f,  1.0f,
       1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
       1.0f, -1.0f, -1.0f,
       1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f
    };
  static const float color[] =
    { 1.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 1.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 1.0f
    };
  static const int index[] =
    { 0, 1, 2,
      0, 2, 3,
      1, 5, 6,
      1, 6, 2,
      5, 4, 7,
      5, 7, 6,
      4, 0, 3,
      4, 3, 7,
      3, 2, 6,
      3, 6, 7,
      1, 0, 4,
      1, 4, 5
    };
  _verts3.assign(position, position + sizeof(position) / sizeof(position[0]));
  _colors4.assign(color, color + sizeof(color) / sizeof(color[0]));
  _indices.assign(index, index + sizeof(index) / sizeof(index[0]));

  // Load Shaders and use the loaded program.
  const char *vS = "shaders/cube.vs";
  const char *fS = "shaders/cube.fs";
  _programHandle = shader::loadEffect(vS, NULL, NULL, NULL, fS);
  glUseProgram(_programHandle);

  // Bind the shader Varying positions.
#define POSITION_INDEX   0
#define COLOR_INDEX      1
#define FRAGCOLOR_NUMBER 0
  glBindAttribLocation(_programHandle, POSITION_INDEX, "position");
  glBindAttribLocation(_programHandle, COLOR_INDEX, "color");
  glBindFragDataLocation(_programHandle, FRAGCOLOR_NUMBER, "fragcolor");

  // Bind a vertex array object.
  // A VAO holds the state of all VBOs associated with it.
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  
  // Create a VBO to hold per-vertex position data.  Point the "position" varying
  //  to this VBO.  (Note that _vao is still bound, so this VBO will be associated
  //  with the enabled VAO)
  glGenBuffers(1, &_vertsVbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vertsVbo);
  glBufferData(GL_ARRAY_BUFFER, _verts3.size() * sizeof(GLfloat), &_verts3[0], GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(_programHandle, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(_programHandle, "position"));

  // Create a VBO to hold per-vertex color data.  Point the "color" varying
  //  to this VBO.  (Note that _vao is still bound, so this VBO will be associated
  //  with the enabled VAO)
  glGenBuffers(1, &_colorsVbo);
  glBindBuffer(GL_ARRAY_BUFFER, _colorsVbo);
  glBufferData(GL_ARRAY_BUFFER, _colors4.size() * sizeof(GLfloat), &_colors4[0], GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(_programHandle, "color"), 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(_programHandle, "color"));

  // Create a VBO to hold vertex index data. This VBO isn't attached to a varying, since it simply
  // provides an index into the other VBOs at rendering time.  See glDrawElements().  (Note that
  // _vao is still bound, so this VBO will be associated with the enabled VAO)
  glGenBuffers(1, &_indicesVbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesVbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), &_indices[0], GL_STATIC_DRAW);

  // Now that we're done uploading data to the OpenGL server, unbind the VAO and VBO.
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Populate the default modelview and projection matrices.
  _modelViewMatrix  = glm::mat4(1.0f);
  _projectionMatrix = glm::mat4(1.0f) * glm::perspective
    (25.0f, (float)RENDER_WIDTH /(float)RENDER_HEIGHT, 1.0f, 65.0f);

  return;
}

Cube::~Cube()
{
  // Delete all buffers and vao.
  glDeleteBuffers(1, &_vertsVbo);
  glDeleteBuffers(1, &_colorsVbo);
  glDeleteVertexArrays(1, &_vao);

  return;
}

void Cube::render(float secondsElapsed)
{
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // rotate cube
  _modelViewMatrix = glm::translate(_modelViewMatrix, glm::vec3(0.0, 0.0, -10.0));
  _modelViewMatrix = glm::rotate(_modelViewMatrix, secondsElapsed, glm::vec3(0.0f, 1.0f, 0.0f));
  _modelViewMatrix = glm::rotate(_modelViewMatrix, secondsElapsed/3, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _modelViewMatrix;
  _modelViewMatrix = glm::translate(_modelViewMatrix, glm::vec3(0.0, 0.0, 10.0));
  glUniformMatrix4fv(glGetUniformLocation(_programHandle, "modelViewProjectionMatrix"),
		     1, false, &modelViewProjectionMatrix[0][0]);
  
  // draw cube
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL);
}
