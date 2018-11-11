#version 150

in vec3 pass_Normal;
in vec3 pass_Color;

//is defined by glUniform3f
//uniform vec3 planetColor;

out vec4 out_Color;

void main() {
  //out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4(pass_Color, 1.0);
}
