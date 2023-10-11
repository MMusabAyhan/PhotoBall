#pragma once

#include "physics.hpp"
#include "glm/glm.hpp"

struct PhysicSolver;

class PhysicsWorld
{
public:
	PhysicsWorld(float gridX, float gridY, std::string circleCoordsFilePath);
	~PhysicsWorld();

	void OnUpdate(float deltaTimeSecs);
	void AddObject(glm::vec2 pos, float initialSpeed, glm::vec2 direction, glm::vec4 color = {1,1,1,1});

	inline auto GetPhysicsObjects() const { return m_Solver.objects; };
	inline float GetWorldWidth() const { return m_WorldWidth; };
	inline float GetWorldHeight() const { return m_WorldHeight; };
	inline const std::vector<glm::vec2>& GetFinalObjectCoords() const { return m_FinalObjectCoords; };

private:

	PhysicSolver m_Solver;
	float m_WorldWidth;
	float m_WorldHeight;

	std::vector<glm::vec2> m_FinalObjectCoords;
};
