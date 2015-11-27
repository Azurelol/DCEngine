/******************************************************************************/
/*!
\file   CameraViewport.cpp
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "CameraViewport.h"
#include "EngineReference.h"

namespace DCEngine {

  void CameraViewport::Serialize(Json::Value & root) {
  }
  void CameraViewport::Deserialize(Json::Value & root) {
  }

  /**************************************************************************/
  /*!
  \brief  Displays the properties of the currently selected object.
  */
  /**************************************************************************/
  void CameraViewport::Initialize() {

    // Temporary: Look for the default camera object by searching
    // through the space's gameobjects and looking for one with the
    // camera component
    FindDefaultCamera();
  }

  /**************************************************************************/
  /*!
  @brief  Converts the Windows screenPos coordinates to OpenGL coordinates.
  @param  screenPoint The mouse position on the screenPos, as pixels with (0,0)
  @return A 2D vector containing the coordinates in world space.
  */
  /**************************************************************************/
  Vec2 CameraViewport::ScreenToViewport(Vec2 screenPoint)
  {    
    // We need to grab the current viewport. The information we need is the
    // starting X and Y position of our GL viewport along with the width and
    // height.
    
    // We need to grab the current viewport. The information we need is the
    // starting X and Y position of our GL viewport along with the width and
    // height.

    auto screenCenter = Vec3(*CameraObj->ScreenWidth / 2, *CameraObj->ScreenHeight / 2, 0);
    auto vecCenter = Vec3(screenPoint, 0) - screenCenter;
    auto angle = (float)(CameraObj->FieldOfView) / 2;
    auto PI = 3.1415926535897f;

    auto distBetweenCameraAndPlane = 1;
    float height = tan(angle / 180 * PI) * distBetweenCameraAndPlane;
    height *= 2;
    float width = height * *CameraObj->ScreenWidth / *CameraObj->ScreenHeight;
    float widthRatio = vecCenter.x / *CameraObj->ScreenWidth;
    float heightRatio = vecCenter.y / *CameraObj->ScreenHeight;
    vecCenter.x = widthRatio * width + CameraObj->TransformComponent->Translation.x;
    vecCenter.y = CameraObj->TransformComponent->Translation.y - heightRatio * height;

    /**********************************************
      1. Viewport Origin and Extent
    **********************************************/
    //// Retrieve the viewport values (X, Y, Width, Height)
    //GLfloat pixelDepth;
    //glReadPixels(screenPoint.x, screenPoint.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &pixelDepth);

    ////auto viewport = glm::vec4(0.0f, 0.0f, CameraObj->WindowWidth, CameraObj->WindowHeight);
    //GLint viewport[4];
    //glGetIntegerv(GL_VIEWPORT, viewport);
    //auto screenPos = glm::vec3(screenPoint.x, viewport[3] - screenPoint.y, pixelDepth);
    //auto view = CameraObj->GetViewMatrix();
    //auto proj = CameraObj->GetProjectionMatrix();

    //auto pos = glm::unProject(screenPos, view, proj, glm::vec4(0, 0, viewport[2], viewport[3]));

    return Vec2(vecCenter.x, vecCenter.y);
  }

  /**************************************************************************/
  /*!
  @brief  Sets the viewport's default camera.
  @param  camera A pointer to the new camera to be used.
  /**************************************************************************/
  void CameraViewport::setCamera(Camera * camera)
  {
    CameraObj = camera;
    // If the camera pointer was invalid... 
    if (camera == nullptr) {
      DCTrace << "\n";
      DCTrace << "CameraViewport::setCamera - No active camera has been set. \n";
      DCTrace << "\n";
      return;
    }
    DCTrace << "\n";
    DCTrace << "CameraViewport::setCamera - Setting " << camera->Owner()->Name() << " as the active camera \n";
    DCTrace << "\n";
  }

  /**************************************************************************/
  /*!
  @brief  Finds this space's default camera.
  @todo   Currently looking for the first Camera in the space.
  /**************************************************************************/
  Camera* CameraViewport::FindDefaultCamera() {
    // Look for a GameObject with the 'Camera' component in the space
    for (auto gameObj : *SpaceRef->AllObjects()) {
      auto camera = gameObj->getComponent<Camera>();
      // Do not set the EditorCamera as the space's default camera.
      if (camera != nullptr && camera->Owner()->Name() != std::string("EditorCamera")) {
        DCTrace << "CameraViewport::FindDefaultCamera - Setting " << camera->Owner()->Name() << " as the default camera \n";
        DefaultCameraObj = camera;
        return DefaultCameraObj;
      }        
    }    
    DCTrace << "CameraViewport::FindDefaultCamera - No camera was found on the space \n";
    return nullptr;
    //CameraObj = SpaceRef->FindObjectByName("Camera")->getComponent<Camera>();
  }
}