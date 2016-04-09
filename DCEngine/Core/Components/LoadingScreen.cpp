/*****************************************************************************/
/*!
\file   LoadingScreen.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   4/8/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "LoadingScreen.h"

#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief  LoadingScreen ZilchDefinition.
    */
    /**************************************************************************/
    ZilchDefineType(LoadingScreen, "LoadingScreen", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(LoadingScreen);      
    }

    /**************************************************************************/
    /*!
    @brief  LoadingScreen constructor.
    */
    /**************************************************************************/
    LoadingScreen::LoadingScreen(Entity & owner) : Component(std::string("LoadingScreen"), owner)
    {      
    }

    /**************************************************************************/
    /*!
    @brief  LoadingScreen destructor.
    */
    /**************************************************************************/
    LoadingScreen::~LoadingScreen()
    {
    }

    /**************************************************************************/
    /*!
    @brief  LoadingScreen Initializer.
    */
    /**************************************************************************/
    void LoadingScreen::Initialize()
    {
      // Subscribe to events
      Daisy->Connect<Events::FrameUpdate>(SpaceRef, &LoadingScreen::OnFrameUpdateEvent, this);
      // Construct the screen
      ConstructScreen();
    }

    /**************************************************************************/
    /*!
    @brief  The loadingscreen is updated per frame.
    */
    /**************************************************************************/
    void LoadingScreen::OnFrameUpdateEvent(Events::FrameUpdate * event)
    {
      Update(event->Dt);
    }

    /**************************************************************************/
    /*!
    @brief  The loadingscreen is updated per frame.
    */
    /**************************************************************************/
    void LoadingScreen::Update(float dt)
    {
      // Update the progress of the loading
      static auto& resources = Daisy->getSystem<Systems::Content>()->LoadedGraphicalResources();
      auto progressText = resources.LastLoaded + " - [" + std::to_string(resources.NumLoaded) + "/" 
                          + std::to_string(resources.NumTotal) + "]";
      TextProgress->setText(progressText);

      // Rotate the animation object
      static auto rotFactor = 20;
      auto currentRotation = AnimationObj->getComponent<Transform>()->getRotation();
      AnimationObj->getComponent<Transform>()->setRotation(Vec3(0, -0, currentRotation.z * 1 + dt * rotFactor));

    }

    /**************************************************************************/
    /*!
    @brief  Constructs the loading screen programmatically. Ho!
    */
    /**************************************************************************/
    void LoadingScreen::ConstructScreen()
    {
      auto& editor = Daisy->getSystem<Systems::Editor>();
      
      //-----------------------------------------------------------------------//
      // Title Text
      ProjectTitleObj = SpaceRef->CreateObject();
      ProjectTitleObj->getComponent<Transform>()->setTranslation(Vec3(0, 25, 0));
      ProjectTitleText = dynamic_cast<SpriteText*>(ProjectTitleObj->AddComponentByName("SpriteText"));
      ProjectTitleText->setText("Loading 'Rebound'");
      ProjectTitleText->setFontSize(30);
      // Progress Text
      TextProgressObj = SpaceRef->CreateObject();
      TextProgressObj->getComponent<Transform>()->setTranslation(Vec3(-10, -5, 0));
      TextProgress = dynamic_cast<SpriteText*>(TextProgressObj->AddComponentByName("SpriteText"));
      TextProgress->setFontSize(12);
      // Progress Bar

      // Animation Object
      AnimationObj = SpaceRef->CreateObject();
      AnimationObj->getComponent<Transform>()->setTranslation(Vec3(25, -20, 0));
      AnimationObj->getComponent<Transform>()->setScale(Vec3(25, 25, 0));
      auto animationObjSprite = dynamic_cast<Sprite*>(AnimationObj->AddComponentByName("Sprite"));
      animationObjSprite->setSpriteSource("Jotaro");
    }

  }
}