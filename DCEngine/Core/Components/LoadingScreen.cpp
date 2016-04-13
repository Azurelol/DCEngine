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

    static Camera* sCamera;
    static bool Construct = false;

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

      if (Construct) {
        CreateCamera();
        CreateText();
      }      
      else {
        SetProgressText();
      }

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
      UpdateProgress();
    }

    void LoadingScreen::CreateCamera()
    {
      // Create a camera on the space
      auto cameraObj = SpaceRef->CreateObject("Camera");
      sCamera = cameraObj->getComponent<Components::Camera>();
      sCamera->TransformComponent->setWorldTranslation(Vec3(0, 0, 1));
      SpaceRef->getComponent<Components::CameraViewport>()->setCamera(sCamera);
    }
    
    void LoadingScreen::SetProgressText()
    {
      TextProgress = SpaceRef->FindObjectByName("ProgressText")->getComponent<SpriteText>();
      TextTitle = SpaceRef->FindObjectByName("TitleText")->getComponent<SpriteText>();
    }

    void LoadingScreen::UpdateProgress()
    {
      if (!TextProgress || !TextTitle)
        return;

      // Update the progress of the loading
      static auto& resources = Daisy->getSystem<Systems::Content>()->LoadedGraphicalResources();
      auto percentage = (static_cast<float>(resources.NumLoaded) / static_cast<float>(resources.NumTotal)) * 100.0f;
      auto progressText = resources.LastLoaded;
      TextTitle->setText("Now loading... [" + std::to_string(static_cast<int>(percentage)) + "%]");
      TextProgress->setText(progressText);
    }

    /**************************************************************************/
    /*!
    @brief  Constructs the loading screen programmatically. Ho!
    */
    /**************************************************************************/
    void LoadingScreen::CreateText()
    {
      auto& editor = Daisy->getSystem<Systems::Editor>();
      
      //-----------------------------------------------------------------------//
      // Title Text
      ProjectTitleObj = SpaceRef->CreateObject();
      ProjectTitleObj->getComponent<Transform>()->setTranslation(Vec3(-15, 5, 0));
      TextTitle = dynamic_cast<SpriteText*>(ProjectTitleObj->AddComponentByName("SpriteText"));      
      TextTitle->setFontSize(24);
      // Progress Text
      TextProgressObj = SpaceRef->CreateObject();
      TextProgressObj->getComponent<Transform>()->setTranslation(Vec3(-10, -2.5, 0));
      TextProgress = dynamic_cast<SpriteText*>(TextProgressObj->AddComponentByName("SpriteText"));
      TextProgress->setFontSize(12);
      //// Animation Object
      //AnimationObj = SpaceRef->CreateObject();
      //AnimationObj->getComponent<Transform>()->setTranslation(Vec3(0, -30, 0));
      //AnimationObj->getComponent<Transform>()->setScale(Vec3(40, 40, 0));
      //auto animationObjSprite = dynamic_cast<Sprite*>(AnimationObj->AddComponentByName("Sprite"));
      //animationObjSprite->setSpriteSource("ReboundLogoLowRes");
      //// Background
      //BackgroundSpriteObj = SpaceRef->CreateObject();
      //BackgroundSpriteObj->getComponent<Transform>()->setTranslation(Vec3(0, 0, -0.5));
      //BackgroundSpriteObj->getComponent<Transform>()->setScale(Vec3(100, 100, 1));
      //auto backgroundSprite = dynamic_cast<Sprite*>(BackgroundSpriteObj->AddComponentByName("Sprite"));
      //backgroundSprite->setColor(Vec4(0, 0, 0, 0));
      //// Light
      //LightObj = SpaceRef->CreateObject();
      //LightSource = dynamic_cast<Light*>(BackgroundSpriteObj->AddComponentByName("Light"));
      //LightSource->setFalloff(1.5);
      //LightSource->setIntensity(5);
      //LightSource->setRange(50);
      
    }



  }
}