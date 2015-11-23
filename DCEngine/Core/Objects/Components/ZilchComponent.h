/*****************************************************************************/
/*!
\file   ZilchComponent.h
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   9/22/2015
\brief  Zilch components are used by game programmers to write game logic into.
        These components, alongside events, drive the logic of a game project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../Component.h"

namespace DCEngine {

  class ZilchComponent : public Component {

    ZilchComponent(Entity& owner) : Component(std::string("ZilchComponent"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  };

}