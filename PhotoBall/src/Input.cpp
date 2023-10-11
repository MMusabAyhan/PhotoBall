#include "Input.h"
#include "GLFW/glfw3.h"
#include "Application.h"
#include "Window.h"

bool Input::isMouseButtonPressed(int mouseButton)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
	auto state = glfwGetMouseButton(window, mouseButton);
	return state == GLFW_PRESS;
}
bool Input::isKeyPressed(int keyCode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
	auto state = glfwGetKey(window, keyCode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::isKeyReleased(int keyCode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
	auto state = glfwGetKey(window, keyCode);
	return state == GLFW_RELEASE;
}
float Input::GetMousePositionX()
{
	auto [x, y] = GetMousePosition();
	return x;
}
float Input::GetMousePositionY()
{
	auto [x, y] = GetMousePosition();
	return y;
}

std::pair<float, float> Input::GetMousePosition()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}
