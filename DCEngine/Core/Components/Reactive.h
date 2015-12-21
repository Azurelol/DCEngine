/******************************************************************************/
/*!
@file   Reactive.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/22/2015
@brief  The reactive component allows the object to react to mouse events. 
        Uses the collision volume on its collider to test for picking.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class Reactive : public Component {
    public:
#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Reactive, Component);
#endif

      // Properties   
      DCE_DEFINE_PROPERTY(Boolean, Active);
      DCE_DEFINE_PROPERTY(Boolean, CaptureMouse);
      DCE_DEFINE_PROPERTY(Boolean, Noise);
      // Methods
      Reactive(Entity& owner);
      ~Reactive();
      void Initialize();
      void CheckMousePosition(Vec2);
      void DispatchMouseEnterEvent();
      void DispatchMouseExitEvent();
      void DispatchMouseClickedOnEvent();
      // Events
      void OnLogicUpdate(Events::LogicUpdate* event);
      void OnMouseUpdateEvent(Events::MouseUpdate* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnMouseScrollEvent(Events::MouseScroll* event);

    private:

      Vec2 LastPosition;
      Boolean Active;
      Boolean CaptureMouse;
      Boolean MouseEntered;
      Boolean Noise = false;

    };


  }
}