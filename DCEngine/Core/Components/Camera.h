/******************************************************************************/
/*!
\file   Camera.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/15/2015
\brief  The camera class.
\note   Resource: "http://learnopengl.com/#!Getting-started/Camera"

*/
/******************************************************************************/

/**************************************************************/
/*
9/14 - Camera Projection done. (not tested)
Log: Change Projection to Orthographic or Perspective when
needed.

9/18
Log: Deleted two variables width and height. Replaced by size.
     Camera shouuld be set to square and changed with screen
   or windows resolution.
*/
/**************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Engine/Types.h"
#include "Transform.h"

namespace DCEngine {

  enum class ProjectionMode {
    Orthographic = 0,
    Perspective,
  };

  class Camera : public Component {
  public:

    // Camera Attributes
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    // Euler Angles(not needed in 2D camera)
    GLfloat Yaw;
    GLfloat Pitch;
    GLfloat Roll; // rotation degree
    //Projection Mode
    ProjectionMode Projection = ProjectionMode::Orthographic;
    GLfloat FieldOfView = 45;
    GLfloat CameraWidth = 800;
    GLfloat CameraHeight = 600;
    GLfloat Size = 20;
    //Clipping plane
    GLfloat NearPlane = 1.0f;
    GLfloat FarPlane = 10.0f;
        
    Camera(Entity& owner);
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    //Temporary update function
    void TransformUpdate(glm::vec3 &PositionInput, GLfloat RotationDegree);//

  private:

    //(Need coordinates system, it is a temporary function)
    glm::vec3 Position;

    // Returns view matrix calculated using Euler Angles and LookAt Matrix;
	  void Update();
	

  private:
	Transform *Transform_;
    // Calculates the front vector from the Camera's (updated) Euler angles
    void UpdateCameraVectors();


  };


}
