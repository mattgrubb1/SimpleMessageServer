--require "lua" -- add this to load your module

-- premake5.lua
workspace "SimpleMessageServer"
  location "./"
  architecture "x86_64"
  configurations { "Debug", "Release" }

  filter "configurations:Debug"
    symbols "On"

  filter "configurations:Release"
    optimize "On"

  filter { }

project "MessageServer"
  kind "WindowedApp"
  language "C++"
  location "MessageServer"
  targetdir "bin/%{cfg.buildcfg}"
  cppdialect "C++14"
  characterset("MBCS")
  files { "MessageServer/source/SimpleServerPCH.cpp",
          "MessageServer/headers/SimpleServerPCH.h",
          "MessageServer/headers/**.h",
          "MessageServer/source/**.cpp" }
  pchheader "SimpleServerPCH.h"
  pchsource "MessageServer/source/SimpleServerPCH.cpp"
  includedirs { "MessageServer/headers", "MessageServer/source"}
  links { "Ws2_32.lib" }
  