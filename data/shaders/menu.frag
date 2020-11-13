#version 330

uniform sampler2D u_texture_diffuse;
in vec2 v_uv;
out vec4 fragColor;

void main(void)
{
	fragColor =  vec4(texture2D(u_texture_diffuse, v_uv).xyz, 1.0);
}