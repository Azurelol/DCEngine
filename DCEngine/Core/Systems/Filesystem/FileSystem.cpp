/*!****************************************************************************\
@file   FileSystem.cpp
@author Christian Sagel
@author Johannes Peter
@brief  A BOOST-powered interface static class that provides common and useful
filesystem operations (such as scanning folders, etc)
@note   The Boost Filesystem library:
http://www.boost.org/doc/libs/1_39_0/libs/filesystem/doc/index.htm
Great reference for using the library:
http://www.technical-recipes.com/2014/using-boostfilesystem/
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

\******************************************************************************/
#include "FileSystem.h"

// Standard libraries
#include <iostream>
#include <algorithm>
// External Libraries
#include <BOOST/filesystem.hpp>
#include <BOOST/lambda/bind.hpp>
#include <BOOST/algorithm/string/replace.hpp>
//#include <BOOST/process.hpp>
#include <POCO\Process.h>

#include <NFD\nfd.h>
#include <TFD\tinyfiledialogs.h>


namespace DCEngine
{
  bool FileSystem::Execute(std::string command)
  {
    std::vector<std::string> arguments;
    arguments.push_back(command);
    //boost::process::context context;
    //boost::process::launch(command, arguments, context);
    //auto a = boost::process::launch<command, 
    //auto a = boost::process::context::basic_work_directory_context();
    //boost::process::launch_shell(command, a);
    command = "explorer";
    Poco::Process::launch(command, arguments);
    return true;
  }
  /*!************************************************************************\
  @brief  Opens a file dialog for opening a file.
  @param  openpath The path from which to start.
  @param  filters What filters to be used for the files.  
  @return The path of the selected file that will be opened.
  @note   Filters are of the format: "png,jpg;pdf"
          The FileOpenDialog is currently set to return a relativePath;
  \**************************************************************************/
  std::string FileSystem::FileOpenDialog(std::string & defaultPath, std::string& filters)
  {
   
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(filters.c_str(), defaultPath.c_str(), &outPath);
    //nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", openPath.c_str(), &outPath);
    if (result == NFD_OKAY)
    {
      //puts("Success!");      
      std::string outPathString(outPath);
      //puts(outPath);
      free(outPath);
      
      // Return a relative path
      return FileSystem::RelativePath(outPathString, boost::filesystem::initial_path().string());
      //return outPathString;
    }
    else if (result == NFD_CANCEL)
    {
      puts("User pressed cancel.");
    }
    else
    {
      printf("Error: %s\n", NFD_GetError());
    }

    // Returns the selected file path
    return std::string();
  }

  /*!************************************************************************\
  @brief  Opens a file dialog for saving a file.
  @param  openpath The path from which to start.
  @param  filters What filters to be used for the files.
  @return The path of the selected file that will be saved.
  @note   Filters are of the format: "png,jpg;pdf"
  \**************************************************************************/
  std::string FileSystem::FileSaveDialog(std::string & defaultPath, std::string& filters)
  {
    nfdchar_t *savePath = NULL;
    nfdresult_t result = NFD_SaveDialog(filters.c_str(), defaultPath.c_str(), &savePath);
    if (result == NFD_OKAY)
    {
      //puts("Success!");
      std::string savePathString(savePath);
      //puts(savePath);
      free(savePath);
      return savePathString;
    }
    else if (result == NFD_CANCEL)
    {
      puts("User pressed cancel.");
    }
    else
    {
      printf("Error: %s\n", NFD_GetError());
    }

    return std::string();
  }

  /*!************************************************************************\
  @brief  Looks for a file or directory in a given path.
  @param  The path of the file or directory
  @return Whether the file or directory was found or not.
  \**************************************************************************/
  bool FileSystem::Exists(filepath path)
  {
    if (boost::filesystem::exists(path))
      return true;
    return false;
  }


  /*!************************************************************************\
  @brief  Finds out when a file was last modified.
  @param  The file path.
  @return The last time the file was modified, an std::time_t object.
  \**************************************************************************/
  std::time_t FileSystem::LastModified(filepath path)
  {
    std::time_t time = 0;
    if (boost::filesystem::exists(path))
      time = boost::filesystem::last_write_time(path);
    return time;
  }


  /*!************************************************************************\
  @brief  Delets a file or directory (and all its contained files)
  @param  The file path.
  @param  The new filename.
  @return A boolean determining the success of the operation.
  \**************************************************************************/
  bool FileSystem::Delete(filepath path)
  {
    if (boost::filesystem::exists(path))
    {
      boost::filesystem::remove_all(path);
      return true;
    }
    return false;
  }

  std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
      str.replace(start_pos, from.length(), to);
      start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
  }

  /*!************************************************************************\
  @brief  Fixes the current path in the string to be compatible with
          file dialogs.
  @param  path The file path.
  @return A string containing the corrected path.
  \**************************************************************************/
  std::string FileSystem::CorrectPath(std::string & path)
  {    
    boost::replace_all(path, "/", "\\");

    std::replace(path.begin(), path.end(), '/', '\\');
    //std::for_each(path.begin(), path.end(), [&](auto val) {
    //  if (val == "/" || val == "\")
    //    val = "\\";
    //
    //} );

    return std::string();
  }

  /*!************************************************************************\
  @brief  Translates an absolute path to a relative path, given the path
          to be translated as well as the path it should be relative to.
  @param  path The file path.
  @param  pathRelativeTo The file path relative to.
  @return A relative path.
  \**************************************************************************/
  std::string FileSystem::RelativePath(const std::string & inputPath, const std::string & pathRelativeTo)
  {
    boost::filesystem::path path = boost::filesystem::absolute(inputPath);
    boost::filesystem::path relativePath = boost::filesystem::absolute(pathRelativeTo);

    // If the root paths are different, return absolute path
    if (path.root_path() != relativePath.root_path())
      return path.string();

    // Initialize the relative path
    boost::filesystem::path result;

    // Find out where the two paths diverge
    boost::filesystem::path::const_iterator iterPath = path.begin();
    boost::filesystem::path::const_iterator iterRelativeTo = relativePath.begin();
    while (*iterPath == *iterRelativeTo
           && iterPath != path.end()
           && iterRelativeTo != relativePath.end()) {
      ++iterPath;
      ++iterRelativeTo;
    }

    // Add "../" for each remaining token in the relativeTo
    if (iterRelativeTo != relativePath.end()) {
      ++iterRelativeTo;
      while (iterRelativeTo != relativePath.end()) {
        result /= "..";
        ++iterRelativeTo;
      }
    }

    // Add the remaining path
    while (iterPath != path.end()) {
      result /= *iterPath;
      ++iterPath;
    }

    return result.string();

    return std::string();
  }



  /*!************************************************************************\
  @brief  Returns the size of the file, in bytes.
  @param  The file path.
  @return The size of the file in bytes, an unsigned integer.
  \**************************************************************************/
  int FileSystem::FileSize(filepath filePath)
  {
    if (boost::filesystem::exists(filePath))
      return static_cast<unsigned int>(boost::filesystem::file_size(filePath));

    return -1;
  }


  /*!************************************************************************\
  @brief  get a file without its extension or preceeding path
  @param  The file path.
  @return A new string without the extension or path
  \**************************************************************************/
  std::string FileSystem::FileNoExtension(filepath filePath)
  {
    return filePath.stem().string();
  }

  /*!************************************************************************\
  @brief  Returns a file's extension.
  @param  The file path.
  @return A new string containing the extension's name.
  \**************************************************************************/
  std::string FileSystem::FileExtension(filepath filePath)
  {
    return filePath.extension().string();
  }

  /*!************************************************************************\
  @brief  Returns whether a file has the given extension.
  @param  The file path.
  @return Whether the file has the specified extension or not.
  \**************************************************************************/
  bool FileSystem::FileHasExtension(filepath filePath, std::string extension)
  {
    if (FileExtension(filePath) == extension)
      return true;
    return false;
  }


  /*!************************************************************************\
  @brief  get a file without its extension
  @param  The file path.
  @return A new string without the path
  \**************************************************************************/
  std::string FileSystem::FileNoPath(filepath filePath)
  {
    return filePath.filename().string();
  }
  

  /*!************************************************************************\
  @brief  Renames a file.
  @param  The file path.
  @param  The new filename.
  @return A boolean determining the success of the operation.
  \**************************************************************************/
  bool FileSystem::FileRename(filepath filePath, std::string newFileName)
  {
    const boost::filesystem::path newPath = filePath.parent_path() / newFileName;
    if (boost::filesystem::exists(filePath))
    {
      boost::filesystem::rename(filePath, newPath);
      return true;
    }

    return false;
  }


  /*!************************************************************************\
  @brief  Creates a file.
  @param  The new file, in its path.
  @return A boolean determining the success of the operation.
  \**************************************************************************/
  bool FileSystem::FileCreate(filepath filePath)
  {
    std::ofstream newFile(filePath.string());
    if (newFile)
    {
      newFile.close();
      return true;
    }
    return false;
  }


  /*!************************************************************************\
  @brief  Extracts a file's contents into a single string.
  @param  The file path.
  @return The string containing the file's contents.
  @note   This operation is particularly useful for deserializing data.
  \**************************************************************************/
  bool FileSystem::FileReadToString(filepath filePath, std::string& output)
  {   
    std::ifstream inputFile(filePath.string());
    if (inputFile)
    {
      std::string content((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
      output = content;
      return true;
    }
    return false;
  }


  /*!************************************************************************\
  @brief  Writes to a file.
  @param  The new file, in its path.
  @return A boolean determining the success of the operation.
  \**************************************************************************/
  bool FileSystem::FileWriteString(filepath filePath, std::string& input, bool append)
  {
    std::ofstream file(filePath.string());
    
    if (file)
    {
      if (!append)
        file.clear();

      file << input;
      file.close();
      return true;
    }
    return false;
  }


  /*!************************************************************************\
  @brief
  @param  The folder path.
  @return The string vector, containing all the files.
  \**************************************************************************/
  bool FileSystem::DirectoryListPaths(filepath dirPath,
    std::vector<filepath>& paths)
  {

    if (boost::filesystem::exists(dirPath))
    {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath))
        paths.push_back(file.path().filename());
      return true;
    }

    return false;
  }


  /*!************************************************************************\
  @brief
  @param  The folder path.
  @return The string vector, containing all the files.
  \**************************************************************************/
  bool FileSystem::DirectoryListFiles(filepath dirPath,
    std::vector<filepath>& files)
  {

    if (boost::filesystem::exists(dirPath))
    {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath))
        if (boost::filesystem::is_regular_file(file))
          files.push_back(file.path().filename());

      return true;
    }

    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Scans a folder, given a path and extracts all the file names
  into a string vector.
  @param  The folder path.
  @return The string vector, containing all the file names.
  */
  /**************************************************************************/
  bool FileSystem::DirectoryListFileNames(std::string dirPath,
    std::vector<std::string>& fileNames)
  {
    if (boost::filesystem::exists(dirPath)) {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath))
        fileNames.push_back(file.path().filename().string());
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Scans a folder, given a path and extracts all the filePaths
  into a string vector.
  @param  The folder path.
  @return The string vector, containing all the files paths.
  */
  /**************************************************************************/
  bool FileSystem::DirectoryListFilePaths(std::string dirPath,
    std::vector<std::string>& filePaths) {

    if (boost::filesystem::exists(dirPath)) {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath)) {
        auto filePath = dirPath + file.path().filename().string();
        filePaths.push_back(filePath);
      }
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief Scans a folder, given a path and extracts all the files paths
         with a given extension into a string vector.
  @param dirPath The directory path.
  @param filePaths A vector containing all the file paths.
  @param extension The extension which to extract.
  @return The string vector, containing all the files paths.
  */
  /**************************************************************************/
  bool FileSystem::DirectoryListFilePaths(std::string dirPath, std::vector<std::string>& filePaths, std::string extension)
  {
    if (boost::filesystem::exists(dirPath)) {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath)) {        
        // Only add the path if it has the given extension
        if (file.path().extension().string() != extension)
          continue;

        auto filePath = file.path().filename().string();        
        auto relativeFilePath = dirPath + filePath;        
        filePaths.push_back(relativeFilePath);
      }
      return true;
    }
    return false;
  }


  /*!************************************************************************\
  @brief
  @param  The folder path.
  @return The string vector, containing all the files.
  \**************************************************************************/
  bool FileSystem::DirectoryListDirs(filepath dirPath,
    std::vector<filepath>& dirs)
  {

    if (boost::filesystem::exists(dirPath))
    {
      for (auto&& file : boost::filesystem::directory_iterator(dirPath))
        if (boost::filesystem::is_directory(file))
          dirs.push_back(file.path().filename());

      return true;
    }

    return false;
  }


  /*!************************************************************************\
  @brief  Recursively scans a folder looking for a specific filename. If it
  finds it, it will return the filepath.
  @param  The file name.
  @param  The folder path.
  @return A string containing the file path.
  \**************************************************************************/
  bool FileSystem::DirectoryContainsFile(filepath dirPath, std::string fileName, std::string& foundPath)
  {
    if (boost::filesystem::exists(dirPath))
    {
      const boost::filesystem::path file(fileName);
      // Create the recursive directory iterators to be used
      const boost::filesystem::recursive_directory_iterator end;
      const boost::filesystem::recursive_directory_iterator dirIter(dirPath);
      // Recursively search for the file within the folder
      const auto it = std::find_if(dirIter, end,
        [&file](const boost::filesystem::directory_entry& e)
      { return e.path().filename() == file && boost::filesystem::is_regular_file(e.path()); });

      if (it != end)
      {
        foundPath = it->path().string();
        return true;
      }
    }

    return false;

  }


  /*!************************************************************************\
  @brief  Counts the number of files in a given directory.
  @param  The directory path.
  @return The number of files found.
  \**************************************************************************/
  unsigned int FileSystem::DirectoryCountFiles(filepath dirPath)
  {
    if (boost::filesystem::exists(dirPath))
    {
      unsigned int count = static_cast<unsigned int>(std::count_if(boost::filesystem::directory_iterator(dirPath),
        boost::filesystem::directory_iterator(),
        bind(static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file),
          bind(&boost::filesystem::directory_entry::path,
            boost::lambda::_1))));
      return count;
    }

    return 0;
  }


  /*!************************************************************************\
  @brief  Counts the number of files in a given directory.
  @param  The directory path.
  @return The number of files found.
  \**************************************************************************/
  unsigned int FileSystem::DirectoryCountDirs(filepath dirPath)
  {
    if (boost::filesystem::exists(dirPath))
    {
      unsigned int count = static_cast<unsigned int>(std::count_if(boost::filesystem::directory_iterator(dirPath),
        boost::filesystem::directory_iterator(),
        bind(static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_directory),
          bind(&boost::filesystem::directory_entry::path,
            boost::lambda::_1))));
      return count;
    }

    return 0;
  }

  /*!************************************************************************\
  @brief  Creates a directory in the specified path.
  @param  The directory  path.
  @return The string containing the file's contents.
  @note   This operation is particularly useful for deserializing data.
  \**************************************************************************/
  bool FileSystem::DirectoryCreate(filepath dirPath)
  {
    if (boost::filesystem::create_directories(dirPath))
      return true;
    return false;
  }
}