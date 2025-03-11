#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;

uniform sampler2D sampler;

void main()
{
	vec2 location  = uv * (200.0, 250.0);
	vec2 dx = dFdx(location);
	vec2 dy = dFdy(location);

	float maxDelta = sqrt(max(dot(dx,dx), dot(dy, dy)));
	float L = log2(maxDelta);
	int level = max(int(L + 0.5), 0);

   FragColor = textureLod(sampler, uv, level);
  // FragColor = vec4(1.0);
}