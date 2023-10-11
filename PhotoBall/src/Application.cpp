#include "Application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer2D.h"
#include "Window.h"
#include "OrthographicCameraController.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "MouseButtonCodes.h"
#include "KeyCodes.h"
#include "Input.h"

#include "PhysicsWorld.hpp"
#include "PhysicObject.hpp"
#include <windows.h>
#include "glm/glm.hpp"
#include <memory>
#include "Image.hpp"
#include "imguiWindow.h"

#include <iostream>
#include <fstream>


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name, unsigned int _width, unsigned int _height, const std::string& filePath)
	: m_ImageFilePath(filePath), m_Image(nullptr), m_CameraController(nullptr), m_PhysicWorld(nullptr), m_Renderer2D(nullptr)
{
	s_Instance = this;

	m_Window = new Window({ name,_width,_height });
	m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	m_ImguiWindow = new imguiWindow((GLFWwindow*)m_Window->GetNativeWindow());
	m_ImguiWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	m_Renderer2D = new Renderer2D();
	m_Renderer2D->Init();
}

void Application::Run()
{
	float fpsCap = 45.f;
	float deltaTimeSecsFpsCap = 1 / fpsCap;
	float deltaTimeMilliSecsCap = 1000 * deltaTimeSecsFpsCap;

	while (m_Running)
	{
		double deltaTimeMilliSecs = 1000 * CalculateDeltaTimeSecs();

		// [ Writing final Coords ]
		if (m_ShouldWriteCoords)
		{
			if (m_SpawnedObjectCount >= 25000)
				m_WriteTimeCountDown -= deltaTimeMilliSecs / 1000.f;

			if (m_WriteTimeCountDown < 0)
			{
				WriteCoordsToFileByType(m_SimulationType);
				m_Running = false;
			}
		}

		if (m_SpawnedObjectCount >= 25000)
		{
			m_SimulationFinishTime -= deltaTimeMilliSecs / 1000.f;

			if (m_SimulationFinishTime < 0)
			{
				m_SimulationFinishTime = 7.f;
				SimulationStoppedEvent e;
				OnEvent(e);
			}
		}

		float startTime = (float)glfwGetTime();

		Update(deltaTimeSecsFpsCap);
		Render();

		float endTime = (float)glfwGetTime();
		appTimeSpent = endTime - startTime;
		float sleepTime = deltaTimeMilliSecsCap - appTimeSpent * (1000);

		if (!m_Running)
			return;
		if (!m_InSimulation)
			continue;
		if (m_SpawnedObjectCount < 25000)
		{
			sleepTime = max(0, sleepTime);
			Sleep(sleepTime / 2.f);
		}
	}
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	dispatcher.Dispatch<SimulationSetEvent>(BIND_EVENT_FN(Application::OnSimulationSet));
	dispatcher.Dispatch<SimulationStartedEvent>(BIND_EVENT_FN(Application::OnSimulationStarted));
	dispatcher.Dispatch<SimulationStoppedEvent>(BIND_EVENT_FN(Application::OnSimulationStopped));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));

	if (m_InSimulation)
		m_CameraController->OnEvent(e);
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

void Application::Update(float deltaTimeSecs)
{
	m_TimePassed += deltaTimeSecs;

	m_Window->OnUpdate();

	if (m_InSimulation)
	{
		int objectCount = m_PhysicWorld->GetPhysicsObjects().size();
		m_FrameCount++;

		if (objectCount < 25000)
		{
			SpawnCircleByType(m_SimulationType);
		}

		m_CameraController->OnUpdate(deltaTimeSecs);
		m_PhysicWorld->OnUpdate(deltaTimeSecs);
	}
	else
	{
		m_ImguiWindow->OnUpdate();
	}
}

void Application::Render()
{
	m_Renderer2D->Clear();

	if (m_PhysicWorld)
	{
		m_Renderer2D->BeginScene(m_CameraController->GetOrthographicCamera());
	
		auto m_Circles = m_PhysicWorld->GetPhysicsObjects();

		for (int i = 0; i < m_Circles.size(); i++)
		{
			auto currentCircle = m_Circles[i];

			glm::vec3 pos = { currentCircle.position.x, currentCircle.position.y, 0.f };

			glm::vec4 color = currentCircle.color;
			float radius = 0.5f;

			m_Renderer2D->DrawCircle(pos, radius, color, 1.f, 0.005f, -1);
		}

		m_Renderer2D->EndScene();
	}

	m_ImguiWindow->OnRender();
}

float Application::CalculateDeltaTimeSecs()
{
	float time = (float)glfwGetTime();
	float deltaTimeSecs = time - m_LastFrameTime;
	m_LastFrameTime = time;

	return deltaTimeSecs;
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == KEY_ESCAPE) 
	{
		WindowCloseEvent e;
		OnWindowClose(e);
	}

	return false;
}

bool Application::OnSimulationSet(SimulationSetEvent& e)
{
	delete m_CameraController;
	delete m_Renderer2D;
	delete m_PhysicWorld;
	delete m_Image;

	m_SpawnedObjectCount = 0;

	float worldWidth = 145.f;
	float worldHeight = 145.f;

	m_CameraController = new OrthographicCameraController(-worldWidth/2.f , worldWidth/2.f , -worldHeight/2.f , worldHeight/2.f );
	m_CameraController->SetPosition({ worldWidth / 2.f, worldHeight / 2.f, 0.f });

	m_Renderer2D = new Renderer2D();
	m_Renderer2D->Init();

	std::string coordsFilePath = GetCoordsFilePathByType(m_SimulationType);
	m_PhysicWorld = new PhysicsWorld(worldWidth, worldHeight, coordsFilePath);

	m_ImageFilePath = e.GetFilePath();
	m_Image = new Image(m_ImageFilePath);

	return false;
}

bool Application::OnSimulationStarted(SimulationStartedEvent& e)
{
	m_InSimulation = true;

	return false;
}

bool Application::OnSimulationStopped(SimulationStoppedEvent& e)
{
	m_InSimulation = false;
	return false;
}

void Application::SpawnCircleAt(float x, float y, glm::vec2 direction, float speed)
{
	float width = m_Image->GetWidth();
	float height = m_Image->GetHeight();

	float initialSpeed = speed;

	auto finalCoords = m_PhysicWorld->GetFinalObjectCoords();
	float circleFinalX = finalCoords[m_SpawnedObjectCount].x;
	float circleFinalY = finalCoords[m_SpawnedObjectCount].y;

	int correspondingX = circleFinalX * (float)width / m_PhysicWorld->GetWorldWidth();
	int correspondingY = circleFinalY * (float)height / m_PhysicWorld->GetWorldHeight();

	unsigned char* data = m_Image->GetData();

	int index = (correspondingY * width + correspondingX) * 4;

	float r = data[index] / 255.f;
	float g = data[index + 1] / 255.f;
	float b = data[index + 2] / 255.f;

	glm::vec4 color = { r,g,b,1 };
	m_PhysicWorld->AddObject({ x,y }, initialSpeed, direction,  color);

	m_SpawnedObjectCount++;
}

void Application::SpawnCircleByType(SimulationType type)
{
	switch (type)
	{
	case Waterfall:
	{
		for (int i = 0; i < 30; i++)
		{
			float x = m_PhysicWorld->GetWorldWidth() * 0.5;
			float y = m_PhysicWorld->GetWorldHeight() * 0.6;
			glm::vec2 direction = {1,0};
			float speed = 0.5f;
			SpawnCircleAt(x, y + 1.75f * i, direction,speed);
		}
	}
		break;
	case Shower:
	{
		float angle{200.f};
		for (int i = 0; i < 5; i++)
		{
			float startX = m_PhysicWorld->GetWorldWidth() * 0.5 - 26.25f;
			float x = startX;
			float y = m_PhysicWorld->GetWorldHeight() * 0.9;
			
			glm::vec2 direction = { glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle))};
			float speed = 0.5f;
			for (int j = i * 6; j < (i * 6) + 6; j++)
			{
				SpawnCircleAt(x + 1.75f * j, y, direction, speed);
			}
		
			angle += 35.f;
		}
	}
		break;
	case TwoReversedWaterFall:
	{
		float speed = 0.15f;
		float startY = m_PhysicWorld->GetWorldHeight() * 0.75;
		
		for (int i = 0; i < 10; i++)
		{
			float x = m_PhysicWorld->GetWorldWidth() * 0.5;
		
			glm::vec2 direction = {1,0};
			SpawnCircleAt(x, startY + 1.75f * i, direction, speed);
		}
		for (int i = 10; i < 20; i++)
		{
			float x = m_PhysicWorld->GetWorldWidth() * 0.5;
			glm::vec2 direction = {-1,0 };
			SpawnCircleAt(x, startY + 1.75f * i, direction, speed);
		}
	}
		break;
	case Sprinkle:
	{
		float angle{270.f};
		float oscillationSpeed = m_SpawnedObjectCount / 1000.f;
		angle += glm::sin(glm::radians(90.f) * oscillationSpeed) * 45.f;
		
		for (int i = 0; i < 20; i++)
		{
			float x = m_PhysicWorld->GetWorldWidth() * 0.4;
			float y = m_PhysicWorld->GetWorldHeight() * 0.9;
			glm::vec2 direction = { glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)) };
			float speed = 0.15f;
			SpawnCircleAt(x + 1.75f * i, y, direction,speed);
		}
	}
		break;
	case MovingCloud:
	{
		float x = m_PhysicWorld->GetWorldWidth() * 0.4;
		float oscillationSpeed = m_SpawnedObjectCount / 1000.f;
		x += glm::sin(glm::radians(60.f) * oscillationSpeed) * m_PhysicWorld->GetWorldHeight() * 0.2f;

		for (int i = 0; i < 20; i++)
		{
			float y = m_PhysicWorld->GetWorldHeight() * 0.9;
			glm::vec2 direction = { 0,-1.f };
			float speed = 0.15f;
			SpawnCircleAt(x + 1.75f * i, y, direction, speed);
		}
	}
		break;
	default:
		break;
	}

}

void Application::writeFinalCoordsToFile(std::string filePath)
{
	std::ofstream myfile;
	myfile.open(filePath);

	auto m_Circles = m_PhysicWorld->GetPhysicsObjects();

	for (int i = 0; i < m_Circles.size(); i++)
	{
		auto currentCircle = m_Circles[i];
		myfile << currentCircle.position.x << " " << currentCircle.position.y << "\n";
	}

	myfile.close();
}

void Application::WriteCoordsToFileByType(SimulationType type)
{
	std::string filePath{};

	std::string fileRoot = "resources/CircleCoords/";
	std::string fileName{};

	switch (m_SimulationType)
	{
	case Waterfall:
		fileName = "WaterFall.txt";
		break;
	case Shower:
		fileName = "Shower.txt";
		break;
	case TwoReversedWaterFall:
		fileName = "TwoReversedWaterFall.txt";
		break;
	case Sprinkle:
		fileName = "Sprinkle.txt";
		break;
	case MovingCloud:
		fileName = "MovingCloud.txt";
		break;
	default:
		break;
	}

	filePath = fileRoot + fileName;
	writeFinalCoordsToFile(filePath);
}

std::string Application::GetCoordsFilePathByType(SimulationType type)
{
	std::string fileRoot = "resources/CircleCoords/";
	std::string fileName{};

	switch (type)
	{
	case Waterfall:
		fileName = "WaterFall.txt";
		break;
	case Shower:
		fileName = "Shower.txt";
		break;
	case TwoReversedWaterFall:
		fileName = "TwoReversedWaterFall.txt";
		break;
	case Sprinkle:
		fileName = "Sprinkle.txt";
		break;
	case MovingCloud:
		fileName = "MovingCloud.txt";
		break;
	default:
		break;
	}

	return fileRoot + fileName;
}


//TODO - I may add it at the end

//glm::vec4 Application::GetRainbowColor(float ratio)
//{
//	glm::vec4 outColor;
//
//	float r = (int)ratio / 10 % 255;
//	float g = (int)ratio / 100 % 255 - r;
//	float b = (int)ratio % 255 - g;
//
//	return outColor;
//}
