#version 150

in vec2 texture_Coordinates;
// flat in int shader_Mode;

uniform sampler2D FramebufferTex;

uniform bool HorizontalMirroringMode;
uniform bool GreyScaleMode;
uniform bool VerticalMirroringMode;
uniform bool BlurMode;


in vec4 gl_FragCoord;

out vec4 out_Color;




void main() {

  float tex_x = texture_Coordinates.x;
  float tex_y = texture_Coordinates.y;

  out_Color = texture(FramebufferTex, texture_Coordinates);

  

  if (HorizontalMirroringMode) {
		tex_y = 1 - tex_y;
		out_Color = texture(FramebufferTex, vec2(tex_x, tex_y));
  }


  if (VerticalMirroringMode) {
    tex_x = 1 - tex_x;
		out_Color = texture(FramebufferTex, vec2(tex_x, tex_y));
  }


  if (BlurMode) {
    float temp_x = 1.0 / 1024;
    float temp_y = 1.0 / 768;
    vec4 sum = vec4(0.0);
    vec2 temp = vec2(0.0);

    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        temp = vec2(tex_x + x * temp_x, tex_y + y * temp_y);
        if(x == 1 && y == 1) {
          sum += texture(FramebufferTex, temp) * 	0.195346;
        } else if(mod(x, 2) == 0 || mod(y, 2) == 0) {
          sum += texture(FramebufferTex, temp) * 	0.123317;
        } else {
          sum += texture(FramebufferTex, temp) * 	0.077847;
        }
      }
    }
    out_Color = sum;
  }


  if(GreyScaleMode) {
    float avg = 0.2126 * out_Color.r + 0.7152 * out_Color.g + 0.0722 * out_Color.b;
    out_Color = vec4(avg, avg, avg, 1.0);
  }
}