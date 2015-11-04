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
    setCamera();
  }

  /**************************************************************************/
  /*!
  @brief  Converts the Windows screenPos coordinates to OpenGL coordinates.
  @param  screenPoint The mouse position on the screenPos, as pixels with (0,0)
          
  */
  /**************************************************************************/
  Vec2 CameraViewport::ScreenToViewport(Vec2 screenPoint)
  {    
    // We need to grab the current viewport. The information we need is the
    // starting X and Y position of our GL viewport along with the width and
    // height.
    
    auto screenCenter = Vec4(1024 / 2, 768 / 2, 0, 1);
    auto vecCenter = Vec4(screenPoint, 0, 1) - screenCenter;
    auto view = CameraObj->GetViewMatrix();
    auto proj = CameraObj->GetProjectionMatrix();
    vecCenter = proj * vecCenter;
    // Translate by the camera's translation in the space
    vecCenter.x += CameraObj->TransformComponent->Translation.x;
    vecCenter.y += CameraObj->TransformComponent->Translation.y;

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
  /**************************************************************************/
  void CameraViewport::setCamera() {
    CameraObj = SpaceRef->FindObjectByName("Camera")->getComponent<Camera>();
  }
}