#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) 
	{
		switch (type) 
		{
			case GL_FLOAT:				return sizeof(GLfloat);
			case GL_INT:				return sizeof(GLint);
			case GL_UNSIGNED_INT:		return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:		return sizeof(GLubyte);
		}

		return 0;
	}
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() 
		:m_Stride(0) {};


	template<typename T>
	void Push(int count)
	{
		//static_assert(false);
	}

	template<>
	void Push<float>(int count)
	{
		m_Elements.push_back({ GL_FLOAT,(unsigned int) count,GL_FALSE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void Push<int>(int count)
	{
		m_Elements.push_back({ GL_INT,(unsigned int) count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
	}
	template<>
	void Push<unsigned int>(int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,(unsigned int) count,GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,(unsigned int) count,GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return m_Stride; };
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; };
private:

	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};
