/******************************************************************************/
/*!
\file   CameraViewport.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its 
        projection matrix.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "Camera.h"

namespace DCEngine {

  class CameraViewport : public Component {
  public:

    /* Properties */
    bool Active = true;
    bool Blocking = true;
    int Layer; // Layer index relative to other active viewporrts. 
               // Viewports will be drawn from lowest to highest.
    bool Background; // Should the viewport background be transparent.
    Vec2 ViewportResolution; // The current resolution of the viewport

    /* Methods */
    Vec3 ScreenToWorldViewPlane(Vec2 screenPoint, float viewDepth);
    Vec3 ScreenToWorldPlane(Vec2 screenPoint, Vec3 worldPlaneNormal, Vec3 worldPlanePosition);
    Vec2 WorldToScreen(Vec3 worldPoint);
    Vec2 ScreenToViewport(Vec2 screenPoint);
    Vec2 ViewportToScreen(Vec2 viewportPoint);
    Vec2 ViewPlaneSize(float viewDepth);
    void MouseToWorldRay(); // Get the world ray starting from the mouse

    Camera* getCamera() { return CameraObj; }
    void setCamera(Camera*);
    Camera* FindDefaultCamera();
    
    CameraViewport(Entity& owner) : Component(std::string("CameraViewport"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);    

  private:       

    glm::mat4 OrthographicProjection; // Orthographic projection for the camera
    glm::mat4 ViewMatrix;             // View matrix for the camera
    Camera* CameraObj;
    Camera* DefaultCameraObj;
    



  };


}
