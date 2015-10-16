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

// Libraries
#include <string>
#include <memory>
#include <vector>

// Headers
//#include "../Objects/Entities/EntitiesInclude.h"
#include "..\Objects\Entity.h"
#include "..\Debug\Debug.h"

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

    // The amount of systems
    Capacity
  };

  // Alias for the enum
  using ESys = EnumeratedSystem;

  class System {

  public:
    System(std::string& name, ESys type) : SysName(name), _type(type) {
      //trace << _name << "::Constructor \n";
    }

    virtual void Serialize(Json::Value& root) = 0;
    virtual void Deserialize(Json::Value& root) = 0;
    void ClearEntities();

    inline mask Mask() { return _mask; }

    // Sharing is caring
    friend class Space;
    friend class Engine; 

  private:
    System() = delete;
    virtual void Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Terminate() = 0;


  protected:
    std::string SysName;
    const ESys _type;
    EntityVec _entities;
    mask _mask;
    
  };

  // ALIASES
  using SystemPtr = std::shared_ptr<System>;
  using SystemVec = std::vector<SystemPtr>;

} // DCEngine