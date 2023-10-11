project "PhotoBall"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/imgui/**.h",
		"vendor/imgui/**.cpp",
	}

	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.imgui}",
		"../JohnBufferPhysicsEngine/include"
	}
	
	links 
	{ 
		"GLFW",
		"Glad",
		"stb_image",
		"opengl32.lib",
		"JohnBufferPhysicsEngine"

	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
        optimize "on"
