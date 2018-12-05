#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO

layout(location = 0) in vec4 in_Position;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
out vec3 eyeDirection;

void main() {
  
  mat4 inverseProjection = inverse(ProjectionMatrix);
  mat3 inverseModelview  = transpose(mat3(ViewMatrix));
  vec3 unprojected       = (inverseProjection * in_Position).xyz;
  eyeDirection           = inverseModelview * unprojected;
  gl_Position            = in_Position;
}