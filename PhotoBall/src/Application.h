#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <memory>
#include <functional>
#include "Image.hpp"
#include "SimulationType.h"

class Window;
class imguiWindow;
class OrthographicCameraController;
class Renderer2D;
class PhysicsWorld;
class PhysicObject;

class Application 
{
public:
	Application(const std::string& name = "2D Pyhsics Engine", unsigned int width = 800, unsigned int height = 800, const std::string& filePath = "resources/Images/SiskoMusab.jpg");
	~Application() = default;

	void Run();
	void OnEvent(class Event& e);
	bool OnWindowClose(class WindowCloseEvent& e);

	inline Window* GetWindow() const { return m_Window; }
	inline PhysicsWorld* GetPhysicsWorld() const { return m_PhysicWorld; }
	inline static Application& Get() { return *s_Instance; }
	inline bool isInSimulation() { return m_InSimulation; }

	inline void SetSimulationType(SimulationType type) { m_SimulationType = type; }
	inline SimulationType GetSimulationType() { return m_SimulationType; }

private:
	void Update(float deltaTimeSecs);
	void Render();
	float CalculateDeltaTimeSecs();

	bool OnKeyPressed(class KeyPressedEvent& e);
	bool OnSimulationSet(class SimulationSetEvent& e);
	bool OnSimulationStarted(class SimulationStartedEvent& e);
	bool OnSimulationStopped(class SimulationStoppedEvent& e);

	void SpawnCircleAt(float x, float y, glm::vec2 direction, float speed);
	void SpawnCircleByType(SimulationType type);
	void writeFinalCoordsToFile(std::string filePath);
	void WriteCoordsToFileByType(SimulationType type);

	std::string GetCoordsFilePathByType(SimulationType type);

private:
	static Application* s_Instance;
	std::string m_ImageFilePath;

	Window* m_Window;
	imguiWindow* m_ImguiWindow;
	PhysicsWorld* m_PhysicWorld;
	OrthographicCameraController* m_CameraController;
	Renderer2D* m_Renderer2D;
	Image* m_Image;

private:

	SimulationType m_SimulationType{SimulationType::Waterfall};
	bool m_InSimulation{false};
	float m_SimulationFinishTime{ 7.f };

	bool m_ShouldWriteCoords{ false };
	float m_WriteTimeCountDown{ 7.f };
	int m_SpawnedObjectCount{ 0 };
	int m_FrameCount{ 0 };
	float sleepTime{0};
	float appTimeSpent{ 0 };

	float m_TimePassed{ 0.f };
	bool m_Running = true;
	float m_LastFrameTime = 0.0f;
};