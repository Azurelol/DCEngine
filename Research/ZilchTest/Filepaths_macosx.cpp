
#include <errno.h>      // errno
#include <libproc.h>    // pid_t, getpid, proc_pidpath, PROC_PIDPATHINFO_MAXSIZE
#include <unistd.h>     // POSIX 
#include <stdlib.h>     // realpath

#include <iostream>

#include "Filepaths.hpp"


// setting the static base path variable on filePaths
const std::string FilePaths::BasePath = FilePaths::CacheBasePath();

const std::string FilePaths::GetBasePath(std::string path)
{
  return BasePath + GetPosixPath(path);
}

const std::string FilePaths::CacheBasePath()
{
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

  if(proc_pidpath (getpid(), pathbuf, sizeof(pathbuf)) <= 0)
    throw strerror(errno);
  
  std::string path(pathbuf);

  size_t path_end = path.find_last_of("/");
  if(path_end == std::string::npos)
    throw "invalid path";

  // turn /My/Path/To/Executable into /My/Path/To/
  path = path.substr(0, path_end + 1);

  return path;
}


//! @todo why does this cause segfaults...
// const std::string FilePaths::RealPath(std::string path)
// {
//   // allocates buffer automatically. need to free
//   char *real_path=  realpath(path, NULL);
//   std::string out(real_path);
//   free(real_path);
//   return out;
// }
