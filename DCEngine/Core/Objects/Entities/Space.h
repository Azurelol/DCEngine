/******************************************************************************/
/*!
\file   Space.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  Spaces are containers for game objects. When a level is loaded into
        a space, all the game objects from that level are put into the space.
        Each space has its own instances of the core systems of the engine.
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

namespace DCEngine {

  // Forward declarations
  class GameSession;
  class Factory;
  class Archetype;

  class Space : public Entity {
    friend class GameSession;
    friend class Factory;
  
  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Space, Entity);
    #endif


    Space(std::string name, GameSession& gamesession);
    ~Space();
    void Initialize();
    void Terminate();
    void Update(float dt);

    void SaveLevel(const std::string& level);
    void LoadLevel(LevelPtr level);
    void LoadLevel(std::string& level);
    void ReloadLevel();

    GameObjectPtr CreateObject(); 
    GameObjectPtr CreateObject(std::shared_ptr<Archetype> archetype);
    GameObject* FindObjectByName(const std::string name);
    GameObjectVec* AllObjects();
    void AddObject(GameObjectPtr entity);
    void RemoveObject(GameObject&);
    void Clear(); // Remves all entities and systems
    void DestroyAll();

    GameSession& getGameSession();
    const GameSession* Owner() { return GameSessionRef; };

  private:
        
    LevelPtr CurrentLevel; //!< The currently-loaded level.
    GameSession* GameSessionRef; //!< The gamesession in which this space resides
    GameObjectVec GameObjectContainer; //!< A vector of GameObjects this space holds.

    Space() = delete; //!< Spaces should never be default or copy constructed.
    Space(Space& space) = delete;  
  };

  using SpacePtr = std::shared_ptr<Space>;
  using SpaceMap = std::unordered_map<std::string, SpacePtr>;

}