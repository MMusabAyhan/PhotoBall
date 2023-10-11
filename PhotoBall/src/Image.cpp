#include "Image.hpp"
#include "stb_image.h"


Image::Image(std::string filePath)
	: m_FilePath(filePath), m_Width(0), m_Height(0), m_ChannelCount(0), m_Data(nullptr)
{
	stbi_set_flip_vertically_on_load(1);
	m_Data = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_ChannelCount, 4);
}

Image::~Image()
{
	stbi_image_free(m_Data);
}