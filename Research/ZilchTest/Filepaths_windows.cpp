
#include <iostream>
#include <windows.h>

#include "Filepaths.hpp"

const std::string FilePaths::BasePath = FilePaths::CacheBasePath();

const std::string FilePaths::GetBasePath(std::string path)
{
  return BasePath + GetWinPath(path);
}

const std::string FilePaths::CacheBasePath()
{
  char buffer[MAX_PATH];
  if(!GetModuleFileName( NULL, buffer, MAX_PATH ))
    throw "invalid path";

  std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
  return std::string( buffer ).substr( 0, pos) + "\\";
}
