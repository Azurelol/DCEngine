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
    @brief Resizes the size of the rendering window while in editor mode.
    */
    /**************************************************************************/
    void Editor::ApplyEditorWindowLayout()
    {
      auto viewportResize = new Events::ResizeViewportEvent();
      if (EditorEnabled) {
        viewportResize->viewportScale.x = 1;
        viewportResize->viewportScale.y = 1;
      }
      else {
        viewportResize->viewportScale.x = ViewportResize.x;
        viewportResize->viewportScale.y = ViewportResize.y;
      }
      Daisy->Dispatch<Events::ResizeViewportEvent>(viewportResize);
      delete viewportResize;
    }

    /**************************************************************************/
    /*!
    @brief Constructs and sets the space's default camera to be the Editor's
    @todo  L57: Currently not deleting the editor camera when switching. It breaks!
    */
    /**************************************************************************/
    void Editor::SetEditorCamera()
    {
      if (!EditorEnabled) {
        DCTrace << "Editor::SetEditorCamera - Removing the editor camera. \n";
        // Look for a camera on the space to be default
        auto defaultcam = CurrentSpace->getComponent<CameraViewport>()->FindDefaultCamera();
        // Set it as the default camera
        CurrentSpace->getComponent<CameraViewport>()->setCamera(defaultcam);
        // Remove the editor camera from the space
        auto editorCamera = CurrentSpace->FindObjectByName("EditorCamera");
        editorCamera->Destroy();
        //CurrentSpace->RemoveObject(*editorCamera);
        return;
      }

      DCTrace << "Editor::SetEditorCamera - Setting the editor camera. \n";
      auto editorCamera = Daisy->getSystem<Systems::Factory>()->CreateGameObject("EditorCamera", *CurrentSpace, false);
      editorCamera->AddComponent<DCEngine::Camera>(true);
      editorCamera->AddComponent<DCEngine::EditorCameraController>(true);
      editorCamera->AddComponent<DCEngine::DebugAudio>(true);
      // Camera properties      
      editorCamera->getComponent<DCEngine::DebugAudio>()->Track1 = "soulja";
      editorCamera->getComponent<DCEngine::DebugAudio>()->Track2 = "Halloween 1";
      editorCamera->getComponent<DCEngine::Transform>()->Translation = Vec3(1.0f, 11.0f, 1.0f);
      editorCamera->getComponent<DCEngine::Camera>()->Size = 70;
      editorCamera->getComponent<DCEngine::Camera>()->Projection = ProjectionMode::Perspective;
      // Add the camera to the current space
      CurrentSpace->AddObject(editorCamera);
      // Set it as the default camera on the space
      auto cameraComp = editorCamera->getComponent<DCEngine::Camera>();
      CurrentSpace->getComponent<CameraViewport>()->setCamera(cameraComp);
      
    }


  }
}