#version 400

// Uniforms
uniform mat4 modelViewProjectionMatrix;

// Incoming Varyings
in vec3 position;
in vec4 color;

// Outgoing Varyings
out vec3 vObjPos;
out vec4 vColor;

void main() {
  // Simply pass along the color and object position of this vertex.
  vColor  = color;
  vObjPos = position;
}