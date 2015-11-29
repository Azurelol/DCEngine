/******************************************************************************/
/*!
\file   Camera.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/15/2015
\brief  The camera class.
\note   Resource: "http://learnopengl.com/#!Getting-started/Camera"

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
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

  class CameraViewport;
	class Camera : public Component {
    friend class CameraViewport;
  public:

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(Camera, Component);
    #endif

    /* Property-methods */
    Real getFieldOfView() const;
    void setFieldOfView(Real);
    Real getNearPlane() const;
    void setNearPlane(Real);
    Real getFarPlane() const;
    void setFarPlane(Real);
    Real getSize() const;
    void setSize(Real);

    DCE_DEFINE_PROPERTY(bool, Active);

    // Camera Attributes
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		// Euler Angles(Yaw and Pitch are not needed in 2D camera)
    GLfloat Yaw = 90.0f;
    GLfloat Pitch = 0.0f;;
    GLfloat Roll = 90.0f; // rotation degree
    GLfloat BaseRollVal = 90.0f;
		//Projection Mode
		ProjectionMode Projection = ProjectionMode::Perspective;
		GLfloat FieldOfView = 250;
		GLfloat WindowWidth = 8;
		GLfloat WindowHeight = 6;
		GLfloat Size = 90; // Size of th orthographic projection. 
                       // 0 - 100 **need a if statement to control outranged value
		//Clipping plane
		GLfloat NearPlane = 0.1f;
		GLfloat FarPlane = 100.0f;
    Transform *TransformComponent;


    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
		Camera(Entity& owner);
    //Camera(const Camera& rhs) { DCTrace << "Camera::COPY??/n"; }
    ~Camera();
		void Initialize();
		void OnLogicUpdate(Events::LogicUpdate* event);
		
	private:
    // References to the Window context's settings
    unsigned int* ScreenWidth;
    unsigned int* ScreenHeight;
    bool Active;
    		
		void Update();
		// Calculates the front vector from the Camera's (updated) Euler angles
		void UpdateCameraVectors();


	};


}
