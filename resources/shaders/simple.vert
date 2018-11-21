#version 150

#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
//uniform int ShaderMode;

out vec3 pass_Normal;
out vec3 pass_VertPos;
out vec3 pass_eyePos;
//flat out int shader_Mode;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;

	//computes the position of the viewer - going from camera coordinates to model
	pass_eyePos = vec3(ViewMatrix*ModelMatrix*NormalMatrix*vec4(in_Position, 1.0)).xyz; 

	vec4 vertPos4 = ModelMatrix* vec4(in_Position, 1.0);
	pass_VertPos = vec3 (vertPos4/vertPos4.w);

	//shader_Mode = ShaderMode;
}
