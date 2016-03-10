/******************************************************************************/
/*!
\file   Space.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  Spaces are containers for game objects. When a level is loaded into
        a space, all the game objects from that level are put into the space.
        Each space has its own instances of the core systems of the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Base class
#include "..\Entity.h"
// Libraries
#include <vector>
#include <unordered_map>
// Headers
#include "..\..\Systems\System.h"
#include "..\..\Resources\Level.h"
#include "GameObject.h"
#include "..\..\Resources\Archetype.h"

namespace DCEngine {

  // Forward declarations
  class GameSession;
  class Archetype;
  namespace Systems {
    class Editor;
    class Factory;
  }

  class Space : public Entity {
    friend class GameSession;
    friend class Systems::Factory;
    friend class Systems::Editor;    
  
  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Space, Entity);
    #endif
    
    Space(std::string name, GameSession& gamesession);
    ~Space();
    void Initialize();
    void Terminate();
    void Destroy();
    void Update(float dt);
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);

    // Levels
    void LoadLevel(std::string& level);
    void ReloadLevel();

    // GameObjects
    GameObjectPtr CreateObject(); 
    GameObjectPtr CreateObject(std::string archetypeName);
    GameObjectPtr CreateObject(ArchetypePtr archetype);
    GameObjectPtr CreateObjectAtPosition(ArchetypePtr, const Vec3&);
    GameObject* FindObjectByName(const std::string name);
    GameObjectVec* AllObjects();
    GameObject::Identifiers IdentifyAllObjects();
    void AddObject(GameObjectPtr entity);
    void RemoveObject(GameObject&);
    void DestroyAll();
    void TestSpace();

    GameSession* getGameSession();
    const GameSession* Owner() { return GameSessionRef; };
    friend std::ostream& operator<<(std::ostream& out, Space const& space);

  private:
        
    LevelPtr CurrentLevel; //!< The currently-loaded level.
    GameSession* GameSessionRef; //!< The gamesession in which this space resides
    GameObjectVec GameObjectContainer; //!< A vector of GameObjects this space holds.
    //!< A container of recently created GameObjects. They are added onto the main container on the next frame.
    GameObjectVec RecentlyCreatedGameObjects;

    // GameObjects
    void MigrateRecentlyAdded();
    void SwapGameObject(GameObjectPtr, Direction);
    // Levels
    void LoadSampleLevel(LevelPtr level);
    LevelPtr SaveLevel(const std::string& levelName);
    void LoadLevel(LevelPtr level);

    Space() = delete; //!< Spaces should never be default or copy constructed.
    Space(Space& space) = delete;  
  };

  using SpacePtr = Space*;
  using SpaceStrongPtr = std::shared_ptr<Space>;
  using SpaceMap = std::unordered_map<std::string, SpaceStrongPtr>;

}