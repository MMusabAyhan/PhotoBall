#include "PhysicsWorld.hpp"
#include "physics.hpp"
#include <fstream>
#include <iostream>

PhysicsWorld::PhysicsWorld(float gridX, float gridY, std::string circleCoordsFilePath)
	: m_Solver({ gridX,gridY}) , m_WorldWidth(gridX) , m_WorldHeight(gridY)
{
	std::string line;
	std::ifstream myFile;

	myFile.open(circleCoordsFilePath);
	if (!myFile.is_open())
		std::cout << "Circle Coords could not be loaded!" << "\n";
	else 
	{
		while (getline(myFile, line)) 
		{
			int posSpace = line.find(" ");
			float x = std::stof(line.substr(0, posSpace));
			float y = std::stof(line.substr(posSpace + 1));

			m_FinalObjectCoords.emplace_back(x, y);
		}
	}
}

PhysicsWorld::~PhysicsWorld()
{
	m_FinalObjectCoords.clear();
}

void PhysicsWorld::OnUpdate(float deltaTimeSecs)
{
	m_Solver.update(deltaTimeSecs);
}

void PhysicsWorld::AddObject(glm::vec2 pos, float initialSpeed, glm::vec2 direction, glm::vec4 color)
{
	const auto id = m_Solver.createObject(pos);

	m_Solver.objects[id].last_position.x -= initialSpeed * direction.x;
	m_Solver.objects[id].last_position.y -= initialSpeed * direction.y;
	m_Solver.objects[id].color = color;
}