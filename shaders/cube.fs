#version 400 core

// Incoming Varyings
in float vWorldZ;
in vec4 vColor;

// Outgoing (final) Varyings
out vec4 fragcolor;

// Returns the appropriate value from the Jet color function.
vec3 getJetColor(float value) {
  float fourValue = 4 * value;
  float red   = min(fourValue - 1.5, -fourValue + 4.5);
  float green = min(fourValue - 0.5, -fourValue + 3.5);
  float blue  = min(fourValue + 0.5, -fourValue + 2.5);
 
  return clamp( vec3(red, green, blue), 0.0, 1.0 );
}

void main() {
  fragcolor = vec4(getJetColor(1 - (-vWorldZ - 9.0)/2), 1.0);
}

