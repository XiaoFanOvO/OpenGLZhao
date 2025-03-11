#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;

uniform sampler2D sampler;
uniform float time;

void main()
{
	float deltaU = time * 0.3;
	vec2 fraguv = vec2(uv.x + deltaU, uv.y);

	FragColor = texture(sampler, fraguv);
}