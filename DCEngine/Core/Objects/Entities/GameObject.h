/******************************************************************************/
/*!
\file   GameObject.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The Game Object composition class. Spaces are comprised of GameObject
        entities.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "..\Entity.h"

namespace DCEngine {

  class GameObject;
  using GameObjectPtr = GameObject*;
  using GameObjectVec = std::vector<GameObjectPtr>;
  using GameObjectStrongPtr = std::unique_ptr<GameObject>;
  using GameObjectStrongVec = std::vector<GameObjectStrongPtr>;  
  using GameObjectRawVec = std::vector<GameObject*>;  

  /* Forward declarations*/
  class Space;
  class GameSession;
  //class Factory;

  namespace Systems {
    class Factory;
    class Editor;
  }

  class GameObject : public Entity {
    friend class Space;
    friend class Factory;
    friend class Systems::Editor;

  public:
    
    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(GameObject, Entity);
    #endif
    
    GameObject(std::string name, Space& space, GameSession& gamesession);
    GameObject();
    ~GameObject();
    void Destroy();
    Space* GetSpace();
    GameSession* GetGameSession();  
    GameObjectPtr FindChildByName(std::string name);
    GameObjectVec FindAllChildrenByName(std::string name);
    GameObjectVec& Children();    
    void AttachTo(GameObjectPtr parent);
    void AttachToRelative(GameObjectPtr parent);
    void Detach();
    void DetachRelative();
    GameObjectPtr Parent() { return ParentRef; }    
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);

    // Stream
    friend std::ostream& operator<<(std::ostream&, GameObject const&);

    // Static member variables
    static unsigned int GameObjectsCreated;
    static unsigned int GameObjectsDestroyed;
    static std::string GameObjectLastCreated;
    static std::string GameObjectLastDestroyed;
    static bool DiagnosticsEnabled;

  private:
    GameObjectPtr ParentRef;
    GameObjectVec ChildrenContainer;
    Space* SpaceRef;
    GameSession* GamesessionRef;

    void AddChild(GameObjectPtr child);
    void RemoveChild(GameObjectPtr child);

    const unsigned int GameObjectID;    

  };  
  
}