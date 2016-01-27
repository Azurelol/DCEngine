/******************************************************************************/
/*!
@file   MenuLaunch.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/26/2016
@brief  This file includes the implementation for the Editor's project menu.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/

#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Opens the folder where the project is stored.
    */
    /**************************************************************************/
    void Editor::LaunchProjectFolder()
    {
      // Open the projects path
      auto executablePath = boost::filesystem::initial_path().string();
      auto projectPath = "\\" + Settings.ProjectInfo->ProjectPath;
      FileSystem::CorrectPath(projectPath);
      // If the projects directory doesn't exist, start from the executable      
      if (FileSystem::Exists(executablePath + projectPath)) {
        std::string qm = "\"";
        std::string command;
        command = qm + executablePath + projectPath + qm;
        // Launch!
        FileSystem::Execute(command);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Opens the documentation page.
    */
    /**************************************************************************/
    void Editor::LaunchDocumentation()
    {
      // Open the projects path
      auto executablePath = boost::filesystem::initial_path().string();
      auto documentationPath = "\\Documentation\\html\\index.html";
      // If the projects directory doesn't exist, start from the executable      
      if (FileSystem::Exists(executablePath + documentationPath)) {
        std::string qm = "\"";
        std::string command;
        command = qm + executablePath + documentationPath + qm;
        // Launch!
        FileSystem::Execute(command);
      }
    }

  }
}