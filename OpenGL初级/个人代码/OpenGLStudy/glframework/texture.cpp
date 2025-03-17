#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "../application/stb_image.h"


Texture::Texture(const std::string& path, unsigned int unit) {
	//加载图片到显存并且需要绑定到纹理单元上
	mUnit = unit;
	//1 stbImage 读取图片
	int channels;
	//--翻转y轴
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);//读取出来全部转化为RGBA格式
	//2 生成纹理并且激活单元绑定
	glGenTextures(1, &mTexture);
	//--激活纹理单元--
	glActiveTexture(GL_TEXTURE0 + mUnit);
	//--绑定纹理对象--
	glBindTexture(GL_TEXTURE_2D, mTexture);//这里就自动与上面激活的0号纹理单元链接
	//3 传输纹理数据 开辟显存 从CPU到GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//***释放数据
	stbi_image_free(data);

	//4 设置纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 需要的像素>图片像素 用双线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// 需要的像素<图片像素 用临近过滤

	//5 设置纹理的包裹方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//v

	//典型BUG 这里如果这么调用的话会显示错误
	//已经这个时候纹理单元还是激活状态,会直接设置当前纹理为0,相当于清空了之前设置的状态
	//纹理单元激活之后是没法关掉的! 只有激活其他纹理单元
	//glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {
	if (mTexture != 0)
	{
		glDeleteTextures(1, &mTexture);
	}
}
 
void Texture::bind() {
	//先切换纹理单元,然后绑定texture对象
	glActiveTexture(GL_TEXTURE0 + mUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}