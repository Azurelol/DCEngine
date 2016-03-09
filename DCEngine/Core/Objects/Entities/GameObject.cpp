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
    
  // Initialize the static member variables
  unsigned int GameObject::GameObjectsCreated = 0;
  unsigned int GameObject::GameObjectsDestroyed = 0;
  unsigned int GameObject::GameObjectsActive = 0;
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
   , GameObjectID(GameObjectsCreated++) 
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
    // Serialize the type as the key
    builder.Key("GameObject");
    builder.Begin(Zilch::JsonType::Object); {
      // Serialize the underlying Entity object, which includes its components.
      Entity::Serialize(builder);
    }
    builder.End();

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
    DeserializeByType(properties, interface->GetState(), this->ZilchGetDerivedType(), this);
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