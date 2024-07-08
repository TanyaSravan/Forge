workspace "Forge"
    architecture "x64"
    configurations{
        "Debug",
        "Release",
        "Dist"
    }
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Forge"
    location "Forge"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/"..outputDir.."/%{prj.name}")
    objdir ("bin-int/"..outputDir.."/%{prj.name}")

    pchheader "FGpch.h"
    pchsource "Forge/src/Forge/FGpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs{
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:Windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"
        defines{
            "FG_PLATFORM_WINDOWS",
            "FG_BUILD_DLL"
        }
    
        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..outputDir.."/Sandbox")
        }

    filter "configurations: Debug"
        defines "FG_DEBUG"
        symbols "On"
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "On"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "On"

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
    
    filter "configurations: Release"
        defines "FG_RELEASE"
        optimize "On"

    filter "configurations: Dist"
        defines "FG_DIST"
        optimize "On"

    
    
