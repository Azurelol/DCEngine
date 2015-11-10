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
     

      /* Getters */
      Zilch::ExecutableState* getState();
      Zilch::BoundType* getBoundType(std::string name, Zilch::LibraryRef library);
      Zilch::Function* getFunction(std::string name, Zilch::BoundType* type, const Zilch::Array<Zilch::Type*>& parameters, Zilch::Type* returnType, Zilch::FindMemberOptions::Flags options);
      Zilch::Field* getInstanceField(std::string name, Zilch::BoundType* type);

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
      void Update(float dt);
      void Terminate();

    };



  }
}