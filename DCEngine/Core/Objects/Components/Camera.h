/******************************************************************************/
/*!
\file   Camera.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/2/2015
\brief  The camera class.
\note   Resource: "http://learnopengl.com/#!Getting-started/Camera"

*/
/******************************************************************************/
#pragma once
#include "..\Component.h"

#include "..\..\..\Dependencies\include\GLM\glm.hpp"
#include "..\..\..\Dependencies\include\GLM\gtc\matrix_transform.hpp"
#include "..\..\..\Dependencies\include\GLEW\glew.h"

namespace DCEngine {

  class Camera : public Component {
  public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera Options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Returns view matrix calculated using Euler Angles and LookAt Matrix
    glm::mat4 GetViewMatrix(); 
        

  private:
    // Calculates the front vector from the Camera's (updated) Euler angles
    void UpdateCameraVectors();


    float FieldOfView;
    float NearPlane;
    float FarPlane;
    float Size; //!< Size of the orthographic projection


  };


}