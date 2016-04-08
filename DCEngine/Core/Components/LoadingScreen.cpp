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

      // DCTrace << "Loading screeen!!!";
      static auto& resources = Daisy->getSystem<Systems::Content>()->LoadedGraphicalResources();
      auto progressText = resources.LastLoaded + " - [" + std::to_string(resources.NumLoaded) + "/" 
                          + std::to_string(resources.NumTotal) + "]";
      TextProgress->setText(progressText);
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
      ProjectTitleObj->getComponent<Transform>()->setTranslation(Vec3(-10, 25, 0));
      ProjectTitleText = dynamic_cast<SpriteText*>(ProjectTitleObj->AddComponentByName("SpriteText"));
      ProjectTitleText->setText("Loading 'Rebound'");
      ProjectTitleText->setFontSize(2);
      // Progress Text
      TextProgressObj = SpaceRef->CreateObject();
      ProjectTitleObj->getComponent<Transform>()->setTranslation(Vec3(-10, -5, 0));
      TextProgress = dynamic_cast<SpriteText*>(TextProgressObj->AddComponentByName("SpriteText"));
      TextProgress->setFontSize(1);
      // Progress Bar
    }

  }
}