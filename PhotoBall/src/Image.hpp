#pragma once
#include <string>

class Image
{
public:
 
	Image(std::string filePath);
	~Image();

	inline unsigned char* GetData() const { return m_Data; };
	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };

private:

	unsigned char* m_Data;

	int m_Width;
	int m_Height;
	int m_ChannelCount;

	std::string m_FilePath;

};

