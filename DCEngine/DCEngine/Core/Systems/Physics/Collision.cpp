#pragma once
#include "Collision.h"
#include "../../Components/PhysicsSpace.h"
#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Engine/Engine.h"

namespace DCEngine
{

	/**************************************************************************/
	/*!
	@brief  This function determines if two 'GameObjects' with the 'BoxCollider'
	component are colliding.
	@param  A pointer to the first object.
	@param  A pointer to the second object.
	@return True if a collision was detected, false otherwise.
	*/
	/**************************************************************************/
	bool BoxtoBox(GameObject * obj1, GameObject * obj2)
	{
		/* christian if you are looking at this im sorry about the math in here */
		
		/* get the rigidbodies */
		auto rigidbody1 = obj1->getComponent<RigidBody>();
		auto rigidbody2 = obj2->getComponent<RigidBody>();

		/* get the colliders */
		auto boxcollider1 = obj1->getComponent<BoxCollider>();
		auto boxcollider2 = obj2->getComponent<BoxCollider>();

		/* get the transforms */
		auto transform1 = obj1->getComponent<Transform>();
		auto transform2 = obj2->getComponent<Transform>();


		float Min1 = 0, Max1 = 0, Min2 = 0, Max2 = 0;


		glm::vec3 topL1;
		glm::vec3 topR1;
		glm::vec3 botL1;
		glm::vec3 botR1;

		glm::vec3 topL2;
		glm::vec3 topR2;
		glm::vec3 botL2;
		glm::vec3 botR2;


		glm::vec3 Axis1;
		glm::vec3 Axis2;
		glm::vec3 Axis3;
		glm::vec3 Axis4;


		glm::vec3 ProjvertexTL1;
		glm::vec3 ProjvertexTR1;
		glm::vec3 ProjvertexBL1;
		glm::vec3 ProjvertexBR1;

		glm::vec3 ProjvertexTL2;
		glm::vec3 ProjvertexTR2;
		glm::vec3 ProjvertexBL2;
		glm::vec3 ProjvertexBR2;

		float Height0 = boxcollider1->getSize().y;
		float Height1 = boxcollider2->getSize().y;

		float Width0 = boxcollider1->getSize().x;
		float Width1 = boxcollider2->getSize().x;

		float rot0 = transform1->WorldRotation.z;
		float rot1 = transform2->WorldRotation.z;


		topL1.x = transform1->Translation.x + ((0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
		topL1.y = transform1->Translation.y + ((0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

		topR1.x = transform1->Translation.x + ((0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
		topR1.y = transform1->Translation.y + ((0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

		botL1.x = transform1->Translation.x + ((-0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
		botL1.y = transform1->Translation.y + ((-0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

		botR1.x = transform1->Translation.x + ((-0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
		botR1.y = transform1->Translation.y + ((-0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));
		/*
		*/
		topL2.x = transform2->Translation.x + ((0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
		topL2.y = transform2->Translation.y + ((0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

		topR2.x = transform2->Translation.x + ((0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
		topR2.y = transform2->Translation.y + ((0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

		botL2.x = transform2->Translation.x + ((-0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
		botL2.y = transform2->Translation.y + ((-0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

		botR2.x = transform2->Translation.x + ((-0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
		botR2.y = transform2->Translation.y + ((-0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

		/* this block of math above calculates the veracities of the rectangles */

		Axis1.x = topR1.x - topL1.x;
		Axis1.y = topR1.y - topL1.y;

		Axis2.x = topR1.x - botR1.x;
		Axis2.y = topR1.y - botR1.y;

		Axis3.x = topL2.x - botL2.x;
		Axis3.y = topL2.y - botL2.y;

		Axis4.x = topL2.x - topR2.x;
		Axis4.y = topL2.y - topR2.y;

			/* this ^ calculates the 4 axis I will be using to determine collision */

		ProjvertexTL1.x = ((topL1.x * Axis1.x + topL1.y * Axis1.y) / (glm::dot(Axis1, Axis1)) * Axis1.x);
		ProjvertexTL1.y = ((topL1.x * Axis1.x + topL1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;

		ProjvertexTR1.x = ((topR1.x * Axis1.x + topR1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexTR1.y = ((topR1.x * Axis1.x + topR1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;

		ProjvertexBL1.x = ((botL1.x * Axis1.x + botL1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexBL1.y = ((botL1.x * Axis1.x + botL1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;

		ProjvertexBR1.x = ((botR1.x * Axis1.x + botR1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexBR1.y = ((botR1.x * Axis1.x + botR1.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;
		/*
		*/
		ProjvertexTL2.x = ((topL2.x * Axis1.x + topL2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexTL2.y = ((topL2.x * Axis1.x + topL2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;
																			   	   
		ProjvertexTR2.x = ((topR2.x * Axis1.x + topR2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexTR2.y = ((topR2.x * Axis1.x + topR2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;
																			   	   
		ProjvertexBL2.x = ((botL2.x * Axis1.x + botL2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexBL2.y = ((botL2.x * Axis1.x + botL2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;
																			   	   
		ProjvertexBR2.x = ((botR2.x * Axis1.x + botR2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.x;
		ProjvertexBR2.y = ((botR2.x * Axis1.x + botR2.y * Axis1.y) / (glm::dot(Axis1, Axis1))) * Axis1.y;

		Min1 = glm::dot(ProjvertexTL1, Axis1);
		Max1 = Min1;

		if (glm::dot(ProjvertexTR1, Axis1) > Max1)
		{
			Max1 = glm::dot(ProjvertexTR1, Axis1);
		}
		else if (glm::dot(ProjvertexTR1, Axis1) < Min1)
		{
			Min1 = glm::dot(ProjvertexTR1, Axis1);
		}

		if (glm::dot(ProjvertexBR1, Axis1) > Max1)
		{
			Max1 = glm::dot(ProjvertexBR1, Axis1);
		}
		else if (glm::dot(ProjvertexBR1, Axis1) < Min1)
		{
			Min1 = glm::dot(ProjvertexBR1, Axis1);
		}

		if (glm::dot(ProjvertexBL1, Axis1) > Max1)
		{
			Max1 = glm::dot(ProjvertexBL1, Axis1);
		}
		else if (glm::dot(ProjvertexBL1, Axis1) < Min1)
		{
			Min1 = glm::dot(ProjvertexBL1, Axis1);
		}



		Min2 = glm::dot(ProjvertexTL2, Axis1);
		Max2 = Min2;

		if (glm::dot(ProjvertexTR2, Axis1) > Max2)
		{
			Max2 = glm::dot(ProjvertexTR2, Axis1);
		}
		else if (glm::dot(ProjvertexTR2, Axis1) < Min2)
		{
			Min2 = glm::dot(ProjvertexTR2, Axis1);
		}

		if (glm::dot(ProjvertexBR2, Axis1) > Max2)
		{
			Max2 = glm::dot(ProjvertexBR2, Axis1);
		}
		else if (glm::dot(ProjvertexBR2, Axis1) < Min2)
		{
			Min2 = glm::dot(ProjvertexBR2, Axis1);
		}

		if (glm::dot(ProjvertexBL2, Axis1) > Max2)
		{
			Max2 = glm::dot(ProjvertexBL2, Axis1);
		}
		else if (glm::dot(ProjvertexBL2, Axis1) < Min2)
		{
			Min2 = glm::dot(ProjvertexBL2, Axis1);
		}

		if (Min1 > Max2 || Min2 > Max1)
		{
			return 0;
		}
		/**********************************************************/
		/**********************************************************/
		/**********************************************************/
		ProjvertexTL1.x = ((topL1.x * Axis2.x + topL1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexTL1.y = ((topL1.x * Axis2.x + topL1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexTR1.x = ((topR1.x * Axis2.x + topR1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexTR1.y = ((topR1.x * Axis2.x + topR1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexBL1.x = ((botL1.x * Axis2.x + botL1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexBL1.y = ((botL1.x * Axis2.x + botL1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexBR1.x = ((botR1.x * Axis2.x + botR1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexBR1.y = ((botR1.x * Axis2.x + botR1.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;
		/*
		*/
		ProjvertexTL2.x = ((topL2.x * Axis2.x + topL2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexTL2.y = ((topL2.x * Axis2.x + topL2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexTR2.x = ((topR2.x * Axis2.x + topR2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexTR2.y = ((topR2.x * Axis2.x + topR2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexBL2.x = ((botL2.x * Axis2.x + botL2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexBL2.y = ((botL2.x * Axis2.x + botL2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		ProjvertexBR2.x = ((botR2.x * Axis2.x + botR2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.x;
		ProjvertexBR2.y = ((botR2.x * Axis2.x + botR2.y * Axis2.y) / (glm::dot(Axis2, Axis2))) * Axis2.y;

		Min1 = glm::dot(ProjvertexTL1, Axis2);
		Max1 = Min1;

		if (glm::dot(ProjvertexTR1, Axis2) > Max1)
		{
			Max1 = glm::dot(ProjvertexTR1, Axis2);
		}
		else if (glm::dot(ProjvertexTR1, Axis2) < Min1)
		{
			Min1 = glm::dot(ProjvertexTR1, Axis2);
		}

		if (glm::dot(ProjvertexBR1, Axis2) > Max1)
		{
			Max1 = glm::dot(ProjvertexBR1, Axis2);
		}
		else if (glm::dot(ProjvertexBR1, Axis2) < Min1)
		{
			Min1 = glm::dot(ProjvertexBR1, Axis2);
		}

		if (glm::dot(ProjvertexBL1, Axis2) > Max1)
		{
			Max1 = glm::dot(ProjvertexBL1, Axis2);
		}
		else if (glm::dot(ProjvertexBL1, Axis2) < Min1)
		{
			Min1 = glm::dot(ProjvertexBL1, Axis2);
		}



		Min2 = glm::dot(ProjvertexTL2, Axis2);
		Max2 = Min2;

		if (glm::dot(ProjvertexTR2, Axis2) > Max2)
		{
			Max2 = glm::dot(ProjvertexTR2, Axis2);
		}
		else if (glm::dot(ProjvertexTR2, Axis2) < Min2)
		{
			Min2 = glm::dot(ProjvertexTR2, Axis2);
		}

		if (glm::dot(ProjvertexBR2, Axis2) > Max2)
		{
			Max2 = glm::dot(ProjvertexBR2, Axis2);
		}
		else if (glm::dot(ProjvertexBR2, Axis2) < Min2)
		{
			Min2 = glm::dot(ProjvertexBR2, Axis2);
		}

		if (glm::dot(ProjvertexBL2, Axis2) > Max2)
		{
			Max2 = glm::dot(ProjvertexBL2, Axis2);
		}
		else if (glm::dot(ProjvertexBL2, Axis2) < Min2)
		{
			Min2 = glm::dot(ProjvertexBL2, Axis2);
		}

		if (Min1 > Max2 || Min2 > Max1)
		{
			return 0;
		}
		/**********************************************************/
		/**********************************************************/
		/**********************************************************/

		ProjvertexTL1.x = ((topL1.x * Axis3.x + topL1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexTL1.y = ((topL1.x * Axis3.x + topL1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexTR1.x = ((topR1.x * Axis3.x + topR1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexTR1.y = ((topR1.x * Axis3.x + topR1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexBL1.x = ((botL1.x * Axis3.x + botL1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexBL1.y = ((botL1.x * Axis3.x + botL1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexBR1.x = ((botR1.x * Axis3.x + botR1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexBR1.y = ((botR1.x * Axis3.x + botR1.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
		/*																			  
		*/																			  
		ProjvertexTL2.x = ((topL2.x * Axis3.x + topL2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexTL2.y = ((topL2.x * Axis3.x + topL2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexTR2.x = ((topR2.x * Axis3.x + topR2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexTR2.y = ((topR2.x * Axis3.x + topR2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexBL2.x = ((botL2.x * Axis3.x + botL2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexBL2.y = ((botL2.x * Axis3.x + botL2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;
																					  
		ProjvertexBR2.x = ((botR2.x * Axis3.x + botR2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.x;
		ProjvertexBR2.y = ((botR2.x * Axis3.x + botR2.y * Axis3.y) / (glm::dot(Axis3, Axis3))) * Axis3.y;

		Min1 = glm::dot(ProjvertexTL1, Axis3);
		Max1 = Min1;

		if (glm::dot(ProjvertexTR1, Axis3) > Max1)
		{
			Max1 = glm::dot(ProjvertexTR1, Axis3);
		}
		else if (glm::dot(ProjvertexTR1, Axis3) < Min1)
		{
			Min1 = glm::dot(ProjvertexTR1, Axis3);
		}

		if (glm::dot(ProjvertexBR1, Axis3) > Max1)
		{
			Max1 = glm::dot(ProjvertexBR1, Axis3);
		}
		else if (glm::dot(ProjvertexBR1, Axis3) < Min1)
		{
			Min1 = glm::dot(ProjvertexBR1, Axis3);
		}

		if (glm::dot(ProjvertexBL1, Axis3) > Max1)
		{
			Max1 = glm::dot(ProjvertexBL1, Axis3);
		}
		else if (glm::dot(ProjvertexBL1, Axis3) < Min1)
		{
			Min1 = glm::dot(ProjvertexBL1, Axis3);
		}



		Min2 = glm::dot(ProjvertexTL2, Axis3);
		Max2 = Min2;

		if (glm::dot(ProjvertexTR2, Axis3) > Max2)
		{
			Max2 = glm::dot(ProjvertexTR2, Axis3);
		}
		else if (glm::dot(ProjvertexTR2, Axis3) < Min2)
		{
			Min2 = glm::dot(ProjvertexTR2, Axis3);
		}

		if (glm::dot(ProjvertexBR2, Axis3) > Max2)
		{
			Max2 = glm::dot(ProjvertexBR2, Axis3);
		}
		else if (glm::dot(ProjvertexBR2, Axis3) < Min2)
		{
			Min2 = glm::dot(ProjvertexBR2, Axis3);
		}

		if (glm::dot(ProjvertexBL2, Axis3) > Max2)
		{
			Max2 = glm::dot(ProjvertexBL2, Axis3);
		}
		else if (glm::dot(ProjvertexBL2, Axis3) < Min2)
		{
			Min2 = glm::dot(ProjvertexBL2, Axis3);
		}

		if (Min1 > Max2 || Min2 > Max1)
		{
			return false;
		}
		/**********************************************************/
		/**********************************************************/
		/**********************************************************/

		return true;

	}

	/**************************************************************************/
	/*!
	@brief  This function determines if two 'GameObjects', one with a
	'BoxCollider' component and the other with a 'SphereCollider'
	component are colliding.
	@param  A pointer to the first object with the 'BoxCollider'.
	@param  A pointer to the second object with the 'SphereCollider'.
	@return True if a collision was detected, false otherwise.
	*/
	/**************************************************************************/
	bool CircletoBox(GameObject * obj1, GameObject * obj2)
	{
		return false;
	}

	/**************************************************************************/
	/*!
	@brief  This function determines if two 'GameObjects' with the
	'SphereCollider' component are colliding.
	@param  A pointer to the first object.
	@param  A pointer to the second object.
	@return True if a collision was detected, false otherwise.
	*/
	/**************************************************************************/
	bool CircletoCircle(GameObject * obj1, GameObject * obj2)
	{
		return false;
	}
}