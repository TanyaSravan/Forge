 workspace "Forge"
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

include "Forge/vendor/GLFW"
include "Forge/vendor/Glad"

project "Forge"
    location "Forge"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/"..outputDir.."/%{prj.name}")
    objdir ("bin-int/"..outputDir.."/%{prj.name}")

    pchheader "FGpch.h"
    pchsource "Forge/src/FGpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.Glad}",
    }

    links{
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:Windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"
        defines{
            "FG_PLATFORM_WINDOWS",
            "FG_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputDir.."/Sandbox")
        }

    filter "configurations: Debug"
        defines {
            "FG_DEBUG",
            "FG_ENABLE_ASSERTS"
        }
        buildoptions "/MDd"
        symbols "On"
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "On"
        buildoptions "/MD"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "On"
        buildoptions "/MD"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/"..outputDir.."/%{prj.name}")
    objdir ("bin-int/"..outputDir.."/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "Forge/vendor/spdlog/include",
        "Forge/src"
    }

    filter "system:Windows"
        cppdialect "C++17"
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
        symbols "On"
        buildoptions "/MDd"
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "On"
        buildoptions "/MD"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "On"
        buildoptions "/MD"

    
    
