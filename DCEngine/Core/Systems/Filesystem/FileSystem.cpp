#include "FileSystem.h"

#include <ctime>
#include <algorithm>
#include <BOOST/filesystem.hpp>
#include <BOOST/lambda/bind.hpp>


namespace DCEngine {
    

  /**************************************************************************/
  /*!
  @brief  Looks for a file in the specified file path.
  @param  The name of the file.
  @return Whether the file was found or not.
  @note   The user needs to supply the file path relative to where the executable
          is placed. For VS projects, this will be the root folder.
  */
  /**************************************************************************/
  bool FileSystem::FileFound(std::string filePath)
  {
    if (boost::filesystem::exists(filePath))
      return true;
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
  bool FileSystem::DirectoryExtractFileNames(std::string dirPath, 
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
  bool FileSystem::DirectoryExtractFilePaths(std::string dirPath, 
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
  @brief  Recursively scans a folder looking for a specific filename. If it
          finds it, it will return the filepath. Otherwise, it will return
          NULL. 
  @param  The file name.
  @param  The folder path.
  @return A string containing the file path.
  */
  /**************************************************************************/
  bool FileSystem::DirectoryFindFile(std::string fileName, std::string folderPath, std::string& filePath)
  {
    
    // Create a path object for the file that is being searched
    const boost::filesystem::path dirPath(folderPath);

    if (boost::filesystem::exists(dirPath)) {

      const boost::filesystem::path file(fileName);
      // Create the recursive directory iterators to be used
      const boost::filesystem::recursive_directory_iterator end;
      const boost::filesystem::recursive_directory_iterator dirIter(dirPath);
      // Recursively search for the file within the folder
      const auto it = std::find_if(dirIter,
        end,
        [&file](const boost::filesystem::directory_entry& e) {
        return e.path().filename() == file;
      });

      if (it != end) {
        filePath = it->path().string();
        return true;
      }
    }

    return false;

  }

  /**************************************************************************/
  /*!
  @brief  Counts the number of files in a given directory.
  @param  The directory path.
  @return The number of files found.
  */
  /**************************************************************************/
  unsigned int FileSystem::DirectoryCountFiles(std::string dirPath)
  {
    boost::filesystem::path dir(dirPath);

    if (boost::filesystem::exists(dir)) {
      unsigned int count = static_cast<unsigned int>(std::count_if(boost::filesystem::directory_iterator(dir),
        boost::filesystem::directory_iterator(),
        bind(static_cast<bool(*)(const boost::filesystem::path&)>(boost::filesystem::is_regular_file),
          bind(&boost::filesystem::directory_entry::path,
            boost::lambda::_1))));
      return count;
    }

    return 0;
  }
  

  /**************************************************************************/
  /*!
  @brief  Creates a directory in the specified path.
  @param  The directory  path.
  @return The string containing the file's contents.
  @note   This operation is particularly useful for deserializing data.
  */
  /**************************************************************************/
  bool FileSystem::DirectoryCreate(std::string dirPath)
  {
    const boost::filesystem::path dir(dirPath);
    if (boost::filesystem::create_directories(dir))
      return true;
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Returns the size of the file, in bytes.  
  @param  The file path.
  @return The size of the file in bytes, an unsigned integer.
  */
  /**************************************************************************/
  unsigned int FileSystem::FileSize(std::string filePath)
  {
    if (boost::filesystem::exists(filePath)) {
      return static_cast<unsigned int>(boost::filesystem::file_size(filePath));
    }
    return NULL;
  }

  /**************************************************************************/
  /*!
  @brief  Finds out when a file was last modified.
  @param  The file path.
  @return The last time the file was modified, an std::time_t object.
  */
  /**************************************************************************/
  bool FileSystem::FileLastModified(std::string filePath, std::string& modifyData)
  {
    boost::filesystem::path file(filePath);
    if (boost::filesystem::exists(file)) {
      std::time_t time = boost::filesystem::last_write_time(file);
      modifyData = ctime(&time);
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Extracts the extension from a string containing a file path.
  @param  The file path.
  @return A new string without the extension.
  @todo   CURRENTLY NOT DOING AS ADVERTISED.
  */
  /**************************************************************************/
  std::string FileSystem::FileExtractExtension(std::string & filePath)
  {
    boost::filesystem::path file(filePath);
    if (boost::filesystem::exists(file)) {
      return file.stem().string();
    }
    return std::string();
  }

  /**************************************************************************/
  /*!
  @brief  Extracts the extension from a string containing a file path.
  @param  The file path.
  @return A new string without the extension.
  */
  /**************************************************************************/
  std::string FileSystem::FileExtractWithoutExtension(std::string & filePath)
  {
    boost::filesystem::path file(filePath);
    if (boost::filesystem::exists(file)) {
      return file.stem().string();
    }
    return std::string();
  }

  /**************************************************************************/
  /*!
  @brief  Renames a file.
  @param  The file path.
  @param  The new filename.
  @return A boolean determining the success of the operation.
  */
  /**************************************************************************/
  bool FileSystem::FileRename(std::string filePath, std::string newFileName)
  {
    boost::filesystem::path oldPath(filePath);
    const boost::filesystem::path newPath = oldPath.root_directory().string() + newFileName;
    if (boost::filesystem::exists(oldPath)) {
      boost::filesystem::rename(oldPath, newPath);
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Creates a file.
  @param  The new file, in its path.
  @return A boolean determining the success of the operation.
  */
  /**************************************************************************/
  bool FileSystem::FileCreate(std::string filePath)
  {
    std::ofstream newFile(filePath);
    if (newFile) {
      newFile.close();
      return true;
    }      
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Removes a file or directory.
  @param  The file path.
  @param  The new filename.
  @return A boolean determining the success of the operation.
  */
  /**************************************************************************/
  bool FileSystem::Remove(std::string filePath)
  {
    const boost::filesystem::path path(filePath);
    if (boost::filesystem::exists(path)) {
      boost::filesystem::remove_all(path);
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Extracts a file's contents into a single string. 
  @param  The file path.
  @return The string containing the file's contents.
  @note   This operation is particularly useful for deserializing data.
  */
  /**************************************************************************/
  bool FileSystem::FileReadToString(std::string filePath, std::string& output)
  {
    std::ifstream inputFile(filePath);
    if (inputFile) {
      std::string content((std::istreambuf_iterator<char>(inputFile)),
        (std::istreambuf_iterator<char>()));
      output = content;
      return true;
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  Writes to a file.
  @param  The new file, in its path.
  @return A boolean determining the success of the operation.
  */
  /**************************************************************************/
  bool FileSystem::FileWriteString(std::string filePath, std::string& input)
  {
    std::ofstream file(filePath);
    if (file) {
      file << input;
      file.close();
      return true;
    }
    return false;
  }

}