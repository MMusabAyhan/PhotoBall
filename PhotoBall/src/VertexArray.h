#pragma once

#include "Buffer.h"
#include "VertexBufferLayout.h"

class VertexArray 
{
public:

	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void AddBuffer(VertexBuffer* vb, const VertexBufferLayout* layout);
	void SetIndexBuffer(IndexBuffer* ib);

	inline std::vector<VertexBuffer*>& GetVertexBuffers() { return m_VertexBuffers; }
	inline IndexBuffer* GetIndexBuffer() { return m_IndexBuffer; };

private:

	unsigned int m_RendererID;
	unsigned int m_VertexBufferIndex;\
	std::vector<VertexBuffer*> m_VertexBuffers;
	IndexBuffer* m_IndexBuffer;
};