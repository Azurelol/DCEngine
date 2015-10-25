//#include "Gallery.h"
#include "Dollhouse.h"

namespace DCEngine {

	/* Chen, write your tests here. */
	void DollHouse::SetupBackground()
	{
		// Reference to the factory system
		auto factory = Daisy->getSystem<Systems::Factory>(EnumeratedSystem::Factory);

		GameObjectPtr chenObj = ConstructGameObject("Chen");
		chenObj->AddComponent(ComponentPtr(new Transform(*chenObj)));
		chenObj->AddComponent(ComponentPtr(new Sprite(*chenObj)));
		chenObj->AddComponent(ComponentPtr(new DebugReport(*chenObj)));
		// Transform properties
		chenObj->getComponent<Transform>()->Translation = Real3(5, 5, 0.0f);
		chenObj->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 1.0f);
		chenObj->getComponent<Transform>()->Rotation = Real3(0.0f, 0.0f, 2.0f);
		// Sprite properties
		chenObj->getComponent<Sprite>()->Color = Real4(1, 1, 0, 0.5); // Color: Blue
		chenObj->getComponent<Sprite>()->SpriteSource = "Angryeyes";
		// DebugDraw properties
		//chenObj->getComponent<DebugReport>()->DrawType = DebugDrawType::Circle; // Purple circle
		//chenObj->getComponent<DebugReport>()->Radius = 15;
		//chenObj->getComponent<DebugReport>()->Color = Real4(1, 1, 1, 1);

		GameObjectPtr connorObj = ConstructGameObject("Connor");
		connorObj->AddComponent(ComponentPtr(new Transform(*connorObj)));
		connorObj->AddComponent(ComponentPtr(new Sprite(*connorObj)));
		connorObj->AddComponent(ComponentPtr(new DebugReport(*connorObj)));
		// Transform properties
		connorObj->getComponent<Transform>()->Translation = Real3(-5, 5, 0.0f);
		connorObj->getComponent<Transform>()->Scale = Real3(5.0f, 5.0f, 1.0f);
		// Sprite properties
		connorObj->getComponent<Sprite>()->Color = Real4(1, 0.5, 0, 0.7); // Color: Orange

																	  // DebugDraw properties
		//connorObj->getComponent<DebugReport>()->DrawType = DebugDrawType::Rectangle;
		//connorObj->getComponent<DebugReport>()->Radius = 5.0f;
		//connorObj->getComponent<DebugReport>()->Height = 10.0f;
		connorObj->getComponent<DebugReport>()->Offset = Real3(5, 5, 3);
		connorObj->getComponent<DebugReport>()->Color = Real4(1, 1, 0, 1); // Color: Yellow

																		   /* SPRITE TEXT */
																		   //auto st = factory->CreateGameObject(String("SpriteTextObj"), *SpaceRef, false);
																		   ////st->AddComponent(factory->CreateComponent<Transform>(*st, false));
																		   //st->AddComponent(factory->CreateComponent<SpriteText>(*st, false));
																		   //AddGameObject(st);

		GameObjectPtr boro = ConstructGameObject("SpriteTextor");
		boro->AddComponent(ComponentPtr(new Transform(*boro)));
		boro->AddComponent(ComponentPtr(new SpriteText(*boro)));
		// SpriteText properties
		boro->getComponent<SpriteText>()->Font = "Verdana";
		boro->getComponent<SpriteText>()->Text = "This is our jam!";


	}
}