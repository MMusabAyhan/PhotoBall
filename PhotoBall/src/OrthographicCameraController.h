#pragma once

#include "OrthographicCamera.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class OrthographicCameraController
{
public:
	OrthographicCameraController(float left, float right, float bottom, float top, bool rotation = true);
	~OrthographicCameraController();

	void OnUpdate(float deltaTimeSecs);
	void OnEvent(class Event& e);

	inline float GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(float level) { m_ZoomLevel = level; }
	void SetPosition(const glm::vec3& position) { m_CameraPosition = position; m_Camera.SetPosition(position); }

	inline OrthographicCamera& GetOrthographicCamera() { return m_Camera; };
	inline const OrthographicCamera& GetOrthographicCamera() const { return m_Camera; };

private:
	bool OnWindowResized(class WindowResizeEvent& e);
	bool OnMouseScrolled(class MouseScrolledEvent& e);

	void ResetCamera();
private:
	float m_AspectRatio;
	float m_ZoomLevel;
	float m_ZoomOutConstant = 1.f;
	float m_ZoomInConstant = 7500.f;

	bool QKeyPressedAlready{ false };
	bool EKeyPressedAlready{ false };

	bool m_Rotation;
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;


	OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation;
};
