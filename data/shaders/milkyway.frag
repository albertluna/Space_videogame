#version 330

in vec3 v_normal;
in vec3 v_camera;
in vec2 v_uv;

uniform vec3 u_light;
uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{
	vec3 final_color = vec3(0.1, 0.1, 0.1);

	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light);
	vec3 E = normalize(v_camera);

	float NdotL = dot(N, L);
	if (NdotL > 0.0){
	

		vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz * NdotL;

		final_color += diffuse;

	}

	vec3 R = reflect(-L, N);
	float RdotE = dot(R, E);
	if (RdotE > 0.0) {
		vec3 specular = vec3(1.0, 1.0, 1.0) * pow(max(RdotE,0.0), 30.0); 
		final_color += specular;
	}

	// simple red colour
	fragColor =  vec4(texture2D(u_texture_diffuse, v_uv).xyz, 1.0);
}