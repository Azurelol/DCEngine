/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The

*/
/******************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

// Base class
#include "Object.h"
// Headers
#include "../Events/Event.h" //!< Components need to access events.
#include "../Events/EventsInclude.h" //!< A list of events that can be added.

//#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  // All components can access the engine.
  //extern std::unique_ptr<Engine> Daisy;
  
  // (?) EXPLAIN
  using mask = unsigned;

#pragma region metadataEnums
  
  enum class EnumeratedComponent {
    None = 0,
    Transform = 1,
    Sprite = 2,
    Drawable = 3,

    TimeSpace = 10,
    GraphicsSpace = 11,
    PhysicsSpace = 12,
    SoundSpace = 13,


    Capacity,    

  };

  enum class BitfieldComponent {
    Alive = 1,
    Transform = 1 << 1,
    Sprite = 1 << 2,
    Drawable = 1 << 3,

    TimeSpace = 1 << 10,
    GraphicsSpace = 1 << 11,
    PhysicsSpace = 1 << 12,
    SoundSpace = 1 << 13,

    NoObjects = -1,

  };
  
  // FORWARD-DECLARATIONS
  class Entity;
  class GameObject;
  class Space;
  class GameSession;

  class Component : public Object {
	  friend class Entity; // _owner
    friend class GameObject; 

  public:    
    Component(EnumeratedComponent type, BitfieldComponent mask,
      Entity& owner);
    
	  virtual ~Component() {} // Derived component types need to be deallocated properly
    virtual void Initialize() = 0; // Every component needs to be initialized.
    virtual void Update() = 0; // Components are updated through events.
	
    // 
    
    Entity* Owner(); // Returns a pointer to the component's owner

    // Once the component is created, it should never change types
    const EnumeratedComponent _type;
    const BitfieldComponent _mask;

  private:
    Component() = delete; // No default construction
    void SetReferences();

    Space* space_;
    GameSession* gamesession_;

  };

  using ComponentPtr = std::shared_ptr<Component>;
  using ComponentVec = std::vector<ComponentPtr>;

} // DCEngine
#endif