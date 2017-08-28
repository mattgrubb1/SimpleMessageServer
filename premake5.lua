--require "lua" -- add this to load your module

-- premake5.lua
workspace "SimpleMessageServer"
  location "./"
  architecture "x64"
  configurations { "Debug", "Release" }

  filter "configurations:Debug"
    symbols "On"

  filter "configurations:Release"
    optimize "On"

  filter { }

project "SimpleServer"
  kind "ConsoleApp"
  language "C++"
  location "MessageServer"
  targetdir "bin/%{cfg.buildcfg}"
  flags { "C++14" }
  files { "MessageServer/headers/**.h", "MessageServer/source/**.cpp" }

  