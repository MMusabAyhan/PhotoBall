#include "OrthographicCameraController.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Input.h"
#include "Application.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "KeyCodes.h"
#include "Input.h"

#include "PhysicsWorld.hpp"
#include <algorithm>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

OrthographicCameraController::OrthographicCameraController(float left, float right, float bottom, float top, bool rotation)
	: m_Camera{ left, right, bottom, top }, m_CameraPosition({ 0.f, 0.f, 0.f }),
	m_CameraRotation(0), m_ZoomLevel((top - bottom)/ 2.f), m_Rotation(rotation), m_AspectRatio((float) (right - left) / (top - bottom))
{
}

OrthographicCameraController::~OrthographicCameraController() 
{
}

void OrthographicCameraController::OnUpdate(float deltaTimeSecs) 
{
	if (Input::isKeyPressed(KEY_A))
	{
		m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
		m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
	}
	else if (Input::isKeyPressed(KEY_D))
	{
		m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
		m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
	}

	if (Input::isKeyPressed(KEY_W))
	{
		m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
		m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
	}
	else if (Input::isKeyPressed(KEY_S))
	{
		m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
		m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * deltaTimeSecs;
	}

	if (m_Rotation)
	{
		if (Input::isKeyPressed(KEY_Q) && !QKeyPressedAlready) 
		{
			QKeyPressedAlready = true;
			m_CameraRotation += 90.f;
		}
		if (Input::isKeyPressed(KEY_E) && !EKeyPressedAlready) 
		{
			EKeyPressedAlready = true;
			m_CameraRotation -= 90.f;
		}

		if (m_CameraRotation > 180.0f)
			m_CameraRotation -= 360.0f;
		else if (m_CameraRotation <= -180.0f)
			m_CameraRotation += 360.0f;

		m_Camera.SetRotation(m_CameraRotation);
	}

	m_Camera.SetPosition(m_CameraPosition);

	m_CameraTranslationSpeed = m_ZoomOutConstant * m_ZoomLevel + (m_ZoomInConstant / m_ZoomLevel);

	if (Input::isKeyReleased(KEY_Q))
		QKeyPressedAlready = false;

	if (Input::isKeyReleased(KEY_E))
		EKeyPressedAlready = false;

	if (Input::isKeyPressed(KEY_SPACE)) 
		ResetCamera();

}

void OrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) 
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	return false;
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	unsigned int height = Application::Get().GetPhysicsWorld()->GetWorldHeight();

	m_ZoomLevel -= e.GetYOffset() * 0.05f * std::max(1,(int)height);
	m_ZoomLevel = std::max(m_ZoomLevel, 20.f);
	m_ZoomLevel = std::min(m_ZoomLevel, 300.f);

	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

void OrthographicCameraController::ResetCamera()
{
	int worldWidth = Application::Get().GetPhysicsWorld()->GetWorldWidth();
	int worldHeight = Application::Get().GetPhysicsWorld()->GetWorldHeight();

	m_CameraRotation = 0.f;
	m_Camera.SetRotation(0.f);

	m_CameraPosition = { worldWidth / 2.f,worldHeight / 2.f,0.f };
	m_Camera.SetPosition({ worldWidth / 2.f,worldHeight / 2.f,0.f });

	m_ZoomLevel = worldHeight / 2.f;

	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

