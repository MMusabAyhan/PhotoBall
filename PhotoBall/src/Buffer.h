#pragma once

class VertexBuffer
{
public:

	VertexBuffer(unsigned int size);
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void SetData(const void* data, unsigned int size);

private:

	unsigned int m_RendererID;
};


class IndexBuffer
{
public:

	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:

	unsigned int m_RendererID;
	unsigned int m_Count;
};