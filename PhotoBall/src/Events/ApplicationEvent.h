#pragma once

#include "Event.h"
#include <sstream>

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height) {}

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	uint32_t m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class SimulationSetEvent : public Event
{
public:
	SimulationSetEvent(std::string& filePath)
		: m_FilePath(filePath) {}

	inline const std::string& GetFilePath() const { return m_FilePath; }

	EVENT_CLASS_TYPE(SimulationSet)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	std::string m_FilePath;
};

class SimulationStartedEvent: public Event
{
public:
	SimulationStartedEvent() {}

	EVENT_CLASS_TYPE(SimulationStarted)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	std::string m_FilePath;
};

class SimulationStoppedEvent : public Event
{
public:
	SimulationStoppedEvent() {}

	EVENT_CLASS_TYPE(SimulationStopped)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class AppTickEvent : public Event
{
public:
	AppTickEvent() {}

	EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
	AppUpdateEvent() {}

	EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
	AppRenderEvent() {}

	EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
