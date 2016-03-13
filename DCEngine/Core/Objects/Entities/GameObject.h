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
    friend class Systems::Factory;
    friend class Systems::Editor;

  public:
    
    struct Identifier {
      std::string Name;
      unsigned ID;
      unsigned ParentID;
    };

    struct Identifiers {
      std::vector<std::string> Names;     
      std::vector<unsigned> IDs;
      std::vector<unsigned> ParentIDs;
      
      std::vector<const char*> NamesAsChars() {
        std::vector<const char *> names;
        for (auto& name : Names)
          names.push_back(name.c_str());        
        return names;
      }
    };

    using GameObjectIdentifiers = std::vector<Identifier>;
        
    ZilchDeclareDerivedType(GameObject, Entity);
    GameObject(std::string name, Space& space, GameSession& gamesession);
    GameObject();
    ~GameObject();
    void Destroy();
    Space* GetSpace();
    GameSession* GetGameSession();  
    static GameObjectPtr IsA(EntityPtr);
    GameObjectPtr FindChildByName(std::string name);
    GameObjectVec FindAllChildrenByName(std::string name);
    GameObjectVec& Children();    
    void AttachTo(GameObjectPtr parent);
    void AttachToRelative(GameObjectPtr parent);
    void Detach();
    void DetachRelative();
    GameObjectPtr Parent() { return ParentRef; }    
    DCE_DEFINE_PROPERTY(bool, Locked);
    DCE_DEFINE_PROPERTY(unsigned int, GameObjectID);
    void Serialize(Zilch::JsonBuilder& builder);
    void SerializeChildren(Zilch::JsonBuilder& builder, GameObjectVec& children);
    void Deserialize(Zilch::JsonValue* properties);
    // Stream
    friend std::ostream& operator<<(std::ostream&, GameObject const&);
    // Static member variables
    static unsigned int GameObjectsCreated;
    static unsigned int GameObjectsDestroyed;
    static unsigned int GameObjectsActive;
    static std::string GameObjectLastCreated;
    static std::string GameObjectLastDestroyed;
    static bool DiagnosticsEnabled;

  private:
    static std::unordered_set<unsigned int> ActiveGameObjectIDs;
    unsigned int GameObjectID;    
    GameObjectPtr ParentRef;
    unsigned ParentID;
    GameObjectVec ChildrenContainer;
    Space* SpaceRef;
    GameSession* GamesessionRef;
    bool Locked;    

    void AddChild(GameObjectPtr child);
    void RemoveChild(GameObjectPtr child);
    Identifier Identify();

  };  


  
}