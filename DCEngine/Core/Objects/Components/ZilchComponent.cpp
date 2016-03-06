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
  @brief Returns the BoundType of this ZilchComponent.
  @return The boundtype of this component.
  */
  /**************************************************************************/
  Zilch::BoundType * ZilchComponent::BoundType()
  {
    return Daisy->getSystem<Systems::Reflection>()->Handler()->ScriptLibrary->BoundTypes.findValue(this->getObjectName().c_str(), nullptr);
  }

  /**************************************************************************/
  /*!
  @brief Initializes the ZilchComponent.
  */
  /**************************************************************************/
  void ZilchComponent::Initialize()
  {
    Systems::ZilchInterface* Interface = &Systems::ZilchInterface::Get();
    Zilch::LibraryRef library = Interface->getLibrary();
    if (library->BoundTypes.findValue(stdstringToZilchString(classScript), nullptr) == NULL)
    {
      return;
    }
    else
    {
      zilchClass = Interface->getBoundType(classScript, library);
      InitializeFunc = Interface->getFunction("Initialize", zilchClass, 
                      Zilch::Array<Zilch::Type*>(ZeroInit, ZilchTypeId(Entity)), ZilchTypeId(void), 
                      Zilch::FindMemberOptions::None, true);
    }
    //classInstance = Interface->AllocateDefaultConstructedHeapObject(zilchClass, HeapFlags::ReferenceCounted);
    Zilch::Function* updateFunct = Interface->getFunction("OnLogicUpdate", zilchClass, 
                                  Zilch::Array<Zilch::Type*>(ZeroInit, ZilchTypeId(Events::LogicUpdate)), 
                                  ZilchTypeId(void), Zilch::FindMemberOptions::None, true);
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

  /**************************************************************************/
  /*!
  @brief Serializes a ZilchComponent.
  @param builder A reference to the JSON builder.
  @note  This will serialize the component and all its properties.
  */
  /**************************************************************************/
  void ZilchComponent::Serialize(Zilch::JsonBuilder & builder)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    auto boundType = BoundType();
    auto handle = Handle();

    builder.Key(this->Name().c_str());
    builder.Begin(Zilch::JsonType::Object);

    //SerializeByType(builder, interface->GetState(), handle, boundType);
    //builder.End();
  }

  /**************************************************************************/
  /*!
  @brief Deserializes a ZilchComponent.
  @param builder A pointer to the object containing the properties.
  @note  This will deserialize the Component's properties.
  */
  /**************************************************************************/
  void ZilchComponent::Deserialize(Zilch::JsonValue * properties)
  {
    /*
    if (DCE_TRACE_COMPONENT_INITIALIZE)
      DCTrace << Owner()->Name() << "::" << ObjectName << "::Deserialize \n";*/
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    auto boundType = BoundType();

    DeserializeByType(properties, interface->GetState(), this, this->ZilchGetDerivedType());
  }


  void DCEngine::ZilchComponent::OnLogicUpdate(::DCEngine::Events::LogicUpdate * event)
  {
  }

  void ZilchComponent::CallConnections(::DCEngine::Event Event)
  {

  }
  
  Zilch::Function* ZilchComponent::GetFieldOrProperty(std::string functName)
  {
    Zilch::Property* field = zilchClass->FindPropertyOrField(functName.data(), Zilch::FindMemberOptions::None);
    if (field != NULL)
    {
      return field->Get;
    }
  }

  Zilch::Function* ZilchComponent::SetFieldOrProperty(std::string functName)
  {
    Zilch::Property* field = zilchClass->FindPropertyOrField(functName.data(), Zilch::FindMemberOptions::None);
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