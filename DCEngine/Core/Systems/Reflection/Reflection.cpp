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

      // Loop through every library
      for (auto library : Handler()->Dependencies.all()) {
        // Grab a container of all the bound types in the library
        auto types = library->BoundTypes.all();
        // For every type in the library
        ZilchForEach(auto type, types) {
          // If the type is a component
          if (Zilch::TypeBinding::IsA(type.second, Component::ZilchGetStaticType()))
            componentTypes.push_back(type.second);
        }
      }
      return componentTypes;
    }
    
    /**************************************************************************/
    /*!
    @brief  Constructs a Call object with access to the property's accessor method.
    @param  property A pointer to the property.
    @return A Zilch::Call object.
    */
    /**************************************************************************/
  // Zilch::Call DCEngine::Systems::Reflection::Get(Zilch::Property * property)
  // {

  // }

    std::string DCEngine::Systems::Reflection::PropertyAsString(Zilch::Property * property, ObjectPtr object)
    {
      // Create an exception report object
      Zilch::ExceptionReport report;
      // Grab the current resource
      Zilch::Call getCall(property->Get, Daisy->getSystem<Reflection>()->Handler()->getState());
      getCall.SetHandleVirtual(Zilch::Call::This, object);
      getCall.Invoke(report);
      return std::string(getCall.Get<Zilch::String>(Zilch::Call::Return).c_str());
    }

    ZilchInterface* Reflection::Handler()
    {
      return ZilchHandler.get();
    }




  }


}