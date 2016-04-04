/*****************************************************************************/
/*!
@file   PhysicsMaterial.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu, blaine.reiner\@digipen.edu
@date   12/17/2015
@brief  
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {
  
  class PhysicsMaterial;
  using PhysicsMaterialHandle = std::string;
  using PhysicsMaterialRawPtr = PhysicsMaterial*;
  using PhysicsMaterialPtr = std::shared_ptr<PhysicsMaterial>;

  class PhysicsMaterial : public Resource {

  public:

    DCE_DEFINE_PROPERTY(float, Density);
    DCE_DEFINE_PROPERTY(float, Restitution);
    DCE_DEFINE_PROPERTY(float, Friction);

  private:

    // Density is used to determine the mass of an object. Mass can be
    // computed as density volume. Density can be set to 0 exactly to
    // produce a massless object, however this should only be done with
    // children objets to add collision.
    float Density = 1;

    // How much an object will bounce during collision. Values should be in
    // the range [0,1] where 0 is an in-elastic collision and 1 is a fully
    // elastic collision. (Due to solving constraints with baumgarte energy
    // will not be perfectly conserved with restitution 1)
    float Restitution = 1;
    
    // How slippery or rough the object is. When friction is 0 the object will
    // be completely slippery. As friction increases sliding objects stop
    // quicker. The friction of two objects are combined with the formula:
    // srt(a, b)
    float Friction = 1; 

    ZilchDeclareDerivedType(PhysicsMaterial, Resource);
    PhysicsMaterial(std::string name);
    ~PhysicsMaterial() {}
    static std::string Extension() { return ".PhysicsMaterial"; }
    static PhysicsMaterialRawPtr Find(std::string);

  };

}