/*****************************************************************************/
/*!
@file   Background.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Dollhouse.h"

namespace DCEngine {

	/* Chen, write your tests here. */
	void DollHouse::SetupBackground()
	{
		// Reference to the factory system
		auto factory = Daisy->getSystem<Systems::Factory>();

    /* Moonward Logo Small */
		//GameObjectPtr chenObj = ConstructGameObject("Logo1");
		//chenObj->AddComponent(ComponentPtr(new Transform(*chenObj)));
		//chenObj->AddComponent(ComponentPtr(new Sprite(*chenObj)));
		//chenObj->AddComponent(ComponentPtr(new DebugReport(*chenObj)));
		//// Transform properties
		//chenObj->getComponent<Transform>()->Translation = Vec3(25, 20, 0.0f);
		//chenObj->getComponent<Transform>()->Scale = Vec3(1.50f, 1.0f, 1.0f);
		//chenObj->getComponent<Transform>()->Rotation = Vec3(0.0f, 0.0f, 2.0f);
		//// Sprite properties
		//chenObj->getComponent<Sprite>()->SpriteSource = "awesomeface";

    /* Moonward Logo */
		//GameObjectPtr connorObj = ConstructGameObject("Connor");
		//connorObj->AddComponent(ComponentPtr(new Transform(*connorObj)));
		//connorObj->AddComponent(ComponentPtr(new Sprite(*connorObj)));
  //  connorObj->getComponent<Sprite>()->SpriteSource = "logo";
		//connorObj->AddComponent(ComponentPtr(new DebugReport(*connorObj)));
		//// Transform properties
		//connorObj->getComponent<Transform>()->Translation = Vec3(-90, 50, -2.0f);
		//connorObj->getComponent<Transform>()->Scale = Vec3(15.0f, 10.0f, 1.0f);

    /* Faded Curtain Effect */
    //GameObjectPtr curtain = ConstructGameObject("Curtain");
    //curtain->AddComponent(ComponentPtr(new Transform(*curtain)));
    //curtain->AddComponent(ComponentPtr(new Sprite(*curtain)));
    //curtain->AddComponent(ComponentPtr(new DebugFade(*curtain)));
    //curtain->getComponent<Sprite>()->Color = Vec4(0, 0, 0, 1);
    //curtain->getComponent<Transform>()->Scale = Vec3(150, 150, 1.0f);
    //// Transform properties
    //curtain->getComponent<Transform>()->Translation = Vec3(0, 0, 0.1);




	}
}