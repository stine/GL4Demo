#version 400

layout(triangles, invocations = 1) in;
layout(line_strip, max_vertices = 2) out;

// Uniforms
uniform mat4 modelViewProjectionMatrix;

// Incoming Varyings
in vec4 vColor[];
in vec3 vObjPos[];

// Outgoing Varyings
out vec4 gColor;

// NOTE: doesn't properly use a normalMatrix

void main() {
  
  // Calculate the centerpoint of the current triangle face, in world coords.
  vec3 triCenter = (vObjPos[0] + vObjPos[1] + vObjPos[2]) / 3;

  // Calculate the normal of the face, in world coords.
  vec3 triNormal = normalize( cross(vObjPos[1] - vObjPos[0], vObjPos[2] - vObjPos[0]) );

  // Set vertex attributes and emit the vertex.
  gColor = vec4(0.0, 1.0, 0.0, 0.0);
  gl_Position = modelViewProjectionMatrix * vec4(triCenter, 1.0);
  EmitVertex();

  gColor = vec4(1.0, 1.0, 1.0, 1.0);
  gl_Position = modelViewProjectionMatrix * vec4(triCenter + (triNormal * 1.0), 1.0);
  EmitVertex();
}
