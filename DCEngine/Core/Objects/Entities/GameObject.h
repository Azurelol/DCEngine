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

  /* Forward declarations*/
  class Space;
  class GameSession;
  //class Factory;

  class GameObject : public Entity {
    friend class Space;
    //friend class Factory;

  public:
    
    GameObject(std::string name, Space& space, GameSession& gamesession);
    GameObject();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    Space* GetSpace();
    GameSession* GetGameSession();  

    void AttachTo(GameObject* parent);
    void AttachToRelative(GameObject* parent);
    void Detach();
    void DetachRelative();

    META_ADD_CLASS(GameObject); // Provides reflection for this object

  private:
    Space* SpaceRef;
    GameSession* GamesessionRef;

    //static int GameObjectsCreated;
    //const unsigned int GameObjectID;
    

  };

  using GameObjectPtr = std::shared_ptr<GameObject>;
  using GameObjectVec = std::vector<GameObjectPtr>;
  using GameObjectRawVec = std::vector<GameObject*>;

}