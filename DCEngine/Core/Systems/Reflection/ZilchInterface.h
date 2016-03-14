/******************************************************************************/
/*!
@file   ZilchInterface.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/17/2015
@brief  An interface for accessing the Zilch library the Daisy Chain engine
        uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <ZILCH\Zilch.hpp>
#include "../../Binding/CoreBindingObjects.h"
#include "../../Debug/Debug.h"

namespace DCEngine {
  namespace Systems {
    
    class Reflection;
    class ZilchInterface {
      friend class Reflection;
    public:
      using ParseCallback = void(*)(Zilch::Call&, Zilch::ExceptionReport&);

      
      ~ZilchInterface();
      Zilch::LibraryRef ScriptLibrary;
      static ZilchInterface& Get();
      static Zilch::ExecutableState *GetState();

      // Code
      void AddScriptFile(std::string fileName);
      void AddScript(std::string code, std::string origin);
      bool AddCodeFromFile(std::string fileName, Zilch::Project& project);
      void AddCodeFromString(std::string code, std::string origin, Zilch::Project& project);
      void AddLibrary(Zilch::LibraryRef& library);
      bool CompileScripts();
      void Build();
      void Clean();
      void SetupTypeProperty(Zilch::BoundType* type, Zilch::BoundType* baseType, Zilch::BoundType* extensionType, 
                             Zilch::LibraryBuilder* builder, ParseCallback callback);
      // JSON
      Zilch::JsonValue ParseJSON(std::string& string);
      // Zilch::Call
      template <typename ObjectPtr>
      Zilch::Call ConstructGetCaller(Zilch::Property* property, ObjectPtr object);
      template <typename Value, typename ObjectPtr>
      bool SetProperty(Value val, Zilch::Property* property, ObjectPtr object);
      // Getters
      Zilch::LibraryRef getLibrary();
      Zilch::BoundType* getBoundType(std::string name, Zilch::LibraryRef library);
      Zilch::Function* getFunction(std::string name, Zilch::BoundType* type, const Zilch::Array<Zilch::Type*>& parameters,
      Zilch::Type* returnType, Zilch::FindMemberOptions::Flags options, bool ErrorOn);
      Zilch::Field* getInstanceField(std::string name, Zilch::BoundType* type);
      std::vector<Zilch::BoundType*> GetTypes();
      Zilch::Attribute* getAttribute(Zilch::Property* property, std::string attributeName);
      Zilch::Handle AllocateDefaultConstructedHeapObject(Zilch::BoundType* type, Zilch::HeapFlags::Enum);
      Zilch::Call* const Call(Zilch::Function* function) const;

    private:
      // State
      bool Patching;
      Zilch::ZilchSetup Setup;
      Zilch::ExecutableState* State;
      Zilch::ExceptionReport Report;
      Zilch::Module Dependencies;
      // Scripts
      struct ZilchScriptInfo {
        std::string Name;
        std::string Code;
        ZilchScriptInfo(const std::string& name, const std::string& code) 
                        : Name(name), Code(code) {}
      };
      std::vector<ZilchScriptInfo> Scripts;
      std::vector<std::string> ScriptFiles;

      // Configuration
      void SetupConsole();
      void Terminate();
      void SetupProject(Zilch::Project& project);
      ZilchInterface();
      void Initialize();
      void SetupZilch();
      void SetupLibraries();
      void SetUpComponentTypes(Zilch::LibraryRef library);

      void TypeParsedErrorCallback(Zilch::ParseEvent* event);
      void CustomErrorCallback(Zilch::ErrorEvent* error);
    };

    template<typename ObjectPtr>
    inline Zilch::Call ZilchInterface::ConstructGetCaller(Zilch::Property * property, ObjectPtr object)
    {
      Zilch::Call getCaller(property->Get, State);
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      return getCaller;
    }

    template<typename Value, typename ObjectPtr>
    inline bool ZilchInterface::SetProperty(Value val, Zilch::Property * property, ObjectPtr object)
    {     
      // Construct the Call object
      Zilch::Call setCall(property->Set, State);
      setCall.SetHandleVirtual(Zilch::Call::This, object);
      setCall.Set(0, val);
      // Create an exception report object to check for errors
      Zilch::ExceptionReport report;
      setCall.Invoke(report);
      return true;
    }

  }
}