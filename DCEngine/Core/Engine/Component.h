/******************************************************************************/
/*!
\file   Component.h
\author Allan Deutsch, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The

*/
/******************************************************************************/
#include "Object.h"
#pragma once

#include <memory>
#include <vector>

namespace DCEngine {
  
  using mask = unsigned;

#pragma region metadataEnums
  
  enum class EnumeratedComponent {
    None = 0,
    Transform = 1,
    Sprite = 2,
    Drawable = 3,

    Capacity,    

  };

  enum class BitfieldComponent {
    Alive = 1,
    Transform = 1 << 1,
    Sprite = 1 << 2,
    Drawable = 1 << 3,

    NoObjects = -1,

  };
  
  class Component : public Object {
  public:    
    Component(EnumeratedComponent type, BitfieldComponent mask);

    // Derived component types need to be deallocated properly
    virtual ~Component(); 

    // Once the component is created, it should never change types
    const EnumeratedComponent _type;
    const BitfieldComponent _mask;

  private:
    Component() = delete; // No default construction

  };

  using ComponentPtr = std::shared_ptr<Component>;
  using ComponentVec = std::vector<ComponentPtr>;
  //using ComponentPtr = std::shared_ptr<Component>;

}