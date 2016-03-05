/*****************************************************************************/
/*!
\file   ZilchComponent.cpp
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   9/22/2015
\brief  Zilch components are used by gameplay programmers to write game logic into.
These components, alongside events, drive the logic of a game project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ZilchComponent.h"
#include "Precompiled.h"
#include "../../Systems/Reflection/ZilchInterface.h"
#include "../../Engine/Engine.h"

namespace DCEngine {

  using namespace Zilch;

  /**************************************************************************/
  /*!
  @brief ZilchComponent constructor.
  @param name The name of the component.
  @param owner A reference to the owner of this component.
  */
  /**************************************************************************/
  ZilchComponent::ZilchComponent(std::string name, Entity & owner) : Component(name, owner)
  {
  }
  
  /**************************************************************************/
  /*!
  @brief ZilchComponent default constructor
  */
  /**************************************************************************/
  ZilchComponent::ZilchComponent()
  {
  }
  
  /**************************************************************************/
  /*!
  @brief ZilchComponent destructor.
  */
  /**************************************************************************/
  ZilchComponent::~ZilchComponent()
  {
  }

  /**************************************************************************/
  /*!
  @brief Checks if this is a Zilch Component.
  @param component A pointer to the component.
  @return True if a Zilch component, false otherwise.
  */
  /**************************************************************************/
  bool ZilchComponent::IsZilchComponent(ComponentPtr component)
  {
    if (Zilch::TypeBinding::IsA(component->ZilchGetStaticType(), ZilchComponent::ZilchGetStaticType()))
      return false;
    return true;
  }

  /**************************************************************************/
  /*!
  @brief Initializes the ZilchComponent.
  */
  /**************************************************************************/
  void ZilchComponent::Initialize()
  {
    Systems::ZilchInterface* Interface = &Systems::ZilchInterface::Get();
    LibraryRef library = Interface->getLibrary();
    if (library->BoundTypes.findValue(stdstringToZilchString(classScript), nullptr) == NULL)
    {
      return;
    }
    else
    {
      zilchClass = Interface->getBoundType(classScript, library);
      InitializeFunc = Interface->getFunction("Initialize", zilchClass, Array<Type*>(ZeroInit, ZilchTypeId(Entity)), ZilchTypeId(void), FindMemberOptions::None, true);
    }
    //classInstance = Interface->AllocateDefaultConstructedHeapObject(zilchClass, HeapFlags::ReferenceCounted);
    Function* updateFunct = Interface->getFunction("OnLogicUpdate", zilchClass, Array<Type*>(ZeroInit, ZilchTypeId(Events::LogicUpdate)), ZilchTypeId(void), FindMemberOptions::None, true);
    if (InitializeFunc != NULL)
    {
      //Connect(SpaceRef, Events::LogicUpdate, ZilchComponent::OnLogicUpdate);
      //Daisy->Connect<::DCEngine::Events::LogicUpdate>((Entity*)(SpaceRef), &ZilchComponent::CallConnections, this);
     /* Daisy->ZilchConnect<::DCEngine::Events::LogicUpdate>((Entity*)(SpaceRef), updateFunct, this);
      Call call(InitializeFunc, Interface->GetState());
      call.Set<Handle>(Call::This, classInstance);
      call.Set<>(0, Owner());
      call.Invoke(report);*/
    }
    else
    {
      //THROW EXCEPTION HERE
    }
  }

  void DCEngine::ZilchComponent::OnLogicUpdate(::DCEngine::Events::LogicUpdate * event)
  {
  }

  void ZilchComponent::CallConnections(::DCEngine::Event Event)
  {

  }
  
  Function* ZilchComponent::GetFieldOrProperty(std::string functName)
  {
    Property* field = zilchClass->FindPropertyOrField(functName.data(), FindMemberOptions::None);
    if (field != NULL)
    {
      return field->Get;
    }
  }
  Function* ZilchComponent::SetFieldOrProperty(std::string functName)
  {
    Property* field = zilchClass->FindPropertyOrField(functName.data(), FindMemberOptions::None);
    if (field != NULL)
    {
      return field->Set;
    }
  }



  Zilch::Handle ZilchComponent::getInstance()
  {
    return classInstance;
  }

  
}