/******************************************************************************/
/*!
@file   EditorProjects.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EditorLauncher.h"

#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief EditorLauncher constructor.
    */
    /**************************************************************************/
    EditorLauncher::EditorLauncher() : EditorModule(false)
    {
    }

    /**************************************************************************/
    /*!
    \brief EditorLauncher destructor.
    */
    /**************************************************************************/
    EditorLauncher::~EditorLauncher()
    {
    }
    
    /**************************************************************************/
    /*!
    \brief Launches the Editor.
    */
    /**************************************************************************/
    void EditorLauncher::Launch()
    {
      DCTrace << "EditorLauncher::Launch \n";
      Access().setEnabled(true);
      // Pause the engine (Physics, Input, Events)
      DispatchSystemEvents::EnginePause();
      // Quit the Game
      DispatchGameEvents::GameEnded();
      // Toggle the widgets
      Access().Windows.LibraryEnabled = true;
      Access().Windows.ObjectsEnabled = true;
      // Clear previous commands
      Access().Settings.Commands.CommandsCurrent.clear();
      Access().Settings.Commands.CommandsUndo.clear();
      // Add the Editor camera
      CreateEditorCamera();

      Access().Deselect();
    }

    /**************************************************************************/
    /*!
    \brief Closes the Editor.
    */
    /**************************************************************************/
    void EditorLauncher::Close()
    {
      DCTrace << "EditorLauncher::Close \n";
      Access().setEnabled(false);
      // Unpause the engine (Physics, Input, Events)
      DispatchSystemEvents::EngineResume();
      // Send the game start event
      DispatchGameEvents::GameStarted();
      // Set the editor camera
      RemoveEditorCamera();

      Access().Deselect();
    }

    /**************************************************************************/
    /*!
    \brief Sets the Editor camera.
    \param set Whether the camera should be set on the space.
    */
    /**************************************************************************/
    void EditorLauncher::SetEditorCamera(bool set)
    {
      if (set)
        CreateEditorCamera();
      else
        RemoveEditorCamera();

      Access().Deselect();
    }

    /**************************************************************************/
    /*!
    \brief  Creates the Editor camera.
    */
    /**************************************************************************/
    void EditorLauncher::CreateEditorCamera()
    {
      auto editorCamera = Access().CurrentSpace->CreateObject();
      editorCamera->setObjectName("EditorCamera");
      editorCamera->setLocked(true);
      editorCamera->setProtected(true);
      editorCamera->AddComponentByName("Camera", true);
      editorCamera->AddComponentByName("EditorCameraController", true);
      editorCamera->getComponent<Components::Transform>()->Translation = Vec3(0.0f, 0.0f, 40.0f);
      editorCamera->getComponent<Components::Camera>()->setSize(70);
      editorCamera->getComponent<Components::Camera>()->setFarPlane(500);
      editorCamera->getComponent<Components::Camera>()->Projection = ProjectionMode::Perspective;
      // Set the pointer
      Access().EditorCamera = editorCamera;
      // Set it as the default camera on the space
      auto cameraComp = editorCamera->getComponent<Components::Camera>();
      Access().CurrentSpace->getComponent<Components::CameraViewport>()->setCamera(cameraComp);
      // Set it to last position
      Access().EditorCamera->getComponent<Components::Transform>()->setTranslation(Access().Settings.CameraLastPos);
    }

    /**************************************************************************/
    /*!
    \brief  Removes the current Editor camera.
    */
    /**************************************************************************/
    void EditorLauncher::RemoveEditorCamera()
    {
      auto editorCam = Access().EditorCamera;
      if (!editorCam) {
        //throw DCException("Tried to remove the Editor Camera when there wasn't one!");
      }

      // Save the last position of the editor's camera
      Access().Settings.CameraLastPos = editorCam->getComponent<Components::Transform>()->getTranslation();
      // Look for a camera on the space to be default
      auto defaultcam = Access().CurrentSpace->getComponent<Components::CameraViewport>()->FindDefaultCamera();
      // Set it as the default camera
      Access().CurrentSpace->getComponent<Components::CameraViewport>()->setCamera(defaultcam);
      // Remove the editor camera from the space
      // auto editorCamera = Access().CurrentSpace->FindObjectByName("EditorCamera");
      Access().EditorCamera = nullptr;
    }

  }
}