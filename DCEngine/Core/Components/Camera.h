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
		// Euler Angles(Yaw and Pitch are not needed in 2D camera)
		GLfloat Yaw;
		GLfloat Pitch;
		GLfloat Roll; // rotation degree
		//Projection Mode
		ProjectionMode Projection = ProjectionMode::Orthographic;
		GLfloat FieldOfView = 45;
		GLfloat WindowsWidth = 8;
		GLfloat WindowsHeight = 6;
		GLfloat Size = 90;//0 - 100 **need a if statement to control outranged value
		//Clipping plane
		GLfloat NearPlane = 0.1f;
		GLfloat FarPlane = 100.0f;

		Camera(Entity& owner);
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		void OnLogicUpdate(Events::LogicUpdate* event);
		Transform *Transform_;

	private:


		// Returns view matrix calculated using Euler Angles and LookAt Matrix;
		void Update();
		// Calculates the front vector from the Camera's (updated) Euler angles
		void UpdateCameraVectors();


	};


}
