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

  //1 ��ȡ��ǰ���ض�Ӧ�������ϵ����ؾ���λ��
  vec2 location = uv * vec2(width, height);

  //2 ���㵱ǰ���ض�Ӧ���ؾ���λ����xy�����ϵı仯��
  vec2 dx = dFdx(location);
  vec2 dy = dFdy(location);

  //3 ѡ������delta����log2��delta��
  float maxDelta = sqrt(max(dot(dx, dx), dot(dy,dy)));
  float L = log2(maxDelta);

  //4 �����mipmap�Ĳ�������
  int level = max(int(L + 0.5), 0);

  FragColor = textureLod(sampler, uv, level);
}