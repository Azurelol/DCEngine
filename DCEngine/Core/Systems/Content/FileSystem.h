/******************************************************************************/
/*!
@file   FileSystem.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/17/2015
@brief  A BOOST-powered interface static class that provides common and useful
        filesystem operations (such as scanning folders, etc)
@note   The Boost Filesystem library:
        http://www.boost.org/doc/libs/1_39_0/libs/filesystem/doc/index.htm
        Great reference for using the library:
        http://www.technical-recipes.com/2014/using-boostfilesystem/
*/
/******************************************************************************/
#pragma once
#include <string>
#include <vector>

namespace DCEngine {

  class FileSystem {
  public:

    /* File Operations */
    static bool FileFound(std::string filePath);
    static unsigned int FileSize(std::string filePath);
    static bool FileLastModified(std::string filePath, std::string& modifyData);
    static std::string FileExtractExtension(std::string& filePath); // Should have one to remove the extension
    static std::string FileExtractWithoutExtension(std::string& filePath);
    static bool FileRename(std::string filePath, std::string newFileName);   
    static bool FileCreate(std::string filePath);
    static bool FileReadToString(std::string filePath, std::string& output);
    static bool FileWriteString(std::string filePath, std::string& input);

    /* Folder Operations */
    static bool DirectoryExtractFilePaths(std::string dirPath, std::vector<std::string>& fileNames);
    static bool DirectoryFindFile(std::string fileName, std::string dirPath, std::string& filePath);
    static unsigned int DirectoryCountFiles(std::string dirPath);
    static bool DirectoryCreate(std::string dirPath);

    /* Path Operations*/
    static bool Remove(std::string path);
    // It would be good to restrict the 
  };

}