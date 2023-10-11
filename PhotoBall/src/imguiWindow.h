#pragma once

#include "imgui.h"
#include "imfilebrowser.h"
#include <functional>

using EventCallbackFn = std::function<void(class Event&)>;

class imguiWindow
{
public:
	imguiWindow(struct GLFWwindow* window);
	~imguiWindow();

	void OnUpdate();
	void OnRender();
	inline void SetEventCallback(const EventCallbackFn& callBack) { m_EventCallback = callBack; };
	inline void SetInSimulation(bool simulationState) { m_InSimulation = simulationState; };

private:

	void Init();

private:

	struct ImGuiIO& m_io;
	GLFWwindow* m_GLFWwindow;
	ImGui::FileBrowser m_FileDialog;
	bool m_EventSentAlready{ false };
	std::string m_SelectedFileName;

	EventCallbackFn m_EventCallback;
	bool m_InSimulation{ false };
};

