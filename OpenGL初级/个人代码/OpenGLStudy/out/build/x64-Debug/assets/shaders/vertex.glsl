#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
uniform float time;
uniform float speed;
out vec3 color;
out vec2 uv;
void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//gl_Position = vec4(sin(time) * aPos.x, aPos.y, aPos.z, 1.0);
	//gl_Position = vec4(aPos.x + sin(time * speed) * 0.5f, aPos.y, aPos.z, 1.0);
	//color = aColor * (cos(time) + 1.0f) / 2.0f;
	color = aColor;
	uv = aUV;
}