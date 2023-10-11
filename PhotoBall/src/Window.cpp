
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Window.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

static bool s_GLFWInitialized = false;

Window::Window(const WindowProps& props)
{
	Init(props);
}

Window::~Window()
{
	Shutdown();
}

void Window::OnUpdate()
{
	if (Application::Get().isInSimulation() && glfwGetInputMode(m_Window, GLFW_CURSOR) != GLFW_CURSOR_HIDDEN)
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!Application::Get().isInSimulation() && glfwGetInputMode(m_Window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::Init(const WindowProps& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	if (!s_GLFWInitialized)
	{
		int success = glfwInit();
		_ASSERT(success, "Could not intialize GLFW!");
		s_GLFWInitialized = true;
	}

	int count;
	int windowWidth, windowHeight;
	int monitorX, monitorY;

	GLFWmonitor** monitors = glfwGetMonitors(&count);
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
	windowWidth = props.Width;
	windowHeight = windowWidth;

	glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
	_ASSERT(m_Window, "Could not create Window!");

	// (2).  Reset the window hints to default
	glfwDefaultWindowHints();

	//glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, GLFW_FALSE);

	glfwSetWindowPos(m_Window,
		monitorX + (videoMode->width - windowWidth) / 2,
		monitorY + (videoMode->height - windowHeight) / 2);

	glfwShowWindow(m_Window);

	glfwMakeContextCurrent(m_Window);

	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	_ASSERT(status, "Failed to initialize Glad!");

	glfwSetWindowUserPointer(m_Window, &m_Data);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);

		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});


	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});



}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}