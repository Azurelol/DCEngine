/*****************************************************************************/
/*!
@file   Fade.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Fade.h"
#include "../../CoreComponents.h"

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
/*!
@brief Provides the definition of this class to Zilch.
@note This can only go in the translational unit (.cpp)
*/
/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Fade, "Fade", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Fade);
      DCE_BINDING_DEFINE_PROPERTY(Fade, Timer);
      DCE_BINDING_DEFINE_PROPERTY(Fade, FadeTime);
      DCE_BINDING_DEFINE_PROPERTY(Fade, FadeTimeGrowth);
      DCE_BINDING_DEFINE_PROPERTY(Fade, TargetColor);
      DCE_BINDING_DEFINE_PROPERTY(Fade, Fading);
      DCE_BINDING_DEFINE_PROPERTY(Fade, Pulse);
    }
#endif


    void Fade::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      //Connect(Daisy->getKeyboard(), Events::KeyDown, Fade::OnKeyDownEvent);
      //Connect(Daisy->getKeyboard(), Events::KeyUp, Fade::OnKeyUpEvent);
      //Connect(Daisy->getMouse(), Events::MouseDown, Fade::OnMouseDownEvent);
      //Connect(Daisy->getMouse(), Events::MouseUp, Fade::OnMouseUpEvent);
      //Connect(gameObj, Events::CollisionStarted, Fade::OnCollisionStartedEvent);
      //Connect(gameObj, Events::CollisionEnded, Fade::OnCollisionEndedEvent);
      Connect(SpaceRef, Events::LogicUpdate, Fade::OnLogicUpdateEvent);
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
    }

    void Fade::OnMouseDownEvent(Events::MouseDown * event)
    {

    }

    void Fade::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    void Fade::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }

    void Fade::OnMouseUpEvent(Events::MouseUp * event)
    {

    }

    void Fade::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void Fade::OnKeyUpEvent(Events::KeyDown * event)
    {
    }

    void Fade::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      if (!Fading)
        return;
      if (InitialColor == Vec4(-1.0f, -1.0f, -1.0f, -1.0f))
      {
        InitialColor = SpriteRef->Color;
      }
      Timer += event->Dt;
      if (Timer > FadeTime)
      {
        Timer = FadeTime;
      }

      //SpriteRef->Color = Interpolate(InitialColor, TargetColor, Timer / FadeTime);
      if (SpriteRef->Color == TargetColor)
      {
        Fading = false;
      }
      auto percentDone = Timer / FadeTime;
      //SpriteRef->Color = Interpolate(InitialColor, TargetColor, Timer / MoveToLocationTime);

      SpriteRef->Color = InitialColor * (1 - percentDone) + (TargetColor * percentDone);
      if (SpriteRef->Color == TargetColor)
      {
        if (!Pulse)
        {
          Fading = false;
        }
        Timer = 0;
        FadeTime *= FadeTimeGrowth;
        TargetColor = InitialColor;
        InitialColor = SpriteRef->Color;
      }
    }


  }
}