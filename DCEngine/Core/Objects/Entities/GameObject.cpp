#include "GameObject.h"

#include "Space.h"
#include "GameSession.h"

namespace DCEngine {
  

  /**************************************************************************/
  /*!
  @brief  GameObject constructor.
  @param  The name of the GameObject.
  @param  A reference to the Space.
  @param  A reference to teh GameSession.
  */
  /**************************************************************************/
  GameObject::GameObject(std::string name, Space& space, GameSession& gamesession)
    : Entity(name), SpaceRef(&space), GamesessionRef(&gamesession) 
   // , GameObjectID(GameObjectsCreated++) 
  {

    if (TRACE_ON && TRACE_CONSTRUCTOR) {
      trace << ObjName << "::GameObject - Constructor - "
        << "', Space: '" << SpaceRef->Name()
        << "', GameSession '" << GamesessionRef->Name()
        << "\n";

    }

    type_ = EntityType::GameObject;
    // Every GameObject is parented to the space by default
    Parent = SpaceRef;
  }

  GameObject::GameObject() 
    // : GameObjectID(GameObjectsCreated++)
  {
    ObjName = "GameObject";
  }

  void GameObject::Serialize(Json::Value & root) {
    // Serialize primitives
    root["Name"] = ObjName;
  }

  void GameObject::Deserialize(Json::Value & root) {
    // Deserialize primitives
    ObjName = root.get("Name", "").asString();
  }

  Space* GameObject::GetSpace() {
    return SpaceRef;
  }

  GameSession * GameObject::GetGameSession() {
    return GamesessionRef;
  }


  /**************************************************************************/
  /*!
  @brief  Attach a GameObject to a parent GameObject.
  @param  A pointer to a GameObject.
  */
  /**************************************************************************/
  void GameObject::AttachTo(GameObject* parent)
  {
    Parent = parent;
  }

  /**************************************************************************/
  /*!
  @brief  Attach a GameObject to a parent GameObject and computes a new
          transform so that the objects are relative.
  @param  A pointer to a GameObject.
  */
  /**************************************************************************/
  void GameObject::AttachToRelative(GameObject* parent)
  {
    Parent = parent;
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
    Parent = SpaceRef;
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
    Parent = SpaceRef;
    // Compute new translation if a transform component is attached
    if (auto transform = getComponent<Transform>()) {
      transform->UpdateTranslation();
    }
  }

}