#include "Renderer2D.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include "UniformBuffer.h"
#include "OrthographicCamera.h"

#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"

struct CircleVertex
{
	glm::vec3 worldPosition;
	glm::vec3 localPosition;
	glm::vec4 color;
	float thickness;
	float fade;
	int entityID;
};

struct QuadVertex
{
	glm::vec3 position;
	glm::vec4 color;

	int entityID;
};

struct Renderer2DData
{
	static const unsigned int MaxQuads = 100000;
	static const unsigned int MaxVertices = MaxQuads * 4;
	static const unsigned int MaxIndices = MaxQuads * 6;

	VertexArray* QuadVertexArray;
	VertexBuffer* QuadVertexBuffer;
	VertexBufferLayout* QuadVertexBufferLayout;
	IndexBuffer* quadIb;
	Shader* QuadShader;

	unsigned int QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	VertexArray* CircleVertexArray;
	VertexBuffer* CircleVertexBuffer;
	VertexBufferLayout* CircleVertexBufferLayout;
	Shader* CircleShader;

	unsigned int CircleIndexCount = 0;
	CircleVertex* CircleVertexBufferBase = nullptr;
	CircleVertex* CircleVertexBufferPtr = nullptr;

	glm::vec4 QuadVertexPositions[4];

	Renderer2D::Statistics Stats;

	struct CameraData
	{
		glm::mat4 ViewProjection;
	};

	CameraData CameraBuffer;
	UniformBuffer* CameraUniformBuffer;
};

static Renderer2DData s_Data;

Renderer2D::~Renderer2D()
{
	Renderer2D::Shutdown();
}

void Renderer2D::Init()
{			
	s_Data.QuadVertexArray = new VertexArray();
	s_Data.QuadVertexBuffer = new VertexBuffer(s_Data.MaxVertices * sizeof(QuadVertex));
	s_Data.QuadVertexBufferLayout = new VertexBufferLayout();

	s_Data.QuadVertexBufferLayout->Push<float>(3);
	s_Data.QuadVertexBufferLayout->Push<float>(4);
	s_Data.QuadVertexBufferLayout->Push<int>(1);

	s_Data.QuadVertexArray->Bind();
	s_Data.QuadVertexArray->AddBuffer(s_Data.QuadVertexBuffer, s_Data.QuadVertexBufferLayout);

	s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

	unsigned int* quadIndices = new unsigned int[s_Data.MaxIndices];

	unsigned int offset = 0;
	for (int i = 0; i < s_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 0;
		quadIndices[i + 4] = offset + 2;
		quadIndices[i + 5] = offset + 3;

		offset += 4;
	}

	s_Data.quadIb = new IndexBuffer(quadIndices, s_Data.MaxIndices);
	s_Data.QuadVertexArray->SetIndexBuffer(s_Data.quadIb);
	delete[] quadIndices;

	s_Data.CircleVertexArray = new VertexArray();
	s_Data.CircleVertexBuffer = new VertexBuffer(s_Data.MaxVertices * sizeof(CircleVertex));
	s_Data.CircleVertexBufferLayout = new VertexBufferLayout();

	s_Data.CircleVertexBufferLayout->Push<float>(3);
	s_Data.CircleVertexBufferLayout->Push<float>(3);
	s_Data.CircleVertexBufferLayout->Push<float>(4);
	s_Data.CircleVertexBufferLayout->Push<float>(1);
	s_Data.CircleVertexBufferLayout->Push<float>(1);
	s_Data.CircleVertexBufferLayout->Push<int>(1);

	s_Data.CircleVertexArray->Bind();
	s_Data.CircleVertexArray->AddBuffer(s_Data.CircleVertexBuffer, s_Data.CircleVertexBufferLayout);
	s_Data.CircleVertexArray->SetIndexBuffer(s_Data.quadIb);

	s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];
	
	s_Data.QuadShader = new Shader("resources/shaders/quad.shader");
	s_Data.CircleShader = new Shader("resources/shaders/circle.shader");

	s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	s_Data.CameraUniformBuffer = new UniformBuffer(sizeof(Renderer2DData::CameraData), 0);
}		

void Renderer2D::Shutdown()
{
	delete[] s_Data.QuadVertexBufferBase;
	delete[] s_Data.CircleVertexBufferBase;

	delete s_Data.QuadVertexArray;
	delete s_Data.QuadVertexBuffer;
	delete s_Data.QuadVertexBufferLayout;
	delete s_Data.quadIb;
	delete s_Data.QuadShader;

	delete s_Data.CircleVertexArray;
	delete s_Data.CircleVertexBuffer;
	delete s_Data.CircleVertexBufferLayout;
	delete s_Data.CircleShader;

	delete s_Data.CameraUniformBuffer;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) 
{
	s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
	s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

	StartBatch();
}

void Renderer2D::EndScene() 
{
	Flush();
}

void Renderer2D::Flush() 
{
	if (s_Data.QuadIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		s_Data.QuadShader->Bind();
		DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}
	if (s_Data.CircleIndexCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
		s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

		s_Data.CircleShader->Bind();
		DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
		s_Data.Stats.DrawCalls++;
	}

}

void Renderer2D::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::ResetStats() 
{
	memset(&s_Data.Stats, 0, sizeof(Statistics));
}

Renderer2D::Statistics Renderer2D::GetStats() 
{
	return s_Data.Stats;
}

void Renderer2D::DrawIndexed(VertexArray* va, unsigned int indexCount)
{
	va->Bind();

	unsigned int count = indexCount;
	if (!count && !va->GetIndexBuffer()) 
	{
		std::cout << "Warning : An Index Buffer is found NULL when trying to\n" << 
					 "draw Indices from va->GetIndexBuffer()->GetCount()"		<< std::endl;
		return;
	}

	if (!count)
		count = va->GetIndexBuffer()->GetCount();

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

//////////////     ///////////////
/////////                 ////////
// ------  Draw Calls ------ //
/////////                 ////////
//////////////     ///////////////

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityID)
{
	DrawQuad({ position.x, position.y, 0.0f }, size, color, entityID);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
{

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, color,entityID);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
{
	constexpr size_t quadVertexCount = 4;

	if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		NextBatch();

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		s_Data.QuadVertexBufferPtr->position = transform * s_Data.QuadVertexPositions[i];
		s_Data.QuadVertexBufferPtr->color = color;
		s_Data.QuadVertexBufferPtr->entityID = entityID;
		s_Data.QuadVertexBufferPtr++;
	}

	s_Data.QuadIndexCount += 6;
	s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawCircle(const glm::vec2& position, const float& radius, const glm::vec4& color, float thickness, float fade, int entityID) 
{
	DrawCircle(glm::vec3(position.x, position.y, 0), radius, color, thickness, fade, entityID);
}

void Renderer2D::DrawCircle(const glm::vec3& position, const float& radius, const glm::vec4& color, float thickness, float fade, int entityID) 
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { 2 * radius, 2 * radius, 1.0f });

	DrawCircle(transform, color, thickness, fade, entityID);
}

void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID)
{
	for (size_t i = 0; i < 4; i++)
	{
		s_Data.CircleVertexBufferPtr->worldPosition = transform * s_Data.QuadVertexPositions[i];
		s_Data.CircleVertexBufferPtr->localPosition = s_Data.QuadVertexPositions[i] * 2.0f;
		s_Data.CircleVertexBufferPtr->color = color;
		s_Data.CircleVertexBufferPtr->thickness = thickness;
		s_Data.CircleVertexBufferPtr->fade = fade;
		s_Data.CircleVertexBufferPtr->entityID = entityID;
		s_Data.CircleVertexBufferPtr++;
	}

	s_Data.CircleIndexCount += 6;

	s_Data.Stats.QuadCount++;
}

void Renderer2D::StartBatch() 
{
	s_Data.QuadIndexCount = 0;
	s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	s_Data.CircleIndexCount = 0;
	s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
}

void Renderer2D::NextBatch() 
{
	Flush();
	StartBatch();
}