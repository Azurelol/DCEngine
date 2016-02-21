/******************************************************************************/
/*!
\file   System.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The base System class.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

// Headers
#include "../Objects/Module.h"
#include "..\Objects\Entity.h"
#include "..\Debug\Debug.h"
// Helpers
#include "DispatchSystemEvents.h"

namespace DCEngine {

  enum class EnumeratedSystem {
    Window,
    Input,
    Graphics,
    Audio,
    Physics,
    Factory,
    Content,
    Serializer,
    Reflection,
    Editor,
    GUI,
    Keyboard,
    Mouse,

    // The amount of systems
    Capacity
  };

  // Alias for the enum
  using ESys = EnumeratedSystem;

  class System : public Object {

  public:
    System(std::string name, ESys type) : Object(name), Type(type) {
    }
    ~System();
    
    // Sharing is caring
    friend class Space;
    friend class Engine; 

  private:
    EntityVec ActiveDelegateHolders; //!< @todo Remove me soon
    System() = delete;
    virtual void Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Terminate() = 0;


  protected:
    const ESys Type;
    
  };

  // ALIASES
  using SystemPtr = std::shared_ptr<System>;
  using SystemVec = std::vector<SystemPtr>;

} // DCEngine