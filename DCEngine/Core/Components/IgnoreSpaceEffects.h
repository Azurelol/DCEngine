/*****************************************************************************/
/*!
@file   IgnoreSpaceEffect.szeh
@author Blaine Reiner, Christian Sagel
@par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
@date   10/13/2015
@brief  A component that signals to the physics system that this gameobject
        will be ignoring certain effects on the space. Used to special case 
        objects to ignore things such as drag or gravity.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class IgnoreSpaceEffects : public Component {
  public:
    Boolean IgnoreDrag;
    Boolean IgnoreFlow;
    Boolean IgnoreForce;
    Boolean IgnoreGravity;
    Boolean IgnorePointForce;
    Boolean IgnoreThrust;
    Boolean IgnoreVortex;
    Boolean IgnoreWind;
    Boolean IgnoreTorque;
    Boolean IgnoreBuoyancy;

    /* Initialize */
    IgnoreSpaceEffects(Entity& owner) : Component(std::string("IgnoreSpaceEffects"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
  };
}