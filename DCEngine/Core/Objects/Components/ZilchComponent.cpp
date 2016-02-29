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

  ZilchComponent::ZilchComponent(std::string name, ZilchFile scriptName, Entity & owner) : Component(name, owner), classScript(scriptName)
  {

    //if (DCE_TRACE_COMPONENT_CONSTRUCTOR)

  }

  ZilchComponent::~ZilchComponent()
  {
    if (Destroy != NULL)
    {
      ExecutableState* state = Systems::ZilchInterface::Get().GetState();
      Call call(Destroy, state);
      call.Set<Handle>(Call::This, classInstance);
      call.Invoke(report);
    }
    //delete classInstance;
  }
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
      initFunc = Interface->getFunction("Initialize", zilchClass, Array<Type*>(ZeroInit, ZilchTypeId(Entity)), ZilchTypeId(void), FindMemberOptions::None, true);
    }
    
    classInstance = Interface->AllocateDefaultConstructedHeapObject(zilchClass, HeapFlags::ReferenceCounted);
    Function* updateFunct = Interface->getFunction("OnLogicUpdate", zilchClass, Array<Type*>(ZeroInit, ZilchTypeId(Events::LogicUpdate)), ZilchTypeId(void), FindMemberOptions::None, true);
    if (initFunc != NULL)
    {
      //Connect(SpaceRef, Events::LogicUpdate, ZilchComponent::OnLogicUpdate);
      //Daisy->Connect<::DCEngine::Events::LogicUpdate>((Entity*)(SpaceRef), &ZilchComponent::CallConnections, this);
      Daisy->ZilchConnect<::DCEngine::Events::LogicUpdate>((Entity*)(SpaceRef), updateFunct, this);
      Call call(initFunc, Interface->GetState());
      call.Set<Handle>(Call::This, classInstance);
      call.Set<>(0, Owner());
      call.Invoke(report);
    }
    else
    {
      //THROW EXCEPTION HERE
    }
  }
  void ZilchComponent::LogicUpdate(::DCEngine::Events::LogicUpdate)
  {

  }
  /*void ZilchComponent::ZilchConnect(::DCEngine::Event Event, Entity* EntityConnection, Zilch::Function* funct)
  {

  }*/
  void ZilchComponent::CallConnections(::DCEngine::Event Event)
  {

  }
  /*void ZilchComponent::CallFunction(std::string functName)
  {
  Function* funct = zilchClass->FindFunction(functName.data(), Array<Type*>(), ZilchTypeId(String), FindMemberOptions::None);
  if (funct != NULL)
  {
  Call call(initFunc, ZILCHMANAGER->state);
  call.Set<Handle>(Call::This, classInstance);
  call.Set<>(0, GetOwner());
  call.Invoke(report);
  }
  }*/
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
