/*****************************************************************************/
/*!
\file   LoadingScreen.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   4/8/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components {

    // Forward reference
    class SpriteText;

    class LoadingScreen : public Component {
      ZilchDeclareDerivedType(LoadingScreen, Component);
      LoadingScreen(Entity& owner);
      void Initialize();
      virtual ~LoadingScreen();

      // Background
      GameObjectPtr BackgroundSpriteObj;
      // Light
      GameObjectPtr LightObj;
      // Title Text
      GameObjectPtr ProjectTitleObj;
      SpriteText* ProjectTitleText;
      // Progress Bar
      GameObjectPtr ProgressBarForeground;
      GameObjectPtr ProgressBarBackground;      
      // Progress Text
      GameObjectPtr TextFilenameObj;
      GameObjectPtr TextProgressObj;
      SpriteText* TextFileName;
      SpriteText* TextProgress;
      // Progress animation
      GameObjectPtr AnimationObj;

    public:

      void OnFrameUpdateEvent(Events::FrameUpdate* event);      
      void Update(float dt);
      void ConstructScreen();


    };

  }
}