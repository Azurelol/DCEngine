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
#include "..\Entity.h"

// Libraries
#include <vector>
#include <unordered_map>

// Headers
#include "..\..\Systems\System.h"
#include "..\Component.h"
#include "..\Resources\Level.h"

#define TRACE_ON 1

namespace DCEngine {
  
  class Space : public Entity {
  
  public:
    Space(std::string& name);
    ~Space();
    
    void Initialize();
    void Update(float dt);

    void AddSystem(SystemPtr system);
    void RemoveSystem(std::string system);

    // LEVEL
    void LoadLevel(LevelPtr level);

    EntityPtr CreateEntity();
    void AddEntity(EntityPtr entity);
    void PopulateEntities(SystemPtr sys) const;
    void RemoveEntity(EntityPtr);
    void Clear(); // Remves all entities and systems

    // Allows read only access to the space's name
    const std::string& GetName() const { return _name;  }


  private:
    LevelPtr _currentLevel;
    EntityPtr _camera;
    EntityVec _entities;
    SystemVec _systems; 


    // Spaces should never be default or copy constructed.
    Space() = delete;
    Space(Space& space) = delete;
  };

  using SpacePtr = std::shared_ptr<Space>;
  using SpaceMap = std::unordered_map<std::string, SpacePtr>;

}