#pragma once
#include "..\..\Core\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"
#include "..\..\Core\Components\EngineReference.h"
#include "..\..\Core\Objects\Entity.h"
#include "..\..\Core\ComponentsInclude.h"
#include "..\..\Core\Systems\Factory\Serializer.h"
#include "..\..\Core\Systems\Factory\Factory.h"

namespace DCEngine {

  class Space;
  //class Entity;
  class GameSession;

  class DollHouse : public Level {
  public:
    DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"), 
                            space_(&space), gamesession_(&gamesession) {      

      auto factory = Daisy->getSystem<Systems::Factory>(EnumeratedSystem::Factory);
      
      auto cam = factory->CreateGameObject(String("FactoryTesty"),*space_, false);
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


      // Graphics Testing: Chen
      GalleryTesting();
      // Physics Testing: Blaine
      LaboratoryTesting();
      // Serialization Testing

      //SerializeTest();
      
    }
    
    // Constructs a GameObject and loads some components onto it
    GameObjectPtr ConstructGameObject(std::string name) {
      GameObjectPtr gameObj(new GameObject(name, *space_, *gamesession_));
      AddGameObject(gameObj);
      
      return gameObj;
    }

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
    void LaboratoryTesting();
    void GalleryTesting();

    GameObjectPtr doll;

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };


}