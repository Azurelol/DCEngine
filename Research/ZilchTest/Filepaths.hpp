
// Build system must compile againstOS-specific implementation file at runtime.
// For example, on mac use filepaths_macosx.cpp

#pragma once

#include <string>
#include <algorithm>

class FilePaths
{
public:
  static const std::string GetPosixPath(std::string path)
  {
    std::replace( path.begin(), path.end(), '\\', '/');
    return path;
  }
  static const std::string GetWinPath(std::string path)
  {
    std::replace( path.begin(), path.end(), '/', '\\');
    return path;
  }

  // Executable base path at start time. Guaranteed to end with a /
  static const std::string GetBasePath(std::string path);
  static const std::string GetRealPath(std::string path);
private:
  static const std::string CacheBasePath();
  static const std::string BasePath;
};



