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

#include "../../Systems/Reflection/ZilchInterface.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  

  /*!************************************************************************\
  @brief  Zilch Component Definition
  \**************************************************************************/
  ZilchDefineType(ZilchComponent, "ZilchComponent", DCEngineCore, builder, type) {
    // This sets Zilch's Handle manager setting
    //type->HandleManager = ZilchManagerId(Zilch::HandleManager);    
    //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
    // Constructor / Destructor    
    ZilchBindConstructor(builder, type, ZilchComponent, ZilchNoNames);
    ZilchBindConstructor(builder, type, ZilchComponent, "name, owner", std::string, Entity&);
    ZilchBindDestructor(builder, type, ZilchComponent);
    // Methods
    ZilchBindMethod(builder, type, &ZilchComponent::Initialize, ZilchNoOverload, "Initialize", ZilchNoNames)->IsVirtual = true;
    ZilchBindProperty(builder, type, &ZilchComponent::Owner, ZilchNoSetter, "Owner");
    ZilchBindProperty(builder, type, &ZilchComponent::getSpace, ZilchNoSetter, "Space");
    ZilchBindProperty(builder, type, &ZilchComponent::getGameSession, ZilchNoSetter, "GameSession");    
    ZilchBindProperty(builder, type, &ZilchComponent::getEngine, ZilchNoSetter, "Daisy");
  }

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
  @brief Finds all common functions to be passed to Zilch.
  */
  /**************************************************************************/
  void ZilchComponent::FindFunctions()
  {
    auto boundType = BoundType();
    // Find the Initialize function
    InitializeFunc = boundType->FindFunction("Initialize", Zilch::Array<Zilch::Type*>(), ZilchTypeId(void), Zilch::FindMemberOptions::None);
    // Attempt to find the OnLogicUpdate function
    OnLogicUpdateFunc = boundType->FindFunction("OnLogicUpdate", Zilch::Array<Zilch::Type*>(ZeroInit, ZilchTypeId(Events::LogicUpdate)), 
                                                                                            ZilchTypeId(void), Zilch::FindMemberOptions::None);
    // If it failed to find the event-based one, use the hard one...
    if (!OnLogicUpdateFunc) {
      OnLogicUpdateFunc = boundType->FindFunction("OnLogicUpdate", Zilch::Array<Zilch::Type*>(), ZilchTypeId(void), Zilch::FindMemberOptions::None);
      // If it succesfully found the default 'OnLogicUpdate'...
      if (OnLogicUpdateFunc)
        LogicUpdateDirectly = true;
    }
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
    if (Zilch::TypeBinding::IsA(component->ZilchGetDerivedType(), ZilchComponent::ZilchGetStaticType()))
      return true;
    return false;
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
    

    // Find all bound functions!
    FindFunctions();
    // If updating directly, subscribe through C++ Logic Update Events
    //if (LogicUpdateDirectly)
    //  Daisy->Connect<Events::LogicUpdate>(SpaceRef, &ZilchComponent::OnLogicUpdateEvent, this);

    // Invoke the Initialize Method
    Zilch::Call init(InitializeFunc, Systems::ZilchInterface::GetState());
    init.Set<Zilch::Handle>(Zilch::Call::This, Handle());
    init.Invoke(Report);
  }

  void ZilchComponent::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
    // Invoke the update method
    Zilch::Call update(OnLogicUpdateFunc, Systems::ZilchInterface::GetState());
    update.Set<Zilch::Handle>(Zilch::Call::This, Handle());
    update.Invoke(Report);
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
    SerializeByType(builder, interface->GetState(), boundType, this, handle);
    builder.End();
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
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    auto boundType = BoundType();
    auto handle = Handle();
    DeserializeByType(properties, interface->GetState(), boundType, this, handle);
  }
  
  void ZilchComponent::CallConnections(::DCEngine::Event Event)
  {
  }
  
  Zilch::Function* ZilchComponent::GetFieldOrProperty(std::string functName)
  {
    Zilch::Property* field = BoundType()->FindPropertyOrField(functName.data(), Zilch::FindMemberOptions::None);
    if (field != NULL)
    {
      return field->Get;
    }
  }

  Zilch::Function* ZilchComponent::SetFieldOrProperty(std::string functName)
  {
    Zilch::Property* field = BoundType()->FindPropertyOrField(functName.data(), Zilch::FindMemberOptions::None);
    if (field != NULL)
    {
      return field->Set;
    }
  }
  
  
}