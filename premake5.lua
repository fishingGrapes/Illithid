workspace "Illithid"
	architecture "x64"
	startproject "Sandbox"


	configurations
	{
		--unoptimized without logginf
		"Debug",
		--optimized with logging
		"Release",
		--optimized without logging
		"Dist"
	}


--configuration-platform-architecture (Debug-Windows-x64)
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "Illithid/vendor/spdlog/include"
IncludeDir["GLFW"] = "Illithid/vendor/GLFW/include"
IncludeDir["GLAD"] = "Illithid/vendor/GLAD/include"
IncludeDir["glm"] = "Illithid/vendor/glm"
IncludeDir["json"] = "Illithid/vendor/json/single_include"


group "dependencies"
	include ("Illithid/vendor/GLFW")
	include ("Illithid/vendor/GLAD")
group ""

project "Illithid"
	location "Illithid"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	--Multithreaded vs Multithreaded DLL (CRT packaged statically with binary)
	staticruntime "on"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("binaries/intermediates/" .. outputdir .. "/%{prj.name}")

	--Set the precompiled header
	pchheader ("ilpch.h")
	--Must be hardcoded
	pchsource ("Illithid/src/ilpch.cpp")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.json}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32.lib"
	}

	filter "system:windows"
			systemversion "latest"
			buildoptions { "/Zc:__cplusplus" }

			defines
			{
				"IL_PLATFORM_WINDOWS"
			}

	filter "configurations:Debug"
			defines { "IL_DEBUG", "IL_ENABLE_ASSERTS" }
			runtime "Debug"
			symbols "on"

	filter "configurations:Release"
			defines "IL_RELEASE"
			runtime "Release"
			optimize "on"

	filter "configurations:Dist"
			defines "IL_DIST"
			runtime "Release"
			optimize "on"



project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("binaries/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Illithid/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.json}"
	}

	links { "Illithid" }

	filter "system:windows"
			systemversion "latest"
			buildoptions { "/Zc:__cplusplus" }

			defines
			{
				"IL_PLATFORM_WINDOWS"
			}


	filter "configurations:Debug"
			defines { "IL_DEBUG", "IL_ENABLE_ASSERTS" }
			runtime "Debug"
			symbols "on"

	filter "configurations:Release"
			defines "IL_RELEASE"
			runtime "Release"
			optimize "on"

	filter "configurations:Dist"
			defines "IL_DIST"
			runtime "Release"
			optimize "on"
