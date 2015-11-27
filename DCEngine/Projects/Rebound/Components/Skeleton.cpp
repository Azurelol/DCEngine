/*****************************************************************************/
/*!
@file   Skeleton.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Skeleton.h"
#include "../../CoreComponents.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Skeleton, "Skeleton", DCEngineCore, builder, type) {
    //// Constructor / Destructor
    //ZilchBindConstructor(builder, type, Skeleton, "owner", Entity&);
    //ZilchBindDestructor(builder, type, Skeleton);
    //// Properties
    //ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
    //ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
    //ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
  }
#endif


	void Skeleton::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, Skeleton::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, Skeleton::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, Skeleton::OnLogicUpdateEvent);
	}

	void Skeleton::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void Skeleton::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
	}

	void Skeleton::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void Skeleton::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void Skeleton::OnKeyDownEvent(Events::KeyDown * event)
	{
	}

	void Skeleton::OnKeyUpEvent(Events::KeyDown * event)
	{
	}

	void Skeleton::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{

	}

  
}