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

    /* Variables */
    

    /* Functions */
    CameraViewport(Entity& owner) : Component(std::string("CameraViewport"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    Camera* getCamera() { return cameraObj_; }

  private:
    glm::mat4 OrthographicProjection; // Orthographic projection for the camera
    glm::mat4 ViewMatrix; // View matrix for the camera

    Camera* cameraObj_;

    void setCamera();



  };


}
