#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;

uniform sampler2D sampler;

uniform float width;
uniform float height;

void main()
{
  //FragColor = texture(sampler, uv);

  //1 获取当前像素对应的纹理上的纹素具体位置
  vec2 location = uv * vec2(width, height);

  //2 计算当前像素对应纹素具体位置在xy方向上的变化量
  vec2 dx = dFdx(location);
  vec2 dy = dFdy(location);

  //3 选择最大的delta，求log2（delta）
  float maxDelta = sqrt(max(dot(dx, dx), dot(dy,dy)));
  float L = log2(maxDelta);

  //4 计算出mipmap的采样级别
  int level = max(int(L + 0.5), 0);

  FragColor = textureLod(sampler, uv, level);
}