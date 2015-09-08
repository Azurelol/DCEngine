/******************************************************************************/
/*!
\file   System.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The base System class.

*/
/******************************************************************************/
#pragma once

#include <string>
#include <memory>
#include <vector>

#include "..\Objects\Entity.h"


namespace DCEngine {

  enum class EnumeratedSystem {
    // Window Handler
    Window,
    // Input
    Input,
    // Graphics
    GraphicsGL,
    // Audio
    Audio,

    // Gamestate
    Gamestate,

    // The amount of systems
    Capacity
  };

  // Alias for the enum
  using ESys = EnumeratedSystem;

  class System {

  public:
    System(std::string& name, ESys type) : _name(name),
                                           _type(type)
    { }

    virtual void Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Terminate() = 0;

    void ClearEntities();

    inline mask Mask() { return _mask; }

    // Sharing is caring
    friend class Space;
    friend class Engine; 

  private:
    System() = delete;

  protected:
    std::string _name;
    const ESys _type;
    EntityVec _entities;
    mask _mask;
    
  };

  // ALIASES
  using SystemPtr = std::shared_ptr<System>;
  using SystemVec = std::vector<SystemPtr>;

} // DCEngine