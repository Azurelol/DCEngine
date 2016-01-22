/******************************************************************************/
/*!
@file   Light.h
@author William Mao, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/20/2015
@brief  Light component for dynamic scene lightning.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components {

    enum class LightType {
      Point,
      Spot,
      Directional,
      Box
    };

    class Light : public Component {
    public:

      /*=============
         Properties
      ==============*/
      // If the light should be rendered
      DCE_DEFINE_PROPERTY(bool, Visible);
      // If the object is culled and not graphically updated when its not visible
      DCE_DEFINE_PROPERTY(bool, VisibilityCulling);
      // If the object receives events when entering/exiting the view of a rendering camera or shadow
      DCE_DEFINE_PROPERTY(bool, VisibilityEvents);
      // If the object is rendered to shadow maps
      DCE_DEFINE_PROPERTY(bool, CastShadows);
      // The type of the light (Point, Spot, Directional, etc...)

      // The color of the light.
      DCE_DEFINE_PROPERTY(Vec4, Color);
      // The intensity of the light
      DCE_DEFINE_PROPERTY(float, Intensity);
      // The maximum range that the light can reach.
      DCE_DEFINE_PROPERTY(float, Range);
      // Falloff exponent. 1 is linear.
      DCE_DEFINE_PROPERTY(float, Falloff);
      // The angle of the light
      DCE_DEFINE_PROPERTY(float, Angle);
      DCE_DEFINE_PROPERTY(float, Size);
      
      ZilchDeclareDerivedType(Light, Component);
      Light(Entity& owner);
      ~Light();
      void Initialize();

    private:
      bool Visible; 
      bool VisibilityCulling;
      bool VisibilityEvents; 
      bool CastShadows;
      LightType LightType;
      Vec4 Color;
      float Intensity;
      float Range;
      float Falloff;
      float Angle;
      float Size;

    };


  }
}