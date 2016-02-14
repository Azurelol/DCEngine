/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The main driver for the Daisy Chain engine, and its engine class,
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Libraries
#include <unordered_map>
#include <stack>
#include <functional> // std::function, std::bind

// Headers
#include "Types.h"
#include "Event.h"
#include "Math.h"
#include "Action.h"

#include "../Objects/ObjectsInclude.h"
#include "../Objects/Entities/EntitiesInclude.h"
#include "..\Systems\System.h"
#include "..\Systems\SystemsInclude.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;

  /**************************************************************************/
  /*!
  @class Engine The engine class encapsulates the whole of the engine's
         systems, objects and resources.
  */
  /**************************************************************************/
  class Engine : public Object {
    friend class EngineLauncher;
  public:

    void Register(ActionPtr action);
    void Deregister(ActionPtr action);

    Engine();
    Engine(std::string configFile);
    ~Engine();
    void Initialize();
    void Loop();
    void Terminate();
    auto Stop() { Active = false; }
    void LoadProject(std::string& filename);
    void StartProject();    

    Keyboard* getKeyboard() { return KeyboardHandle.get(); }
    Mouse* getMouse() { return MouseHandle.get(); }
    Systems::Factory& getFactory() {
      return *getSystem<Systems::Factory>(EnumeratedSystem::Factory);
    }
    GameSession* getGameSession() { return CurrentGameSession.get(); }

    // Component Events
    template <typename EventClass, typename Class, typename MemberFunction>
    void Connect(Entity* publisher, MemberFunction fn, Class* inst);
    template <typename Publisher, typename Observer>
    void Disconnect(Publisher* publisher, Observer* observer);
    // System Events
    template<typename EventClass, typename SystemClass, typename MemberFunction>
    void Connect(MemberFunction fn, SystemClass* sys);
    template <typename EventClass> void Dispatch(Event* eventObj);

    template<typename T> std::shared_ptr<T> getSystem(EnumeratedSystem sysType);
    template<typename T> std::shared_ptr<T> getSystem();

  private:

    bool Paused;
    EngineConfigPtr EngineConfiguration;
    GameSessionPtr CurrentGameSession; //!< The current GameSession object.
    KeyboardPtr KeyboardHandle;
    MousePtr MouseHandle;
    float dt; //!< Delta time. 
    float Framerate = 60.0f; //!< The target frame rate.
    float Runtime; //!< How long the engine has been running.
    bool Active; //!< Whether the engine is active or not.      
    std::string _projectName = "Daisy Project"; //!< The current project.
    std::string _defaultSpace = "Daisy World";
    SystemVec Systems; //!< Container for the engine's systems.   
    SpaceMap Spaces; //!< A map of spaces created by the engine.
    ActionSpace ActionSpace; 
    std::map<std::type_index, std::list<DCEngine::EventDelegate*>> ObserverRegistry;

    bool LoadEngineConfig();
    void LoadDefaultSpace();
    // Updates
    void Update(float dt);   
    void DispatchUpdateEvents(float dt);    
    void UpdateActions(float dt);
    // Events
    void Subscribe();
    void OnWindowLostFocusEvent(Events::WindowLostFocus* event);
    void OnWindowGainedFocusEvent(Events::WindowGainedFocus* event);    
    void OnEnginePauseEvent(Events::EnginePause* event);
    void OnEngineResumeEvent(Events::EngineResume* event);
    void OnEngineExitEvent(Events::EngineExit* event);
    void OnEnginePauseMenuEvent(Events::EnginePauseMenu* event);
    
  }; 

  /**************************************************************************/
  /*!
  @class EngineLauncher Helper class that launches the engine,
  */
  /**************************************************************************/
  class EngineLauncher {
  public:
    EngineLauncher() {
      engine_.reset(new Engine());
    }
    void InitializeEngine() {
      engine_->Initialize();
    }
    void StartLoop() {
      engine_->Loop();
    }
    void TerminateEngine() {
      engine_->Terminate();
    }
  private:
    std::unique_ptr<Engine> engine_;
  };

} 

// Templates
#include "Engine.hpp"