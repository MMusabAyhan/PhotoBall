#include "UniformBuffer.h"
#include <glad/glad.h>

UniformBuffer::UniformBuffer(unsigned int size, unsigned int binding) 
{
	glCreateBuffers(1, &m_RendererID);
	glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
}

UniformBuffer::~UniformBuffer() 
{
	glDeleteBuffers(1, &m_RendererID);
}

void UniformBuffer::SetData(const void* data, unsigned int size, unsigned int offset) 
{
	glNamedBufferSubData(m_RendererID, offset, size, data);
}