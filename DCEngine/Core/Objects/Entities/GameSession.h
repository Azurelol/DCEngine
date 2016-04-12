/******************************************************************************/
/*!
\file   GameSession.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/18/2015
\brief  The container object for the spaces in the game, at the highest hierarchy.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

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
#include "..\..\Events\DispatchGameEvents.h"


namespace DCEngine {
   
  // Forward declarations
  class Engine;
  namespace Systems {
    class Factory;
  }
  
  class GameSession : public Entity {

    friend class Engine; //!< Engine has access to gamesession.
    friend class Systems::Factory;

  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(GameSession, Entity);
    #endif


    GameSession(std::string name);
    ~GameSession();
    static GameSession* Get();

    SpacePtr CreateSpace(std::string name, bool initialize = true);
    SpacePtr GetSpace(std::string name);
    SpaceMap& AllSpaces();
    Space* getDefaultSpace();
    void Quit();

  private:

    // Members
    Space* DefaultSpace;
    std::string _defaultSpace = "Daisy Space";
    SystemVec Systems; //!< Container for the GameSession's systems. 
    SpaceMap ActiveSpaces; //!< A map of spaces created by the engine.
    // Methods
    void Serialize(Zilch::JsonBuilder& builder);
    void Deserialize(Zilch::JsonValue* properties);
    void Initialize();
    void Subscribe();
    void UpdateSpace(SpacePtr space, float dt);
    void OnUpdateEvent() {};
    void RemoveSpace(SpacePtr);   

  }; // GameSession

  using GameSessionPtr = GameSession*;
  using GameSessionStrongPtr = std::unique_ptr<GameSession>;

} // DCEngine
