#ifndef __CUBE_H__
#define __CUBE_H__

#include <vector>
#include <glm/glm.hpp>

class Cube {
 public:
  Cube();
  ~Cube();
  void render(float secondsElapsed);
 private:

  // OpenGL handles for uploaded model data
  unsigned int _vao;
  unsigned int _indicesVbo;
  unsigned int _vertsVbo;
  unsigned int _colorsVbo;

  // OpenGL shader handle
  int _programHandle;

  // model data
  std::vector<unsigned> _indices;
  std::vector<float>    _verts3;
  std::vector<float>    _colors4;

  // view data
  glm::mat4 _modelViewMatrix;
  glm::mat4 _projectionMatrix;
};

#endif //__CUBE_H__
