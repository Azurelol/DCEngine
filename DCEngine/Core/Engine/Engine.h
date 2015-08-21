/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  

*/
/******************************************************************************/
#pragma once

#include <memory> // unique_ptr
#include <unordered_map>
#include <stack>

#include "..\Objects\Entities\GameSession.h"
#include "..\Objects\Entities\Space.h"
#include "..\Objects\Entity.h"
#include "..\Objects\Component.h"
#include "..\Systems\System.h"
#include "..\Systems\Gamestate\Gamestate.h"

#include "..\..\Gamestates\GamestatesInclude.h"
#include "..\Systems\SystemsInclude.h"

//extern std::string DefaultSpace = "Daisy World";

namespace DCEngine {
  class Engine {
    public:

      Engine();
      ~Engine();

      void Initialize();      
      void Update(float dt);
      void Loop();
      auto Stop() { _active = false; } // Signals the engine to stop running
      void Terminate();
      
      auto Getdt() { return dt; }

      template<typename T> std::shared_ptr<T> GetSystem(EnumeratedSystem sysType);      

     // auto GetEngine() { return std::shared_ptr<Engine> = this; }

     // GAMESTATE //
      GamestatePtr GetCurrentState() const;
      void PushGamestate(GamestatePtr gamestate);
      void PopGameState();

      // SPACE //
      SpacePtr CreateSpace(std::string name);
      SpacePtr GetSpace(std::string name);
      SpacePtr SetActiveSpace(std::string name);
      SpacePtr GetActiveSpace();
      std::string GetDefaultSpace() { return _defaultSpace; }

    private:

      // VARIABLES //
      GameSessionPtr _gameSession; //!< The current GameSession object.
      float dt; //!< Delta time. 
      float _framerate = 60.0f; //!< The target frame rate.
      float _runtime; //!< How long the engine has been running.
      bool _active; //!< Whether the engine is active or not.
      std::string _projectName = "Daisy Project"; //!< The current project.
      std::string _defaultSpace = "Daisy World"; 

      // CONTAINERS //
      SystemVec _systems; //!< Container for the engine's systems.
      std::string _activeSpace; //!< The current active space.
      std::stack<GamestatePtr> _gamestates; //!< A stack of gamestates. See below.      
      SpaceMap _spaces; //!< A map of spaces created by the engine.

      /* Reference: http://www.cplusplus.com/reference/stack/stack/
      Stacks are a type of container adaptor, specifically designed to operate in a 
      LIFO context (last-in first-out), where elements are inserted and extracted only 
      from one end of the container.
      */
      
  }; // Engine

  // DEFINE MACRO 
  #define GETSYSTEM( systype ) \
  ENGINE->GetSystem<systype>(EnumeratedSystem::##systype)

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
  std::shared_ptr<T> Engine::GetSystem(EnumeratedSystem sysType) {
    for (auto &it : _systems) {
      if (it->_type == sysType)
        return std::static_pointer_cast<T>(it);
    }

    // Throw an error if the system doesn't exist in the engine.
    throw std::range_error("The specified system does not exist.");
  }

} // DCEngine