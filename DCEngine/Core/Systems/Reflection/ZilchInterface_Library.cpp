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
#include "ZilchInterfaceUtilities.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief Sets up an extension property on the specified type.
    @param type The type that is being passed in.
    @param baseType The type of property we are looking for.
    @param extensionType The type to which we are adding the extension property on.
    @param builder A pointer to the builder which is writing the properties.
    @param callback The callback function to call.
    */
    /**************************************************************************/
    bool ZilchInterface::SetupTypeProperty(Zilch::BoundType * type, Zilch::BoundType * baseType,
                                           Zilch::BoundType* extensionType, Zilch::BoundType* returnType,
                                           Zilch::LibraryBuilder * builder, ParseCallback callback, bool makeStatic)
    {
      if (Zilch::TypeBinding::IsA(type, baseType)) {
        Zilch::Property* componentProperty;        
        Zilch::MemberOptions::Enum option = Zilch::MemberOptions::None;
        // If the property is marked as static 
        if (makeStatic) {
          option = Zilch::MemberOptions::Static;
        }
        componentProperty = builder->AddExtensionProperty(extensionType, type->Name, returnType, nullptr, callback, option);
        // Construct data for the getter callback to used by the property
        ComponentData userData;
        userData.Type = type;
        userData.Interface = this;
        auto& data = componentProperty->Get->ComplexUserData.WriteObject(userData);
        return true;
      }
      return false;
    }

    /**************************************************************************/
    /*!
    @brief  Receives Zilch parse events and redirects them to extend properties.
    @param  event A pointer to the parse event.
    */
    /**************************************************************************/
    void ZilchInterface::TypeParsedErrorCallback(Zilch::ParseEvent* event) {
      // If the type being parsed is a Zilch component, attach it to Entity type
      ZilchInterface::Get().SetupTypeProperty(event->Type, ZilchComponent::ZilchGetStaticType(), ZilchTypeId(Entity), 
                                              event->Type, event->Builder, GetZilchComponent);
      // If the type parsed is a C++ component
      ZilchInterface::Get().SetupTypeProperty(event->Type, Component::ZilchGetStaticType(), ZilchTypeId(Entity), 
                                              event->Type, event->Builder, GetNativeComponent);
      // If the type being parsed is a ZilchEvent, attach it to the EventStrings type
      ZilchInterface::Get().SetupTypeProperty(event->Type, ZilchEvent::ZilchGetStaticType(), ZilchTypeId(EventStrings), 
                                              ZilchTypeId(Zilch::String), event->Builder, GetZilchEvent);
    }

    /**************************************************************************/
    /*!
    @brief Receives Zilch parse events and redirects them to extend properties.
    @param event A pointer to the parse event.
    */
    /**************************************************************************/
    void FreeTypeParserCallback(Zilch::ParseEvent* event) {
      bool parsed;
      // If the type being parsed is a Zilch component..
      parsed = ZilchInterface::Get().SetupTypeProperty(event->Type, ZilchComponent::ZilchGetStaticType(), 
                                                       ZilchTypeId(Entity), event->Type, event->Builder, GetZilchComponent);
      // If the type parsed is a C++ component
      if (!parsed)
      parsed = ZilchInterface::Get().SetupTypeProperty(event->Type, Component::ZilchGetStaticType(), 
                                                      ZilchTypeId(Entity), event->Type, event->Builder, GetNativeComponent);

      // If the type being parsed is a ZilchEvent, attach it to the EventStrings type
      if (!parsed) {
        parsed = ZilchInterface::Get().SetupTypeProperty(event->Type, ZilchEvent::ZilchGetStaticType(), 
                                                         ZilchTypeId(EventStrings), ZilchTypeId(Zilch::String), 
                                                         event->Builder, GetZilchEvent, true);
      }
    }
    
    void FreePreParserCallback(Zilch::ParseEvent* event) {
      // If the type being parsed is a Zilch component..
      ZilchInterface::Get().SetupTypeProperty(event->Type, ZilchTypeId(Component), ZilchTypeId(Entity), event->Type, event->Builder, GetNativeComponent);
    }

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
    @brief Configures various callback functions on the specified project.
    @param project A reference to the project.
    */
    /**************************************************************************/
    void ZilchInterface::SetupProject(Zilch::Project & project)
    {
      Zilch::EventConnect(&project, Zilch::Events::CompilationError, FreeCustomErrorCallback);
      Zilch::EventConnect(&project, Zilch::Events::TypeParsed, FreeTypeParserCallback);
      Zilch::EventConnect(&project, Zilch::Events::PreParser, FreePreParserCallback);

      //Zilch::EventConnect(&project, Zilch::Events::CompilationError, &ZilchInterface::CustomErrorCallback, this);      
      //Zilch::EventConnect(&project, Zilch::Events::TypeParsed, &ZilchInterface::TypeParsedErrorCallback, this);
    }

    /**************************************************************************/
    /*!
    @brief  Receives Zilch errors and redirects them to our logging system!
    @param  error A pointer to the error event.
    */
    /**************************************************************************/
    void ZilchInterface::CustomErrorCallback(Zilch::ErrorEvent * error)
    {
      std::string errorMessage = error->GetFormattedMessage(Zilch::MessageFormat::Zilch).c_str();
      // Redirect to our tracing system
      DCTrace << errorMessage;
    }


    /**************************************************************************/
    /*!
    @brief  Sets up the console for the ZilchInterface.
    */
    /**************************************************************************/
    void CustomWriteText(Zilch::ConsoleEvent* event) {
      DCTrace << event->Text.c_str();
    }
    void ZilchInterface::SetupConsole()
    {
      // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
      Zilch::EventConnect(&Zilch::Console::Events, Zilch::Events::ConsoleWrite, CustomWriteText);
      //Zilch::EventConnect(&Zilch::Console::Events, Zilch::Events::ConsoleWrite, Zilch::DefaultWriteText);
      // We can also setup the console so that any 'Read' functions will attempt to read from stdin
      Zilch::EventConnect(&Zilch::Console::Events, Zilch::Events::ConsoleRead, Zilch::DefaultReadText);
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
      Zilch::Project ScriptProject;
      // Here, we can register our own callback for when compilation errors occur
      // The default callback prints the file, line/character number, and message to stderr
      SetupProject(ScriptProject);

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

      // If not patching, clear dependencies and re-add the core libraries
      if (!Patching)
        SetupLibraries();

      // Compile all the code we have added together into a single library for our scripts
      auto tempLib = ScriptProject.Compile("ZilchScripts", Dependencies, Zilch::EvaluationMode::Project);      
        
      // If the script library was compiled successfully..
      if (tempLib) {
        if (Patching) {
          State->PatchLibrary(ScriptLibrary);
          DCTrace << "ZilchInterface::CompileScripts: Successfully patched the script library \n";
        }
        else {
          ScriptLibrary = tempLib;
          AddLibrary(ScriptLibrary);
          Build();
          DCTrace << "ZilchInterface::CompileScripts: Successfully rebuilt the executable state \n";
        }
        return true;
      }
      DCTrace << "ZilchInterface::CompileScripts: Failed to compile \n";
      return false;
    }


    /**************************************************************************/
    /*!
    @brief  Parse the engine's static libraries.
    */
    /**************************************************************************/
    void ZilchInterface::ParseStaticLibraries()
    {
      return;
      //auto builder = DCEngineCore::GetBuilder();
      //auto& boundTypes = DCEngineCore::GetLibrary()->BoundTypes.values();
      ////auto builder5 = DCEngineCore::GetInstance().GetBuilder();
      //ZilchForEach(auto boundType, boundTypes) {
      //  SetupTypeProperty(boundType, ZilchTypeId(Component), ZilchTypeId(Entity), builder, GetNativeComponent);
      //}
    }



    
  }
}