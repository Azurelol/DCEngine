/*****************************************************************************/
/*!
\file   ZilchComponent.h
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   9/22/2015
\brief  Zilch components are used by gameplay programmers to write game logic into.
        These components, alongside events, drive the logic of a game project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../Component.h"

namespace DCEngine {

  // Forward declarations
  class Entity;
  namespace Systems {
    class Factory;
  }
  class Engine;

  class ZilchComponent : public Component {
    friend class Entity;
    friend class Factory;
    friend class Engine;

  public:

    ZilchDeclareDerivedType(ZilchComponent, Component);
    ZilchComponent(std::string name, Entity& owner);
    virtual ~ZilchComponent();
    virtual void Initialize();

  private:



  };

}