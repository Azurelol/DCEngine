//#include "Gallery.h"
#include "Dollhouse.h"

namespace DCEngine {

	/* Chen, write your tests here. */
	void DollHouse::SetupBackground()
	{
		// Reference to the factory system
		auto factory = Daisy->getSystem<Systems::Factory>(EnumeratedSystem::Factory);

    /* Moonward Logo??? */
		GameObjectPtr chenObj = ConstructGameObject("Chen");
		chenObj->AddComponent(ComponentPtr(new Transform(*chenObj)));
		chenObj->AddComponent(ComponentPtr(new Sprite(*chenObj)));
		chenObj->AddComponent(ComponentPtr(new DebugReport(*chenObj)));
		// Transform properties
		chenObj->getComponent<Transform>()->Translation = Real3(15, 15, 0.0f);
		chenObj->getComponent<Transform>()->Scale = Real3(2.0f, 2.0f, 1.0f);
		chenObj->getComponent<Transform>()->Rotation = Real3(0.0f, 0.0f, 2.0f);
		// Sprite properties
		//chenObj->getComponent<Sprite>()->Color = Real4(1, 1, 0, 0.5); // Color: Blue
		chenObj->getComponent<Sprite>()->SpriteSource = "awesomeface";

    /* Moonward Logo */
		GameObjectPtr connorObj = ConstructGameObject("Connor");
		connorObj->AddComponent(ComponentPtr(new Transform(*connorObj)));
		connorObj->AddComponent(ComponentPtr(new Sprite(*connorObj)));
    connorObj->getComponent<Sprite>()->SpriteSource = "awesomeface";
		connorObj->AddComponent(ComponentPtr(new DebugReport(*connorObj)));
		// Transform properties
		connorObj->getComponent<Transform>()->Translation = Real3(-45, 35, -2.0f);
		connorObj->getComponent<Transform>()->Scale = Real3(10.0f, 10.0f, 1.0f);
    
    /* SpriteText! */
    GameObjectPtr boro = ConstructGameObject("SpriteTextor");
		boro->AddComponent(ComponentPtr(new Transform(*boro)));
		boro->AddComponent(ComponentPtr(new SpriteText(*boro)));
		// SpriteText properties
		boro->getComponent<SpriteText>()->Font = "Verdana";
		boro->getComponent<SpriteText>()->Text = "This is our jam!";




	}
}