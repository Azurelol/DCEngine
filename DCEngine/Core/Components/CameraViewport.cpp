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
  @todo   Currently looking for the first Camera in the space.
  /**************************************************************************/
  void CameraViewport::setCamera() {
    // Look for a GameObject with the 'Camera' component in the space
    for (auto gameObj : *SpaceRef->AllObjects()) {
      auto camera = gameObj->getComponent<Camera>();
      if (camera != nullptr)
        CameraObj = camera;
    }

    //CameraObj = SpaceRef->FindObjectByName("Camera")->getComponent<Camera>();
  }
}