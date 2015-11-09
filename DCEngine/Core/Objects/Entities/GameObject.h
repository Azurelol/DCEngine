/******************************************************************************/
/*!
\file   Entity.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The Game Object composition class. Spaces are comprised of GameObject
        entities.

*/
/******************************************************************************/
#pragma once
#include "..\Entity.h"

namespace DCEngine {

  class GameObject;
  using GameObjectPtr = std::shared_ptr<GameObject>;
  using GameObjectVec = std::vector<GameObjectPtr>;
  using GameObjectRawVec = std::vector<GameObject*>;


  /* Forward declarations*/
  class Space;
  class GameSession;
  //class Factory;

  class GameObject : public Entity {
    friend class Space;
    //friend class Factory;

  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(GameObject, Entity);
    #endif
    
    GameObject(std::string name, Space& space, GameSession& gamesession);
    GameObject();

    //virtual void Serialize(Json::Value& root);
    //virtual void Deserialize(Json::Value& root);

    Space* GetSpace();
    GameSession* GetGameSession();  

    GameObjectPtr FindChildByName(std::string name);
    GameObjectVec FindAllChildrenByName(std::string name);
    GameObjectVec& Children();    

    void AttachTo(GameObject* parent);
    void AttachToRelative(GameObject* parent);
    void Detach();
    void DetachRelative();
    
    void Destroy();
    META_ADD_CLASS(GameObject); // Provides reflection for this object

  private:
    Space* SpaceRef;
    GameSession* GamesessionRef;
    GameObjectVec ChildrenContainer;

    void AddChild(GameObjectPtr child);
    void RemoveChild(GameObjectPtr child);

    static int GameObjectsCreated;
    const unsigned int GameObjectID;
    

  };

  
  //using GameObjectPtr = std::shared_ptr<GameObject>;
  //using GameObjectVec = std::vector<GameObjectPtr>;
  //using GameObjectRawVec = std::vector<GameObject*>;

}