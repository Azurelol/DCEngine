/*****************************************************************************/
/*!
@file   Reflection.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/16/2015
@brief  The reflection system provides
@note   The credit to my implementation of the Reflection system used in this
engine goes to Randy Gaul. More information in the implementation
header.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Reflection.h"

#include "../../Engine/Engine.h"


namespace DCEngine {
  
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Reflection system constructor.
    */
    /**************************************************************************/
   Reflection::Reflection() : System(std::string("ReflectionSystem"), EnumeratedSystem::Reflection) {
     DCTrace << "*Using Zilch to bind Object classes and provide reflection \n";
     ZilchHandler.reset(new ZilchInterface());
    }

   /**************************************************************************/
   /*!
   @brief  Initializes the Reflection system.
   */
   /**************************************************************************/
    void Reflection::Initialize()
    {
      ZilchHandler->Initialize();
      Subscribe();
    }

    /**************************************************************************/
    /*!
    @brief  Subscribes to events.
    */
    /**************************************************************************/
    void Reflection::Subscribe()
    {
      Daisy->Connect<Events::ScriptingCompileScripts>(&Reflection::OnScriptingCompileScriptsEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief  Updates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Update(float dt)
    {
    }

    /**************************************************************************/
    /*!
    @brief  Terminates the Reflection system.
    */
    /**************************************************************************/
    void Reflection::Terminate()
    {
      ZilchHandler->Terminate();
    }


    /**************************************************************************/
    /*!
    @brief  Returns a container containing all the bound components
    in the engine.

    */
    /**************************************************************************/
    std::vector<Zilch::BoundType*> Reflection::AllComponents()
    {
      std::vector<Zilch::BoundType*> componentTypes;

      // Loop through the script library for scripts      
      auto scriptLibrary = ZilchHandler->ScriptLibrary;
      if (scriptLibrary) {
        auto boundTypes = scriptLibrary->BoundTypes.all();
        ZilchForEach(auto type, boundTypes)
        {
          componentTypes.push_back(type.second);          
        }
      }

      // Load C++ components
      for (auto library : Handler()->Dependencies.all()) {
        // Grab a container of all the bound types in the library
        auto types = library->BoundTypes.all();
        // For every type in the library
        ZilchForEach(auto type, types) {      
          //// If the type is a Zilch component


          //if (Zilch::TypeBinding::IsA(type.second, ZilchComponent::ZilchGetStaticType()) )
          //  componentTypes.push_back(type.second);
          if (Zilch::TypeBinding::IsA(type.second, ZilchTypeId(Component)))
            componentTypes.push_back(type.second);
        }
      }
      return componentTypes;
    }
    


    /**************************************************************************/
    /*!
    @brief Returns the name of property as string.
    @param property A pointer to the property.
    @param object A pointer to the object.
    @return A string.
    */
    /**************************************************************************/
    std::string DCEngine::Systems::Reflection::PropertyAsString(Zilch::Property * property, ObjectPtr object)
    {
      // Create an exception report object
      Zilch::ExceptionReport report;
      // Grab the current resource
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      return std::string(getCall.Get<Zilch::String>(Zilch::Call::Return).c_str());
    }

    /**************************************************************************/
    /*!
    @brief Returns a reference to the underlying Zilch Interface.
    @return A reference to the Zilch Interface.
    */
    /**************************************************************************/
    ZilchInterface* Reflection::Handler()
    {
      return ZilchHandler.get(); 
    }

    /**************************************************************************/
    /*!
    @brief Compiles all currently loaded scripts.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Reflection::OnScriptingCompileScriptsEvent(Events::ScriptingCompileScripts * event)
    {
      // Inform that it's been done
      if (Handler()->CompileScripts())
        DispatchSystemEvents::ScriptingLibraryPatched();
    }


  }
}