/*****************************************************************************/
/*!
\file   Level.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  A level is a resource that stores a set of GameObjects that can be loaded
        into a space.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "..\Objects\Entities\GameObject.h"

namespace DCEngine {
  
  class Level : public Resource {
  public:
    Level(std::string name) : Resource(name) {
      if (TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << "::Level - Constructor \n";
    }
    ~Level();

    void AddGameObject(GameObjectPtr gameObject);
    GameObjectVec GameObjects; //!< Container for entities in the level.   

  private:

  };
  
  using LevelPtr = std::shared_ptr<Level>;

}

