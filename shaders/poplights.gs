#version 400

layout(triangles, invocations = 1) in;
layout(points, max_vertices = 120) out;

// Uniforms
uniform mat4 modelViewProjectionMatrix;
uniform int seed;
uniform float secondsElapsed;
uniform float frequencyPerSqUnit;
uniform float minAltitude;
uniform float maxAltitude;
uniform float minRadius;
uniform float maxRadius;

// Incoming Varyings
in vec4 vColor[];
in vec3 vObjPos[];

// Outgoing Varyings
out vec3  gObjPos;
out float gMaxRadius;
out float gAgeSeconds;

// NOTE:  This shader uses an absolutely terrible noise generator.  It requires
//         the use of a seed uniform (provided by the CPU) and arbitrary input
//         values (such as hardcoded constants, vertex position, polygon normal,
//         etc.) to produce a single value in [0,1).
//        I will fix this in the near future - I'm still trying to figure out
//         how to generate an arbitrary number of random values on the GPU.
//         Maybe a few octaves of Perlin noise?
//        Also, we're not yet using transform feedback to make each point
//         persist.  Currently, each generated point pops into existance
//         and is immediately rendered.  The desired behavior is for points
//         to start with a 'radius' of 1.0, linearly increase to the point's
//         maxRadius, linearly decrease back to 1.0, and then expire.


// Really crappy noise generator; based on arbitrary xy input.
// Returns value in [0, 1).  Seeded by the "seed" integer uniform.
float rand(vec2 co) {
  return fract(sin(dot(co.xy + seed, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
  
  // Calculate the normal of the face, in object coordinates.
  vec3  triNormal = cross(vObjPos[1] - vObjPos[0], vObjPos[2] - vObjPos[0]);
  vec3  triNormalN = normalize(triNormal);
  float surfaceArea = length(triNormal);

  // Probabilistically calculate the number of particles to emit this frame,
  //  based on the amount of time elapsed, the frequency of particle emission
  //  per square 'unit' of a polygon, and the surface area of the polygon.
  float probability = clamp(secondsElapsed * frequencyPerSqUnit * surfaceArea,
			    0.0f,
			    120.0f);
  float randVal = rand(triNormalN.xy + vObjPos[2].zz - vObjPos[1].yx);
  int pointCount = int(probability) + int(randVal < fract(probability) ? 1 : 0);

  // Calculate the attributes of the n particles we're emitting.
  for(int i = 0; i < pointCount; ++i) {

    // Calculate a random altitude.
    float altitude = rand(vec2(triNormalN.x + 12.6987f,  i)) *
      (maxAltitude - minAltitude) + minAltitude;

    // Calculate a random position in barycentric coords.
    // Use this to calculate the particle's object coord position.
    float u = rand(triNormalN.xy + vec2(0.254 * i, -0.6548f));
    float v = (1-u) * rand(triNormalN.yz + vec2(10.741f, -i));
    float w = (1-u-v);
    gObjPos = (u * vObjPos[0] + v * vObjPos[1] + w * vObjPos[2]) + 
      (altitude * triNormalN);

    // Calculate a random "maxRadius" for the particle
    gMaxRadius = rand(vec2(triNormalN.x + 1.21584f,  i)) *
      (maxRadius - minRadius) + minRadius;

    // Initialize the particle's age.
    gAgeSeconds = 0.0f;

    // TODO: For testing, rasterize immediately.
    //       Instead of rasterization, we should be passing the
    //        particles to transform feedback.
    gl_Position = modelViewProjectionMatrix * vec4(gObjPos, 1.0);
    gl_PointSize = gMaxRadius;

    EmitVertex();
  }
}
