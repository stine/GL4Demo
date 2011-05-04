#version 400 core

// Uniforms
uniform mat4 modelViewProjectionMatrix;

// Incoming Varyings
in vec3 position;
in vec4 color;

// Outgoing Varyings
out vec4 vColor;

void main() {
  // Simply pass along the color of this vertex.
  vColor = color;
  
  // Translate incoming position by modelview and projection matrix,
  //  and store in final gl_Position.
  gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
} 
