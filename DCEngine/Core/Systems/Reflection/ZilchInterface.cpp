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
    ZilchInterface::ZilchInterface() : Setup(Zilch::StartupFlags::None)
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
      // Add our custom static library for binding our classes
      AddLibrary(DCEngineCore::GetInstance().GetLibrary());
      // Add our custom library for our own Zilch scripts
      Zilch::Project ScriptProject;
      Zilch::EventConnect(&ScriptProject, Zilch::Events::CompilationError, Zilch::DefaultErrorCallback);
      ScriptLibrary = ScriptProject.Compile("ZilchScripts", Dependencies, Zilch::EvaluationMode::Project);
      //AddLibrary(ScriptLibrary);
      // Compile and build the exclusive 'ExecutableState' object
      Build();
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


    void CustomWriteText(Zilch::ConsoleEvent* event) {
      DCTrace << event->Text.c_str();
    }
    /**************************************************************************/
    /*!
    @brief  Sets up the console for the ZilchInterface.
    */
    /**************************************************************************/
    void ZilchInterface::SetupConsole()
    {
      // Setup the console so that when we call 'Console.WriteLine' it outputs to stdio
      Zilch::EventConnect(&Zilch::Console::Events, Zilch::Events::ConsoleWrite, CustomWriteText);
      //Zilch::EventConnect(&Zilch::Console::Events, Zilch::Events::ConsoleWrite, Zilch::DefaultWriteText);
      // We can also setup the console so that any 'Read' functions will attempt to read from stdin
      Zilch::EventConnect(&Zilch:: Console::Events, Zilch::Events::ConsoleRead, Zilch::DefaultReadText);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the Library.
    @return The current Script Library Reference for the interface.
    */
    /**************************************************************************/
    Zilch::LibraryRef ZilchInterface::getLibrary()
    {
      return ScriptLibrary;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the Boundtype of the specified object 
            from the specified Zilch library.
    @param  typeName The name of the type.
    @param  library A reference to the library.
    @return A pointer to the bound type.
    */
    /**************************************************************************/
    Zilch::BoundType * ZilchInterface::getBoundType(std::string typeName, Zilch::LibraryRef library)
    {
      Zilch::BoundType* type = library->BoundTypes.findValue(typeName.c_str(), nullptr);
      ErrorIf(type == nullptr, std::string("Failed to find a Zilch type named '" + typeName + "'").c_str());
      return type;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the specified function on the object.
    @param  typeName The name of the function.
    @param  type The BoundType.
    @param  parameters An array specifying the arguments of the function.
    @param  returnType The return type of the function.
    @return A pointer to the function.
    */
    /**************************************************************************/
    Zilch::Function * ZilchInterface::getFunction(std::string name, Zilch::BoundType* type,
                                                  const Zilch::Array<Zilch::Type*>& parameters,
                                                  Zilch::Type* returnType,
                                                  Zilch::FindMemberOptions::Flags options,
                                                  bool ErrorOn)

    {
      Zilch::Function* function = type->FindFunction(name.c_str(),
        parameters,
        returnType,
        options);

      auto typeName = type->Name.c_str();
      ErrorIf(function == nullptr, std::string("Failed to find a function named '" + name + "' on" + typeName).c_str());
      return function;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the specified instance field of the specified
    BoundType.
    @param  typeName The name of the instnace.
    @param  type The BoundType.
    @return A pointer to the function.
    */
    /**************************************************************************/
    Zilch::Field * ZilchInterface::getInstanceField(std::string name, Zilch::BoundType* type)
    {
      Zilch::Field* field = type->InstanceFields.findValue(name.c_str(), nullptr);
      auto typeName = type->Name.c_str();
      ErrorIf(field == nullptr, std::string("Failed to find '" + name + "' on " + typeName).c_str());
      return field;
    }

    /*!************************************************************************\
    @brief  Retrieves an attribute from a property by name.
    @param property A pointer to the property.
    @param attributeName The name of the attribute which to retrieve.
    @return A pointer to the attribute.
    \**************************************************************************/
    Zilch::Attribute * ZilchInterface::getAttribute(Zilch::Property * property, std::string attributeName)
    {
      for (auto& attribute : property->Attributes.all()) {
        if (attribute.Name == attributeName.c_str() )
          return &attribute;
      }
    }

    /*!************************************************************************\
    @brief  Gets all the currently bound types.
    @return A vector containing all the bound types.
    \**************************************************************************/
    std::vector<Zilch::BoundType*> ZilchInterface::GetTypes()
    {
      std::vector<Zilch::BoundType*> allTypes;

      // Loop through every library
      for (auto library : this->Dependencies.all()) {
         // Grab a container of all the bound types in the library
        auto types = library->BoundTypes.all();
        // For every type in the library
        ZilchForEach(auto type, types) {
         
        }
      }

      return std::vector<Zilch::BoundType*>();
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


    /*!************************************************************************\
    @brief  Testing!
    \**************************************************************************/
    void ZilchInterface::Test()
    {
      //auto objectType = DCEngineCore::GetLibrary()->BoundTypes.all();
    }



  }
}