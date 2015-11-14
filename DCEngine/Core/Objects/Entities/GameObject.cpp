#include "GameObject.h"

#include "Space.h"
#include "GameSession.h"

namespace DCEngine {
    
  unsigned int GameObject::GameObjectsCreated = 0;

  /**************************************************************************/
  /*!
  @brief  GameObject constructor.
  @param  The name of the GameObject.
  @param  A reference to the Space.
  @param  A reference to teh GameSession.
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
  }

  GameObject::GameObject() : Entity("GameObject"), ParentRef(nullptr),
                             GameObjectID(GameObjectsCreated++)
  {
  }

  Space* GameObject::GetSpace() {
    return SpaceRef;
  }

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
    if (auto transform = getComponent<Transform>()) {
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
    if (auto transform = getComponent<Transform>()) {
      transform->UpdateTranslation();
    }
  }

  /**************************************************************************/
  /*!
  @brief  Destroys the GameObject after it has removed all its components
          in turn, unsubscribing them.
  */
  /**************************************************************************/
  void GameObject::Destroy()
  {
    // 1. Remove all components from the GameObject
    for (auto component : ComponentsContainer)
      RemoveComponent(component);
    // 2. Mark this GameObject for destruction. On the next frame,
    // the factory will handle deleting it.

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
  void GameObject::RemoveChild(GameObjectPtr child)
  {
    ChildrenContainer.erase(std::remove(ChildrenContainer.begin(),
      ChildrenContainer.end(), child),
      ChildrenContainer.end());    
  }

}