#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
uniform float time;
uniform vec3 uColor;
uniform sampler2D sampler;

void main()
{
	//float intensity = (sin(time) + 1.0f) / 2.0f;
	//FragColor = vec4(vec3(intensity) + color, 1.0f);
	//FragColor = vec4(color, 1.0f);
	FragColor = texture(sampler, uv);
}