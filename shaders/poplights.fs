#version 400

// Incoming Varyings
in vec4 gColor;

// Outgoing (final) Varyings
out vec4 fragcolor;

void main() {
  fragcolor = gColor;
}

