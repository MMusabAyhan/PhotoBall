#pragma once

#include <string>

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title = "2D Physics Engine",
		uint32_t width = 1280,
		uint32_t height = 720)
		: Title(title), Width(width), Height(height)
	{
	}
};

using EventCallbackFn = std::function<void(class Event&)>;

class Window 
{
public:

	Window(const WindowProps& props);
	~Window();

	void OnUpdate();

	inline unsigned int GetWidth() const { return m_Data.Width; }
	inline unsigned int GetHeight() const { return m_Data.Height; }

	inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
	inline virtual void* GetNativeWindow() const { return m_Window; }

private:
	void Init(const WindowProps& props);
	void Shutdown();
private:
	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		uint32_t Width, Height;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;

};