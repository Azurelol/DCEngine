/******************************************************************************/
/*!
@file   ZilchInterface.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/17/2015
@brief  An interface for accessing the Zilch library the Daisy Chain engine
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ZilchInterface.h"

#include "../../Binding/CoreBinding.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    @brief  Constructor.
    @note   Any one time startup and static initialization Zilch needs to do
    This also registers a custom assertion handler (Zilch code has many
    user friendly asserts!)
    */
    /**************************************************************************/
    ZilchInterface::ZilchInterface() : Setup(Zilch::StartupFlags::DoNotShutdown), Patching(false)
    {
      DCTrace << "ZilchInterface::ZilchInterface - Constructor\n";
      
    }
    /**************************************************************************/
    /*!
    @brief  Destructor.
    */
    /**************************************************************************/
    ZilchInterface::~ZilchInterface()
    {
      // Clear the script library
      ScriptLibrary.Clear();
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to this system, Static member
    @return The reference to the single instance of this class
    */
    /**************************************************************************/
    ZilchInterface & ZilchInterface::Get()
    {
      return *Daisy->getSystem<Reflection>()->Handler();
    }
    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the executable state.
    @return The exclusive executable state for the interface.
    */
    /**************************************************************************/

    Zilch::ExecutableState * ZilchInterface::GetState()
    {
      return Daisy->getSystem<Reflection>()->Handler()->State;
    }


    /**************************************************************************/
    /*!
    @brief  Initializes the Zilch interface system.
    */
    /**************************************************************************/
    void ZilchInterface::Initialize()
    {
      DCTrace << "ZilchInterface::Initialize\n";
      SetupZilch();
    }

    /**************************************************************************/
    /*!
    @brief  Starts the Zilch environment.
    */
    /**************************************************************************/
    void ZilchInterface::SetupZilch()
    {
      // Set up the console
      SetupConsole();
      // Parse all static libraries
      ParseStaticLibraries();
      // Add our custom static library for binding our classes
      AddLibrary(DCEngineCore::GetInstance().GetLibrary());
      AddLibrary(Rebound::GetInstance().GetLibrary());
      // Add our custom library for our own Zilch scripts
      Zilch::Project ScriptProject;
      SetupProject(ScriptProject);        
      ScriptLibrary = ScriptProject.Compile("ZilchScripts", Dependencies, Zilch::EvaluationMode::Project);
      //AddLibrary(ScriptLibrary);
      // Compile and build the exclusive 'ExecutableState' object
      Build();
    }

    /**************************************************************************/
    /*!
    @brief  Sets up the main libraries used by the engine.
    */
    /**************************************************************************/
    void ZilchInterface::SetupLibraries()
    {
      Dependencies.clear();
      AddLibrary(Zilch::Core::GetInstance().GetLibrary());
      // Add the Core Library
      AddLibrary(DCEngineCore::GetInstance().GetLibrary());
      SetUpComponentTypes(DCEngineCore::GetInstance().GetLibrary());
      // Add the Rebound Library (Temporary)
      AddLibrary(Rebound::GetInstance().GetLibrary());      
      SetUpComponentTypes(Rebound::GetInstance().GetLibrary());
    }



    /**************************************************************************/
    /*!
    @brief  Terminates the ZilchInterface system.
    */
    /**************************************************************************/
    void ZilchInterface::Terminate()
    {
      DCTrace << "ZilchInterface::Terminate";
      Clean();
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
      Zilch::EventConnect(&Zilch:: Console::Events, Zilch::Events::ConsoleRead, Zilch::DefaultReadText);
    }



    /*!************************************************************************\
    @brief  Invokes the function.
    @param  A pointer to the function.
    @return An ExceptionError object which contains data regarding the script.
    \**************************************************************************/
    Zilch::Handle ZilchInterface::AllocateDefaultConstructedHeapObject(Zilch::BoundType * type, Zilch::HeapFlags::Enum flag)
    {
      // Allocate the player and call a default constructor if one is provided
      // It is legal to allocate Zilch objects that have no defined default constructor
      // The object will first be entirely set to zero (all null and zero values)
      // Then the pre-constructor will run, which initializes any members in the class to the value after the '='
      Zilch::ExceptionReport report;
      Zilch::Handle instance = State->AllocateDefaultConstructedHeapObject(type, report, flag);
      // Check the exception error

      return instance;
    }

    /*!************************************************************************\
    @brief  Creates a function object, which methods that can invoke it, etc.
    @param  A pointer to the function.
    @return An ExceptionError object which contains data regarding the script.
    \**************************************************************************/
    Zilch::Call* const ZilchInterface::Call(Zilch::Function* function) const
    {
      Zilch::ExceptionReport report;
      Zilch::Call* const call = new Zilch::Call(function, State);
      // Check the exception error

      return call;
    }




  }
}