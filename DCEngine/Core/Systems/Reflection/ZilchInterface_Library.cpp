/******************************************************************************/
/*!
@file   ZilchInterfaceLibrary.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ZilchInterface.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Receives Zilch errors and redirects them to our logging system!
    @param  error A pointer to the error event.
    */
    /**************************************************************************/
    void FreeCustomErrorCallback(Zilch::ErrorEvent * error)
    {
      std::string errorMessage = error->GetFormattedMessage(Zilch::MessageFormat::Zilch).c_str();
      // Redirect to our tracing system
      DCTrace << errorMessage;  
      // Send an event containing the message to the engine
      DCEngine::Systems::DispatchSystemEvents::ScriptingErrorMessage(errorMessage);
    }

    /**************************************************************************/
    /*!
    @brief  Adds a Zilch script to the Zilch interface.
    @param  fileName The name of the file containing the code.
    @return The success of the operation.
    */
    /**************************************************************************/
    void ZilchInterface::AddScriptFile(std::string fileName)
    {
      auto iter = std::find(ScriptFiles.begin(), ScriptFiles.end(), fileName);
      if (iter != ScriptFiles.end())
        return;

      ScriptFiles.push_back(fileName);
      DCTrace << "ZilchInterface::AddScriptFile: Added '" << fileName << "' \n";
    }

    void ZilchInterface::AddScript(std::string code, std::string origin)
    {
      Scripts.push_back(ZilchScriptInfo(origin, code));
      DCTrace << "ZilchInterface::AddScript: Added '" << origin << "' \n";
    }

    /**************************************************************************/
    /*!
    @brief  Adds a Zilch script to a Zilch Project.
    @param  fileName The name of the file containing the code.
    @return The success of the operation.
    */
    /**************************************************************************/
    bool ZilchInterface::AddCodeFromFile(std::string fileName, Zilch::Project& project)
    {
      DCTrace << "ZilchInterface::AddCodeFromFile - Adding script: " << fileName << "\n";
      bool success = project.AddCodeFromFile(fileName.c_str());
      ErrorIf(!success, std::string("AddCodeFromFile - Failed to add script:  '" + fileName + "'").c_str());
      return success;
    }

    /**************************************************************************/
    /*!
    @brief  Adds a Zilch script to a Zilch Project.
    @param  code The string containing the code.
    @param  displayName The display name of the code.
    @return The success of the operation.
    */
    /**************************************************************************/
    void ZilchInterface::AddCodeFromString(std::string displayName, std::string code, Zilch::Project& project)
    {
      DCTrace << "ZilchInterface::AddCodeFromString - Adding script: " << displayName << "\n";
      project.AddCodeFromString(code.c_str(), displayName.c_str());
    }


    /**************************************************************************/
    /*!
    @brief  Adds a Zilch library to the interface's dependencies module. 
            It links together all core libraries, along our own libraries.
    @param  library A reference to the library to add.
    @return The success of the operation.
    */
    /**************************************************************************/
    void ZilchInterface::AddLibrary(Zilch::LibraryRef & library)
    {
      DCTrace << "ZilchInterface::AddLibrary - Adding library: " << library->Name.c_str() << "\n";
      ErrorIf(library == nullptr, "ZilchInterface::AddLibrary - Failed to add library!");
      Dependencies.push_back(library);
    }

    /**************************************************************************/
    /*!
    @brief  Compiles all our scripts into a single Zilch library.
    @param  library The name of the Library.
    */
    /**************************************************************************/
    bool ZilchInterface::CompileScripts()
    {      
      //DCTrace << "ZilchInterface::CompileScripts - Compiling the script library: \n";
      // A project contains all of the code we combine together to make a single
      // Zilch library. The project also sends events for compilation errors that occur
      // (Including friendly!?? asserts)
      Zilch::Project ScriptProject;

      // Here, we can register our own callback for when compilation errors occur
      // The default callback prints the file, line/character number, and message to stderr
      Zilch::EventConnect(&ScriptProject, Zilch::Events::CompilationError, FreeCustomErrorCallback);

      // Add code from the scripts stored so far
      for (auto script : Scripts) {
        AddCodeFromString(script.Name, script.Code, ScriptProject);
        //DCTrace << "ZilchInterface::CompileScripts - Adding: '" << script.first << "' \n";
      }

      // Add code from script files stored so far
      for (auto script : ScriptFiles) {
        AddCodeFromFile(script, ScriptProject);
        //DCTrace << "ZilchInterface::CompileScripts - Adding: '" << script.first << "' \n";
      }

      // Compile all the code we have added together into a single library for our scripts
      ScriptLibrary = ScriptProject.Compile("ZilchScripts", Dependencies, Zilch::EvaluationMode::Project);
      // Link together all the libraries that we depend upon
        
      if (ScriptLibrary) {
        State->PatchLibrary(ScriptLibrary);
        DCTrace << "ZilchInterface::CompileScripts: Successfully patched the script library \n";
        return true;
      }
      DCTrace << "ZilchInterface::CompileScripts: Failed to compile \n";
      return false;

      // Build 
      //Build();
    }

    /**************************************************************************/
    /*!
    @brief  Compiles and links all the libraries into one executable state.
    @param  A reference to the current Zilch project object.
    */
    /**************************************************************************/
    void ZilchInterface::Build()
    {
      DCTrace << "ZilchInterface::Build - Linking and compiling the executable state! \n";
      // Link all the libraries together into one executable state
      State = Dependencies.Link();
      ErrorIf(State == nullptr, "Failed to link libraries together");
    }

    /**************************************************************************/
    /*!
    @brief  Clears the interface's Zilch 'state' as well as the 'dependencies'
            module.
    */
    /**************************************************************************/
    void ZilchInterface::Clean()
    {
      DCTrace << "ZilchInterface::Clean - Freeing the state, report, dependencies... \n";
      // Free the State's dynamically allocated memory
      if (State) {
        delete State;
        State = nullptr;
      }
      // Clear the 'Exception' report
      Report.Clear();
      // Clear the 'Dependencies' container
      Dependencies.clear();
      // Re-include Zilch's core libraries
      Dependencies.push_back(Zilch::Core::GetInstance().GetLibrary());
    }
    
  }
}