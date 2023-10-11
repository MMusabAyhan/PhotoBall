#pragma once

#include <glm/ext/matrix_transform.hpp>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class VertexArray;
class OrthographicCamera;

class Renderer2D 
{
public:
	// Stats
	struct Statistics
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
		uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
	};

public:
	~Renderer2D();

	static void Init();
	static void Shutdown();

	static void BeginScene(const OrthographicCamera& camera);
	static void EndScene();
	static void Flush();

	static void Clear();
	static void DrawIndexed(VertexArray* va, unsigned int indexCount = 0);

	static void ResetStats();
	static Statistics GetStats();

	// Draw Calls
	static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
	static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
	static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

	static void DrawCircle(const glm::vec2& position, const float& radius, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
	static void DrawCircle(const glm::vec3& position, const float& radius, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
	static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);

private:
	static void StartBatch();
	static void NextBatch();

};