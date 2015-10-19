-----------------------------------------------------------
-- Global Vars --
-----------------------------------------------------------

-- Project/Solution vars
local slnName = "CCReflect"

-- Directory vars
local workingDir = "../"
local depDir = "../dep/"
local projDir = "../vs/"
local srcDir = "../src/"
local objDir = "../temp/"
local libDir = "../bin/"
local gameDir = "../bin/"

local dxInclude = os.getenv("DXSDK_DIR") .. "/Include/"
local dxLib = os.getenv("DXSDK_DIR") .. "/Lib/x86/"

local releaseFlags = {
  "WinMain", 
  "Symbols",
  "Optimize"
}

local debugFlags = {
  "WinMain", 
  "Symbols",
  "StaticRuntime"
}

-- Preprocessor defines
local releaseDefines = {
  "_CRT_SECURE_NO_WARNINGS",
  "NDEBUG"
}

local debugDefines = {
  "_CRT_SECURE_NO_WARNINGS",
  "DEBUG"
}

-- Projects
local windowedApps = {
}

local consoleApps = {
  "driver",
  "TestEnumEditor",
  "MessagingDemo"
}

local staticLibraries = {
  "Containers",
  "Utility",
  "Utility",
  "CCReflect",
  "Lua"
}

local dynamicLibraries = {
}

-----------------------------------------------------------
-- Functions --
-----------------------------------------------------------

-- Reads each line of a file into a table
-- filename : The name of the file to read
-- ret      : The table of lines from the file
function ReadTableFromFile ( filename )
  local tbl = {}
  local file = io.open( filename )
  
  if file ~= nil then
    for line in io.lines ( filename )
    do
      table.insert ( tbl, line )
    end
  end

  return tbl
end

-- Sets up a visual studio project (must be created before function call)
-- projName : The name of the project
-- projKind : The type of the project (http://industriousone.com/kind)
-- projLoc  : The directory that will contain the VS proj files
-- srcDir   : The directory containing this project's source
-- outDir   : The directory to place the proj's output in (.exe, .lib, or .dll)
function SetUpProj ( projName, projKind, projLoc, projSrcDir, projOutDir )
  language ( "C++" )
  kind ( projKind )  
  location ( projLoc )      
  objdir ( objDir )
  targetdir ( projOutDir )
  includedirs { srcDir .. "**", depDir .. "**" , dxInclude }
  libdirs { libDir .. "**", depDir .. "**" , dxLib }
  debugdir ( gameDir )
  
  files { 
    projSrcDir .. projName .. "/**.h",
    projSrcDir .. projName .. "/**.c",
    projSrcDir .. projName .. "/**.cpp"
  }

  vpaths { [""] = srcDir .. projName } 

  if( projName ~= "Lua" ) then
    pchheader ( "Precompiled.h" )
    pchsource ( projSrcDir .. projName .. "/Precompiled/Precompiled.cpp" )
  end
    
  buildoptions { "/wd4127", "/wd4100", "/wd4481", "/wd4201", "/wd4189" }
  
  -- Debug Configuration Settings
  configuration "Debug"
    defines { debugDefines }
    flags { debugFlags }
    
    links { ReadTableFromFile ( projSrcDir ..  projName .. "/" .. "dep_debug.txt" ) }
  
  -- Release Configuration Settings
  configuration "Release"
    defines { releaseDefines }
    flags { releaseFlags }

    links { ReadTableFromFile ( projSrcDir ..  projName .. "/" .. "dep_release.txt" ) }
end

-----------------------------------------------------------
-- Logic --
-----------------------------------------------------------

if _ACTION ~= nil then
  solution ( slnName )
  location ( projDir .. _ACTION )
  configurations { "Debug", "Release" }
    
  for i = 1,4
  do

    if i == 1 then
      local projKind = "WindowedApp"
      local projLoc = projDir .. _ACTION

      for j, proj in ipairs ( windowedApps )
      do
        project ( proj )
        SetUpProj ( proj, projKind, projLoc, srcDir, gameDir )
      end

    elseif i == 2 then
      local projKind = "ConsoleApp"
      local projLoc = projDir .. _ACTION

      for j, proj in ipairs ( consoleApps )
      do
        project ( proj )
        SetUpProj ( proj, projKind, projLoc, srcDir, libDir )
      end
      
    elseif i == 3 then
      local projKind = "StaticLib"
      local projLoc = projDir .. _ACTION

      for j, proj in ipairs ( staticLibraries )
      do
        project ( proj )
        SetUpProj ( proj, projKind, projLoc, srcDir, libDir )
      end
      
    elseif i == 4 then
      local projKind = "SharedLib"
      local projLoc = projDir .. _ACTION

      for j, proj in ipairs ( dynamicLibraries )
      do
        project ( proj )
        SetUpProj ( proj, projKind, projLoc, srcDir, libDir )
      end
    end -- if/else
  end -- main loop
end -- if _ACTION ~= nil then