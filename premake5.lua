-- OpenGL--2DPhysicsEngine--imgui
workspace "PhotoBall"
	architecture "x64"
	startproject "PhotoBall"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	-- Include directories relative to PhotoBall
	IncludeDir = {}
	IncludeDir["GLFW"] = "vendor/glfw/include"
	IncludeDir["Glad"] = "vendor/glad/include"
	IncludeDir["glm"] = "vendor/glm"
	IncludeDir["stb_image"] = "vendor/stb_image"
	IncludeDir["imgui"] = "vendor/imgui"
	
	-- Projects
	group "Dependencies"
		include "PhotoBall/vendor/glfw"
		include "PhotoBall/vendor/glad"
		include "PhotoBall/vendor/stb_image"
	group ""
	
	include "PhotoBall"
	include "JohnBufferPhysicsEngine"
