workspace "CnnrEngine"
    toolset "clang"
    buildoptions { "-std=c++1z" }
    objdir "obj/%{cfg.system}/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"
    targetdir "bin/%{cfg.system}/%{cfg.platform}/%{cfg.buildcfg}"
    pic "On"

    libdirs { "$(CONNORLIB_HOME)/bin/%{cfg.system}/%{cfg.platform}" }
    includedirs {
        "Source",
        "Source/Vendor/luajit/include",
        "Source/Vendor/gsl/include",
        "$(CONNORLIB_HOME)/include"
    }

    configurations { "Debug", "Release" }
    platforms { "x64" }

project "AssetPipeline"
    kind "StaticLib"
    language "C++"

    files { "Source/AssetPipeline/*.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "LuaInterface"
    kind "StaticLib"
    language "C++"

    files { "Source/LuaInterface/*.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "Engine"
    kind "SharedLib"
    language "C++"

    files { "Source/Engine/*.cpp" }
    links {
        "AssetPipeline",
        "LuaInterface",
        "serialization",
        "imageload",
        "messageipc"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
