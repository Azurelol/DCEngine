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

  class Space : public Entity {
    friend class GameSession;
    friend class Factory;
  
  public:

    Space(std::string& name, GameSession& gamesession);
    ~Space();
    void Initialize();
    void Update(float dt);

    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    
    void AddSystem(SystemPtr system);
    template<typename T> std::shared_ptr<T> getSystem(EnumeratedSystem sysType);
    void RemoveSystem(std::string system);
    
    GameSession& getGameSession();

    void LoadLevel(LevelPtr level);
    GameObjectPtr CreateObject();
    GameObject* FindObjectByName(const std::string& name);
    void AddObject(GameObjectPtr entity);
    void PopulateObjects(SystemPtr sys) const;
    void RemoveObject(GameObjectPtr);
    void Clear(); // Remves all entities and systems

    const GameSession* Owner() { return gamesession_; };

    // Allows read only access to the space's name
    const std::string& GetName() const { return _name;  }

  private:

    /*/ MEMBER [FUNCTIONS] /*/
    Space() = delete; //!< Spaces should never be default or copy constructed.
    Space(Space& space) = delete;

    //void OnUpdateEvent(Event& eventObj);

    /*/ MEMBER [VARIABLES] /*/
    LevelPtr _currentLevel; //!< The currently-loaded level.
    //GameObjectPtr _camera;
    SystemVec _systems; //!< A container of systems this space is running.
    GameSession* gamesession_; //!< The gamesession in which this space resides
    GameObjectVec gameobjects_; //!< A vector of GameObjects this space holds.
    
  };

  /**************************************************************************/
  /*!
  \brief  Allows access to a system running in the engine via system type.
  \note   This function call can be made much easier using the GETSYSTEM
  macro that takes a system typename and expands it to fill the
  template parameters.
  \para   An enumerated system.
  \return A shared pointer to the requested system.
  */
  /**************************************************************************/
  template <typename T>
  std::shared_ptr<T> Space::getSystem(EnumeratedSystem sysType) {
    for (auto &it : _systems) {
      if (it->_type == sysType)
        return std::static_pointer_cast<T>(it);
    }
    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }

  using SpacePtr = std::shared_ptr<Space>;
  using SpaceMap = std::unordered_map<std::string, SpacePtr>;

}