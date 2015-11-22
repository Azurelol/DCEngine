/******************************************************************************/
/*!
@file   ZilchInterface.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/17/2015
@brief  An interface for accessing the Zilch library the Daisy Chain engine
        uses.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <ZILCH\Zilch.hpp>
#include "../../Binding/CoreBindingObjects.h"
#include "../../Debug/Debug.h"
#include <unordered_map>


namespace DCEngine {
  namespace Systems {
    
    using ZilchOrigin = std::string;
    using ZilchFile = std::string;
    using ZilchCode = std::string;

    class Reflection;
    class ZilchInterface {
      friend class Reflection;

    public:

      ~ZilchInterface();

      /* Code */
      bool AddCodeFromFile(std::string fileName, Zilch::Project& project);
      void AddCodeFromString(std::string code, std::string origin, Zilch::Project& project);
      void AddLibrary(Zilch::LibraryRef& library);
      void CompileScripts(std::string libraryName);
      void Build();
      void Clean();
     
      /* Zilch::Call operations */
      template <typename ObjectPtr>
      Zilch::Call ConstructGetCaller(Zilch::Property* property, ObjectPtr object);
      template <typename Value, typename ObjectPtr>
      bool SetProperty(Value val, Zilch::Property* property, ObjectPtr object);

      /* Getters */
      Zilch::ExecutableState* getState();
      Zilch::BoundType* getBoundType(std::string name, Zilch::LibraryRef library);
      Zilch::Function* getFunction(std::string name, Zilch::BoundType* type, const Zilch::Array<Zilch::Type*>& parameters, Zilch::Type* returnType, Zilch::FindMemberOptions::Flags options);
      Zilch::Field* getInstanceField(std::string name, Zilch::BoundType* type);
      std::vector<Zilch::BoundType*> GetTypes();

      /* Type constructors */
      Zilch::Handle AllocateDefaultConstructedHeapObject(Zilch::BoundType* type, Zilch::HeapFlags::Enum);
      Zilch::Call* const Call(Zilch::Function* function) const;

    private:

      void Test();

      /* Member variables */
      Zilch::ZilchSetup Setup;
      Zilch::ExecutableState* State;
      Zilch::ExceptionReport Report;
      Zilch::LibraryRef ScriptLibrary;
      // A container of dependent libraries
      Zilch::Module Dependencies;

      std::unordered_map<ZilchOrigin, ZilchCode> Scripts;
      std::unordered_map<ZilchFile , bool> ScriptFiles;

      /* Base methods */
      void SetupConsole();
      ZilchInterface();
      void Initialize();
      void SetupZilch();
      void Update(float dt);
      void Terminate();

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