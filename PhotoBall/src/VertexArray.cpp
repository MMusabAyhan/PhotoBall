#include "VertexArray.h"
#include "Renderer2D.h"

VertexArray::VertexArray() : m_VertexBufferIndex(0), m_IndexBuffer(nullptr)
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() 
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const 
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const 
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(VertexBuffer* vb, const VertexBufferLayout* layout)
{
	Bind();
	vb->Bind();

	const auto& elements = layout->GetElements();
	unsigned int offset{0};
	for (unsigned int i=0; i < elements.size();i++) 
	{
		const auto& element = elements[i];

		glEnableVertexAttribArray(m_VertexBufferIndex);
		glVertexAttribPointer(m_VertexBufferIndex, element.count, element.type, element.normalized, layout->GetStride(), (const void*) offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		m_VertexBufferIndex++;
	}

	m_VertexBuffers.push_back(vb);

}

void VertexArray::SetIndexBuffer(IndexBuffer* ib)
{
	Bind();
	ib->Bind();
	m_IndexBuffer = ib;
}