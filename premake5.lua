 workspace "Forge"
    startproject "Sandbox"
    architecture "x64"
    configurations{
        "Debug",
        "Release",
        "Dist"
    }
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Forge/vendor/GLFW/include"
IncludeDirs["Glad"] = "Forge/vendor/Glad/include"
IncludeDirs["imgui"] = "Forge/vendor/imgui"
IncludeDirs["glm"] = "Forge/vendor/glm"
IncludeDirs["stb_image"] = "Forge/vendor/stb_image"


include "Forge/vendor/GLFW"
include "Forge/vendor/Glad"
include "Forge/vendor/imgui"

project "Forge"
    location "Forge"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/"..outputDir.."/%{prj.name}")
    objdir ("bin-int/"..outputDir.."/%{prj.name}")
    staticruntime "on"

    pchheader "FGpch.h"
    pchsource "Forge/src/FGpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
    }

    includedirs{
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.imgui}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.stb_image}",
    }

    links{
        "GLFW",
        "Glad",
        "imgui",
        "opengl32.lib"
    }

    defines{
        "_CRT_SECURE_NO_WARNINGS",
        "FG_ENABLE_ASSERTS"
    }

    filter "system:Windows"
        staticruntime "on"
        systemversion "latest"
        defines{
            "FG_PLATFORM_WINDOWS",
            "FG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations: Debug"
        defines {
            "FG_DEBUG",
        }
        runtime "Debug"
        symbols "on"
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "on"
        runtime "Release"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "on"
        runtime "Release"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir ("bin/"..outputDir.."/%{prj.name}")
    objdir ("bin-int/"..outputDir.."/%{prj.name}")
    staticruntime "on"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Forge/vendor/spdlog/include",
        "Forge/src",
        "Forge/vendor",
        "%{IncludeDirs.glm}",
    }

    filter "system:Windows"
        staticruntime "on"
        systemversion "latest"

        defines{
            "FG_PLATFORM_WINDOWS"
        }
    
    links {
        "Forge"
    }

    filter "configurations: Debug"
        defines "FG_DEBUG"
        symbols "on"
        runtime "Debug"
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "on"
        runtime "Release"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "on"
        runtime "Release"

    
    
