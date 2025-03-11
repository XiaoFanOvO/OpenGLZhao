#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform float time;

void main()
{
	//1 当前三角形的顶点，缩放的比例
	float scale = 1.0 / time;
	
	//2 使用scale对顶点位置进行缩放
	vec3 sPos = aPos * scale;

	//3 向后传输位置信息
	gl_Position = vec4(sPos, 1.0);
	color = aColor;
	uv = aUV;
}