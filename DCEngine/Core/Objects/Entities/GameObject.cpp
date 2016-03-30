/******************************************************************************/
/*!
\file   GameObject.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The Game Object composition class. Spaces are comprised of GameObject
entities.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "GameObject.h"

#include "Space.h"
#include "GameSession.h"
// ENGINE ADDED :<
#include "../../Engine/Engine.h"

namespace DCEngine {
    
  /*!************************************************************************\
  @brief  GameObject Definition
  \**************************************************************************/
  ZilchDefineType(GameObject, "GameObject", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    // Constructor / Destructor
    ZilchBindDestructor(builder, type, GameObject);
    ZilchBindConstructor(builder, type, GameObject, "name, space, gamesession", std::string, Space&, GameSession&);
    ZilchBindConstructor(builder, type, GameObject, ZilchNoNames);
    // Methods
    ZilchBindMethod(builder, type, &GameObject::Destroy, ZilchNoOverload, "Destroy", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::GetSpace, ZilchNoOverload, "ThisSpace", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::GetGameSession, ZilchNoOverload, "ThisGameSession", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::FindChildByName, ZilchNoOverload, "FindChildByName", ZilchNoNames);
    //ZilchBindMethod(builder, type, &GameObject::FindAllChildrenByName, ZilchNoOverload, "FindAllChildrenByName", ZilchNoNames);
    //ZilchBindMethod(builder, type, &GameObject::Children, ZilchNoOverload, "Children", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::AttachTo, ZilchNoOverload, "AttachTo", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::AttachToRelative, ZilchNoOverload, "AttachToRelative", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::Detach, ZilchNoOverload, "Detach", ZilchNoNames);
    ZilchBindMethod(builder, type, &GameObject::DetachRelative, ZilchNoOverload, "DetachRelative", ZilchNoNames);
    // Properties
    DCE_BINDING_DEFINE_ATTRIBUTE(Hidden);
    DCE_BINDING_DEFINE_PROPERTY(GameObject, Locked);
    DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLocked, attributeHidden);
    DCE_BINDING_DEFINE_PROPERTY(GameObject, GameObjectID);
  }

  // Initialize the static member variables
  unsigned int GameObject::GameObjectsCreated = 0;
  unsigned int GameObject::GameObjectsDestroyed = 0;
  unsigned int GameObject::GameObjectsActive = 0;
  std::unordered_set<unsigned int> GameObject::ActiveGameObjectIDs;
  std::string GameObject::GameObjectLastCreated;
  std::string GameObject::GameObjectLastDestroyed;
  // Enable diagnostics
  bool GameObject::DiagnosticsEnabled = true;

  /**************************************************************************/
  /*!
  @brief  GameObject constructor.
  @param  The name of the GameObject.
  @param  A reference to the Space.
  @param  A reference to the GameSession.
  */
  /**************************************************************************/
  GameObject::GameObject(std::string name, Space& space, GameSession& gamesession)
    : Entity(name), SpaceRef(&space), GamesessionRef(&gamesession), ParentRef(nullptr)
   , GameObjectID(GameObjectsCreated++), Locked(false)
  {

    if (TRACE_ON && TRACE_CONSTRUCTOR) {
      DCTrace << ObjectName << "::GameObject - Constructor - "
        << "', Space: '" << SpaceRef->Name()
        << "', GameSession '" << GamesessionRef->Name()
        << "\n";

    }
       
    
    type_ = EntityType::GameObject;

    // Diagnostics
    if (DiagnosticsEnabled) {
      GameObjectLastCreated = ObjectName;
      GameObjectsActive++;
    }

  }

  /**************************************************************************/
  /*!
  @brief  Default GameObject constructor.
  @param  A reference to the Space.
  @param  A reference to the GameSession.
  */
  /**************************************************************************/
  GameObject::GameObject() : Entity("GameObject"), ParentRef(nullptr),
                             GameObjectID(GameObjectsCreated++)
  {
    // Diagnostics
    if (DiagnosticsEnabled) {
      GameObjectLastCreated = ObjectName;
      GameObjectsActive++;
    }
  }

  /**************************************************************************/
  /*!
  @brief  GameObject destructor.
  @todo   Remove all the children in a more intelligent way.
  */
  /**************************************************************************/
  GameObject::~GameObject()
  {
    // If the GameObject is attached to a Parent, detach
    if (ParentRef)
      Detach();
    // If there are children attached to this GameObject, dettach them
    for (auto child : ChildrenContainer) {
      child->ParentRef = nullptr;
    }
    ChildrenContainer.clear();

    GameObjectsActive--;
    GameObjectsDestroyed++;

    // Diagnostics
    if (DiagnosticsEnabled)
      GameObjectLastDestroyed = ObjectName;
  }

  /**************************************************************************/
  /*!
  @brief  Grabs a pointer to the Space this GameObject resides on.
  */
  /**************************************************************************/
  Space* GameObject::GetSpace() {
    return SpaceRef;
  }

  /**************************************************************************/
  /*!
  @brief  Grabs a pointer to the GameSession this GameObject resides on.
  */
  /**************************************************************************/
  GameSession * GameObject::GetGameSession() {
    return GamesessionRef;
  }

  GameObjectPtr GameObject::IsA(EntityPtr entity)
  {
    return dynamic_cast<GameObjectPtr>(entity);
  }


  /**************************************************************************/
  /*!
  @brief   Grab a reference to a specific child of the GameObject.
  @param   name The name of the child.
  @return  A reference to the child.
  */
  /**************************************************************************/
  GameObjectPtr GameObject::FindChildByName(std::string name)
  {
    for (auto child : ChildrenContainer) {
      if (child->Name() == name)
        return child;
    }
    return nullptr;
  }

  /**************************************************************************/
  /*!
  @brief   Grab a reference to the container of all of the GameObject's
           children that have the specified name.
  @param   The name of the children.
  @return  A reference to the container.
  */
  /**************************************************************************/
  GameObjectVec GameObject::FindAllChildrenByName(std::string name)
  {
    GameObjectVec childrenByName;
    for (auto child : ChildrenContainer) {
      if (child->Name() == name)
        childrenByName.push_back(child);
    }
    return childrenByName;
  }

  /**************************************************************************/
  /*!
  @brief   Grab a reference to the container of all of the GameObject's 
           children.
  @return  A reference to the container.
  */
  /**************************************************************************/
  GameObjectVec & GameObject::Children()
  {
    return ChildrenContainer;
  }

  /**************************************************************************/
  /*!
  @brief  Attach a GameObject to a parent GameObject.
  @param  A pointer to a GameObject.
  */
  /**************************************************************************/
  void GameObject::AttachTo(GameObjectPtr parent)
  {
    // If there is no parent
    if (!parent) {
      //DCTrace << Name() << "::GameObject: Failed to attach, no parent! \n";
      return;
    }
    // If trying to attach to self
    if (parent->GameObjectID == this->GameObjectID) {
      //DCTrace << Name() << "::GameObject: Cannot attach to self! \n";
      return;
    }
    // If trying to attach to a child
    for (auto& child : ChildrenContainer) {
      if (child->GameObjectID == parent->GameObjectID) {
        //DCTrace << Name() << "::GameObject: Cannot attach to child! \n";        
        return;
      }
    }

    // Detach from the current
    Detach();
    parent->AddChild(GameObjectPtr(this));
  }

  /**************************************************************************/
  /*!
  @brief  Attach a GameObject to a parent GameObject and computes a new
          transform so that the objects are relative.
  @param  A pointer to a GameObject.
  */
  /**************************************************************************/
  void GameObject::AttachToRelative(GameObjectPtr parent)
  {
    // If there is no parent
    if (!parent)
      return;
    // If trying to attach to self
    if (parent->GameObjectID == this->GameObjectID)
      return;
    // If trying to attach to a child
    for (auto& child : ChildrenContainer) {
      if (child->GameObjectID == parent->GameObjectID)
        return;
    }

    DetachRelative();
    parent->AddChild(GameObjectPtr(this));
    // Compute new translation if a transform component is attached
    if (auto transform = getComponent<Components::Transform>()) {
      transform->UpdateTranslation();
    }
  }

  /**************************************************************************/
  /*!
  @brief  Detaches from a parent GameObject.
  @note   This function will set the space back as the parent object.
  */
  /**************************************************************************/
  void GameObject::Detach()
  {
    if (!ParentRef)
      return;
    ParentRef->RemoveChild(GameObjectPtr(this));
    ParentRef = nullptr;
  }

  /**************************************************************************/
  /*!
  @brief  Detaches from a parent GameObject and computes a new
          transform so that the objects are relative.
  @note   This function will set the space back as the parent object.
  */
  /**************************************************************************/
  void GameObject::DetachRelative()
  {
    ParentRef->RemoveChild(GameObjectPtr(this));
    ParentRef = nullptr;
    // Compute new translation if a transform component is attached
    if (auto transform = getComponent<Components::Transform>()) {
      transform->UpdateTranslation();
    }
  }

  /**************************************************************************/
  /*!
  @brief Serializes a GameObject.
  @param builder A reference to the JSON builder.
  @note  This will serialize the GameObject.
  */
  /**************************************************************************/  
  void GameObject::Serialize(Zilch::JsonBuilder & builder)
  {
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();

    // Serialize the type as the key
    builder.Key("GameObject");
    builder.Begin(Zilch::JsonType::Object); 
    {
      // Serialize GameObject properties
      SerializeByType(builder, interface->GetState(), ZilchTypeId(GameObject), this);
      // Serialize the underlying Entity object, which includes its components.
      Entity::Serialize(builder);
      // Serialize all its children and their children
      builder.Key("Children");
      builder.Begin(Zilch::JsonType::Object);
      {
        for (auto& child : ChildrenContainer) {
          child->Serialize(builder);
        }
      }
      builder.End();
    }
    builder.End();

  }

  /**************************************************************************/
  /*!
  @brief Serializes all of the GameObject's children.
  @param builder A reference to the JSON builder.
  @param children A reference to the container of all the children
  @note  This will serialize the GameObject.
  */
  /**************************************************************************/
  void GameObject::SerializeChildren(Zilch::JsonBuilder & builder, GameObjectVec & children)
  {
    //for (auto& child : children) {
    //  child->Serialize(builder);
    //}
  }

  /**************************************************************************/
  /*!
  @brief Deserializes a GameObject.
  @param builder A pointer to the object containing the properties.
  @note  This will deserialize the GameObject's properties, then its components.
  */
  /**************************************************************************/
  void GameObject::Deserialize(Zilch::JsonValue * properties)
  {    
    // Grab a reference to the Zilch Interface
    auto interface = Daisy->getSystem<Systems::Reflection>()->Handler();
    // Deserialize the underlying Entity
    Entity::Deserialize(properties);
    // Deserialize the GameObject properties
    DeserializeByType(properties, interface->GetState(), ZilchTypeId(GameObject), this);
  }
  
  /**************************************************************************/
  /*!
  @brief  Marks the GameObject to be destroyed.
  */
  /**************************************************************************/
  void GameObject::Destroy()
  {
    // Tell the space to remove this object
    SpaceRef->RemoveObject(*this);
    // Tell the space to remove all its children
    for (auto& child : ChildrenContainer) {
      child->Destroy();
    }
  }

  /**************************************************************************/
  /*!
  @brief  Registers a child onto the GameObject.
  @param  A pointer to a child.
  */
  /**************************************************************************/
  void GameObject::AddChild(GameObjectPtr child)
  {
    ChildrenContainer.push_back(child);
    child->ParentRef = GameObjectPtr(this);
  }

  /**************************************************************************/
  /*!
  @brief  Deregisters a child onto the GameObject.
  @param  A pointer to a child.
  */
  /**************************************************************************/
  void GameObject::RemoveChild(GameObjectPtr childToRemove)
  {
    for (auto& child : ChildrenContainer) {
      if (child == childToRemove) {
        child->ParentRef = nullptr;
        std::swap(child, ChildrenContainer.back());
        ChildrenContainer.pop_back();
    break;
      }

    }  
  }

  /**************************************************************************/
  /*!
  @brief  Returns a struct containing data about this GameObject.
  @return A struct containing the Name, ID, and ParentID.
  @param  A pointer to a child.
  */
  /**************************************************************************/
  GameObject::Identifier GameObject::Identify()
  {
    Identifier identifier;
    identifier.Name = Name();
    identifier.ID = GameObjectID;
    identifier.ParentID = ParentID;
    return identifier;
  }

  /**************************************************************************/
  /*!
  @brief  Deregisters a child onto the GameObject.
  @param  A pointer to a child.
  */
  /**************************************************************************/
  std::ostream & operator<<(std::ostream& out, GameObject const & gameObject)
  {
    out << "GameObject: '" << gameObject.Name() << "' \n";
    out << "Components: \n";
    for (auto& component : gameObject.ComponentsContainer) {
      out << " - " << component->getObjectName() << "\n";
    }
    if (gameObject.ParentRef)
      out << "Parent: " << gameObject.ParentRef->getObjectName() << "\n";
    if (!gameObject.ChildrenContainer.empty())
    {
      out << "Children: \n";
      for (auto& child : gameObject.ChildrenContainer) {
        out << " - '" << child->Name() << "' \n";
      }
    }    
    return out;
  }

}