#pragma once

class UniformBuffer 
{
public:

	UniformBuffer(unsigned int size, unsigned int binding);
	~UniformBuffer();

	void SetData(const void* data, unsigned int size, unsigned int offset = 0);
private:

	unsigned int m_RendererID;

};
