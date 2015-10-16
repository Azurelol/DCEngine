#pragma once
#include "..\..\Core\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"
#include "..\..\Core\Components\EngineReference.h"
#include "..\..\Core\Objects\Entity.h"
#include "..\..\Core\ComponentsInclude.h"
#include "..\..\Core\Systems\Factory\Serializer.h"
#include "..\..\Core\Systems\Factory\Factory.h"
#include "ReflectionTest.h"

namespace DCEngine {
  
  class Space;
  //class Entity;
  class GameSession;

  class DollHouse : public Level {
  public:
    DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"), 
                            space_(&space), gamesession_(&gamesession) {      

      // Setup Camera to be used
      SetupCamera();
      // Graphics Testing: Chen
      GalleryTesting();
      // Physics Testing: Blaine
      LaboratoryTesting();
      // Reflection Testing:
      ReflectionTest();
      // Serialization Testing
      SerializeTest();      
    }  

    void SetupCamera() {
      auto factory = Daisy->getSystem<Systems::Factory>(EnumeratedSystem::Factory);

      auto cam = factory->CreateGameObject(String("FactoryTesty"), *space_, false);
      cam->AddComponent(factory->CreateComponent<Transform>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<Camera>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<DebugReport>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<DebugCamera>(*cam, false));
      //cam->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
      //cam->getComponent<Camera>()->Size = 50;

      GameObjectPtr cameraObj = ConstructGameObject("Camera");
      cameraObj->AddComponent(ComponentPtr(new Transform(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
      //cameraObj->AddComponent(ComponentPtr(new DebugReport(*cameraObj)));      
      cameraObj->AddComponent(ComponentPtr(new DebugCamera(*cameraObj)));
      // Camera properties      
      cameraObj->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
      cameraObj->getComponent<Camera>()->Size = 70;
      cameraObj->getComponent<Camera>()->Projection = ProjectionMode::Orthographic;

      // Test to serialize the camera component settings:
      std::string cameraData;
      Serializer::Serialize(cameraObj->getComponent<Camera>(), cameraData);
      trace << "Camera Serialized Output\n" << cameraData << "\n\n";
      // Let's try having a background sprite, should be drawn behind others.
    }

    /* Tests Serialization with JSONCPP */
    void SerializeTest() {
      // Simple entity
      GameObjectPtr rango = ConstructGameObject("Rango");
      rango->AddComponent(ComponentPtr(new Sprite(*rango)));
      rango->AddComponent(ComponentPtr(new Transform(*rango)));
      rango->getComponent<Transform>()->Translation = Real3(2.0f, 1.0f, -3.0f);

      // Serializer test
      std::string input = "{ \"Name\" : \"Pikapikano!\"}\n";
      Serializer::Deserialize(rango.get(), input);
      trace << "Raw Json Input\n" << input << "\n\n";
      std::string output;
      Serializer::Serialize(rango.get(), output);
      trace << "GameObject Serialized Output\n" << output << "\n\n";
    }

    GameObjectPtr ConstructGameObject(std::string name) {
      GameObjectPtr gameObj(new GameObject(name, *space_, *gamesession_));
      AddGameObject(gameObj);

      return gameObj;
    }

    void LaboratoryTesting();
    void GalleryTesting();

    GameObjectPtr doll;

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };

	class Space;
	//class Entity;
	class GameSession;

	class DollHouse : public Level {
	public:
		DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"),
			space_(&space), gamesession_(&gamesession) {

			// Setup Camera to be used
			SetupCamera();
			// Graphics Testing: Chen
			GalleryTesting();
			// Physics Testing: Blaine
			LaboratoryTesting();
			// Serialization Testing
			SerializeTest();
		}

		void SetupCamera() {
			auto factory = Daisy->getSystem<Systems::Factory>(EnumeratedSystem::Factory);

			auto cam = factory->CreateGameObject(String("FactoryTesty"), *space_, false);
			cam->AddComponent(factory->CreateComponent<Transform>(*cam, false));
			//cam->AddComponent(factory->CreateComponent<Camera>(*cam, false));
			//cam->AddComponent(factory->CreateComponent<DebugReport>(*cam, false));
			//cam->AddComponent(factory->CreateComponent<DebugCamera>(*cam, false));
			//cam->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
			//cam->getComponent<Camera>()->Size = 50;

			GameObjectPtr cameraObj = ConstructGameObject("Camera");
			cameraObj->AddComponent(ComponentPtr(new Transform(*cameraObj)));
			cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
			//cameraObj->AddComponent(ComponentPtr(new DebugReport(*cameraObj)));      
			cameraObj->AddComponent(ComponentPtr(new DebugCamera(*cameraObj)));
			// Camera properties      
			cameraObj->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
			cameraObj->getComponent<Camera>()->Size = 70;
			cameraObj->getComponent<Camera>()->Projection = ProjectionMode::Perspective;

			// Test to serialize the camera component settings:
			std::string cameraData;
			Serializer::Serialize(cameraObj->getComponent<Camera>(), cameraData);
			trace << "Camera Serialized Output\n" << cameraData << "\n\n";
			// Let's try having a background sprite, should be drawn behind others.
		}


		/* Tests Reflection with Gaul's MetaData */
		void ReflectionTest() {

			//DEFINE_META(int);
			//DEFINE_META(float);
			//DEFINE_META(double);
			//DEFINE_META(std::string);

			//trace << "-- Reflection Test -- \n";
			//trace << META_TYPE(int)->Name() << "\n"; // "int"
			//trace << META_TYPE(float)->Size() << "\n"; // "4"

			//std::string word = "This is a word.";
			//trace << "Type of word object: " << META(word)->Name() << "\n"; 
			//trace << "Size of double: " << META_STR("double")->Size() << "\n";
			//
			//if (META(word) != META_TYPE(int)) {
			//  trace << "The word object is not an int!\n";
			//}


		}

		/* Tests Serialization with JSONCPP */
		void SerializeTest() {
			// Simple entity
			GameObjectPtr rango = ConstructGameObject("Rango");
			rango->AddComponent(ComponentPtr(new Sprite(*rango)));
			rango->AddComponent(ComponentPtr(new Transform(*rango)));
			rango->getComponent<Transform>()->Translation = Real3(2.0f, 1.0f, -3.0f);

			// Serializer test
			std::string input = "{ \"Name\" : \"Pikapikano!\"}\n";
			Serializer::Deserialize(rango.get(), input);
			trace << "Raw Json Input\n" << input << "\n\n";
			std::string output;
			Serializer::Serialize(rango.get(), output);
			trace << "GameObject Serialized Output\n" << output << "\n\n";
		}

		GameObjectPtr ConstructGameObject(std::string name) {
			GameObjectPtr gameObj(new GameObject(name, *space_, *gamesession_));
			AddGameObject(gameObj);

			return gameObj;
		}

		void LaboratoryTesting();
		void GalleryTesting();

		GameObjectPtr doll;

		// (TEMP) Resources should not contain data of their owners
		Space* space_;
		GameSession* gamesession_;

	};




}