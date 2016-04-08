/******************************************************************************/
/*!
\file   EditorSetup.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/21/2015
\brief  This file contains the functions which set up the Editor when it's
        toggled
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
/******************************************************************************/
#include "Editor.h"
// Engine
#include "../../Engine/Engine.h"
#include "../../Components/Camera.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Draws a grid on screen while the editor is enabled.
    */
    /**************************************************************************/
    void Editor::DrawGrid()
    {
      if (!Settings.GridActive)
        return;

      if (!EditorCamera)
        return;

      auto cameraPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
      // The editor grid will always be in front of the camera.
      Vec3 gridStartPos(cameraPos.x, cameraPos.y, 0);
      Real edge = 500; // We want to make sure we draw 'very' from far away??
      unsigned lines = 100;
      // Start drawings from the center of the screen and outwards
      for (unsigned int i = 0; i < lines; ++i) {
        // Draw the horizontal lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - edge, gridStartPos.y + (i * Settings.GridLength), gridStartPos.z),
          Vec3(gridStartPos.x + edge, gridStartPos.y + (i * Settings.GridLength), gridStartPos.z),
          Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - edge, gridStartPos.y - (i * Settings.GridLength), gridStartPos.z),
          Vec3(gridStartPos.x + edge, gridStartPos.y - (i * Settings.GridLength), gridStartPos.z),
          Settings.GridColor);
        // Draw the vertical lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x + (i * Settings.GridLength), gridStartPos.y - edge, gridStartPos.z),
          Vec3(gridStartPos.x + (i * Settings.GridLength), gridStartPos.y + edge, gridStartPos.z),
          Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - (i * Settings.GridLength), gridStartPos.y - edge, gridStartPos.z),
          Vec3(gridStartPos.x - (i * Settings.GridLength), gridStartPos.y + edge, gridStartPos.z),
          Settings.GridColor);
      }
    }

    /**************************************************************************/
    /*!
    @brief Toggles lightning on and off.
    */
    /**************************************************************************/
    void Editor::ToggleLightning()
    {

    }

    /**************************************************************************/
    /*!
    @brief Resizes the size of the rendering window while in editor mode.
    */
    /**************************************************************************/
    void Editor::ApplyEditorWindowLayout()
    {
      auto viewportResize = new Events::WindowResize();
      if (Settings.EditorEnabled) {
        viewportResize->Dimensions.x = 1;
        viewportResize->Dimensions.y = 1;
      }
      else {
        viewportResize->Dimensions.x = Settings.ViewportResize.x;
        viewportResize->Dimensions.y = Settings.ViewportResize.y;
      }
      Daisy->Dispatch<Events::WindowResize>(viewportResize);
      delete viewportResize;
    }

    /**************************************************************************/
    /*!
    @brief Constructs and sets the space's default camera to be the Editor's
    @todo  L57: Currently not deleting the editor camera when switching. It breaks!
    */
    /**************************************************************************/
    void Editor::SetEditorCamera(bool set)
    {
      // If the editor camera needs to be set
      if (set) {
        // If there's a previous editor camera, do nothing
        if (EditorCamera)
          return;

        //DCTrace << "Editor::SetEditorCamera - Setting the editor camera. \n";
        auto editorCamera = Daisy->getSystem<Systems::Factory>()->CreateGameObject("EditorCamera", *CurrentSpace, false);
        editorCamera->setLocked(true);
        editorCamera->AddComponentByName("Camera", true);
        editorCamera->AddComponentByName("EditorCameraController", true);
        // Whether the editor's transform tool is a component
        if (Settings.TransformTool_IsComponent)
          editorCamera->AddComponent<Components::TransformTool>(true);
        editorCamera->getComponent<Components::Transform>()->Translation = Vec3(0.0f, 0.0f, 40.0f);
        editorCamera->getComponent<Components::Camera>()->setSize(70);
        editorCamera->getComponent<Components::Camera>()->setFarPlane(500);
        editorCamera->getComponent<Components::Camera>()->Projection = ProjectionMode::Perspective;
        // Add the camera to the current space
        CurrentSpace->AddObject(editorCamera);
        EditorCamera = editorCamera;
        // Set it as the default camera on the space
        auto cameraComp = editorCamera->getComponent<Components::Camera>();
        CurrentSpace->getComponent<Components::CameraViewport>()->setCamera(cameraComp);
        // Set it to last position
        EditorCamera->getComponent<Components::Transform>()->setTranslation(Settings.CameraLastPos);
      }
      // If the editor camera needs to be removed
      else {
        //DCTrace << "Editor::SetEditorCamera - Removing the editor camera. \n";
        // Save the last position of the editor's camera
        Settings.CameraLastPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
        // Look for a camera on the space to be default
        auto defaultcam = CurrentSpace->getComponent<Components::CameraViewport>()->FindDefaultCamera();
        // Set it as the default camera
        CurrentSpace->getComponent<Components::CameraViewport>()->setCamera(defaultcam);
        // Remove the editor camera from the space
        auto editorCamera = CurrentSpace->FindObjectByName("EditorCamera");

        if (editorCamera)
          editorCamera->Destroy();
        EditorCamera = nullptr;
      }
      Deselect();
    }

    /**************************************************************************/
    /*!
    @brief Updates the current window's caption!
    */
    /**************************************************************************/
    void Editor::UpdateCaption()
    {
      // Get the current project's name
      auto projectName = Settings.ProjectProperties->ProjectName;
      std::string levelName;
      if (CurrentSpace->CurrentLevel())
        levelName = CurrentSpace->CurrentLevel()->getObjectName();

      DispatchSystemEvents::SetWindowCaption(projectName + " - Level: " + levelName + " - Daisy Chain Engine");
    }



  }
}