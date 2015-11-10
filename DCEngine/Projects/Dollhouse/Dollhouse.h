#pragma once
#include "..\..\Core\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"
#include "..\..\Core\Components\EngineReference.h"
#include "..\..\Core\Objects\Entity.h"
#include "..\..\Core\ComponentsInclude.h"
#include "..\..\Core\Systems\Serialization\Serialization.h"
#include "..\..\Core\Systems\Factory\Factory.h"
#include "ReflectionTest.h"

namespace DCEngine {
  
  class Space;
  class GameSession;

  class DollHouse : public Level {
  public:
    DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"), 
                            space_(&space), gamesession_(&gamesession) {      

      // Setup Camera to be used
      SetupCamera();
      // Graphics Testing: Chen
      SetupBackground();
      // Physics Testing: Blaine
      GameObjects();
      // Reflection Testing:
      //ReflectionTest();
      // Serialization Testing
      SerializeTest();  
      // Background objects
      EnvironmentSetup();
    } 

    void SetupCamera() {
      auto factory = Daisy->getSystem<Systems::Factory>();

      //auto cam = factory->CreateGameObject(String("FactoryTesty"), *space_, false);
      //cam->AddComponent(factory->CreateComponent<Transform>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<Camera>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<DebugReport>(*cam, false));
      //cam->AddComponent(factory->CreateComponent<DebugCamera>(*cam, false));
      //cam->getComponent<Transform>()->Translation = Vec3(1.0f, 1.0f, 1.0f);
      //cam->getComponent<Camera>()->Size = 50;

      GameObjectPtr cameraObj = ConstructGameObject("Camera");
      cameraObj->AddComponent(ComponentPtr(new Transform(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
      //cameraObj->AddComponent(ComponentPtr(new DebugReport(*cameraObj)));      
      cameraObj->AddComponent(ComponentPtr(new DebugCamera(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new DebugAudio(*cameraObj)));
      cameraObj->getComponent<DebugAudio>()->Track1 = "soulja";
      cameraObj->getComponent<DebugAudio>()->Track2 = "spacejam2";
      // Camera properties      
      cameraObj->getComponent<Transform>()->Translation = Vec3(1.0f, 1.0f, 1.0f);
      cameraObj->getComponent<Camera>()->Size = 70;
      cameraObj->getComponent<Camera>()->Projection = ProjectionMode::Perspective;

      // Test to serialize the camera component settings:
      //std::string cameraData;
      //Serialization::Serialize(cameraObj->getComponent<Camera>(), cameraData);
      //DCTrace << "Camera Serialized Output\n" << cameraData << "\n\n";
      // Let's try having a background sprite, should be drawn behind others.
    }

    /* Tests Serialization with JSONCPP */
    void SerializeTest() {
      //// Simple entity
      //GameObjectPtr rango = ConstructGameObject("Rango");
      //rango->AddComponent(ComponentPtr(new Sprite(*rango)));
      //rango->AddComponent(ComponentPtr(new Transform(*rango)));
      //rango->getComponent<Transform>()->Translation = Vec3(2.0f, 1.0f, -3.0f);

      //// Serialization test
      //std::string input = "{ \"Name\" : \"Pikapikano!\"}\n";
      //Serialization::Deserialize(rango.get(), input);
      //DCTrace << "Raw Json Input\n" << input << "\n\n";
      //std::string output;
      //Serialization::Serialize(rango.get(), output);
      //DCTrace << "GameObject Serialized Output\n" << output << "\n\n";
    }

    GameObjectPtr ConstructGameObject(std::string name) {
      GameObjectPtr gameObj(new GameObject(name, *space_, *gamesession_));
      AddGameObject(gameObj);

      return gameObj;
    }

    void GameObjects();
    void SetupBackground();
    void EnvironmentSetup();

    GameObjectPtr doll;

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };
}