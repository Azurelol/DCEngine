/******************************************************************************/
/*!
\file   GameSession.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The container object for the spaces in the game, at the highest hierarchy.

*/
/******************************************************************************/
#include "..\Entity.h"
#pragma once

#include <memory>
#include <unordered_map>
#include <stack>

#include "Space.h"
#include "..\Component.h"
#include "..\..\Systems\System.h"
#include "..\..\Systems\SystemsInclude.h"


namespace DCEngine {

  class Engine;
  
  class GameSession : public Entity {
    friend class Engine; //!< Engine has access to gamesession.

  public:
    GameSession(std::string& name);
    ~GameSession();

    void Initialize();
    void Update(float dt);
    
    template<typename T> std::shared_ptr<T> GetSystem(EnumeratedSystem sysType);

    // SPACE //
    SpacePtr CreateSpace(std::string name);
    SpacePtr GetSpace(std::string name);

  private:
    void UpdateSpace(SpacePtr space, float dt);
    void OnUpdateEvent() {};
    void OnUpdateEvent(Event& eventObj);

    std::string _defaultSpace = "Daisy Space";    
    // CONTAINERS //
    SystemVec _systems; //!< Container for the GameSession's systems. 
    SpaceMap _spaces; //!< A map of spaces created by the engine.
  }; // GameSession

  using GameSessionPtr = std::unique_ptr<GameSession>;

} // DCEngine
