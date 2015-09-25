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
      
      // THIS IS MY PSEUDO-SERIALIZATION JAM!

      // Camera object

      // Run the GLCameraTutorial
      auto glCamTut = Daisy->getFactory().CreateGameObject(space, false);
      AddGameObject(glCamTut);
      //glCamTut->AddComponent(ComponentPtr(new Tutorial::GLCameraTutorial(*glCamTut)));

      // Default camera object for the space
      GameObjectPtr cameraObj = ConstructGameObject("Camera");
      cameraObj->AddComponent(ComponentPtr(new Transform(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
      cameraObj->AddComponent(ComponentPtr(new Camera(*cameraObj)));
      
      cameraObj->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 5.0f);
      
      // Simple entity
      GameObjectPtr gameObj1 = ConstructGameObject("Reiner");
      gameObj1->AddComponent(ComponentPtr(new Sprite(*gameObj1)));
      gameObj1->AddComponent(ComponentPtr(new Transform(*gameObj1)));
      gameObj1->AddComponent(ComponentPtr(new DebugMoveController(*gameObj1)));
	  gameObj1->AddComponent(ComponentPtr(new RigidBody(*gameObj1)));
      gameObj1->AddComponent(ComponentPtr(new DebugReport(*gameObj1)));
      gameObj1->getComponent<Transform>()->Translation = Real3(2.0f, 1.0f, 3.0f);
      gameObj1->getComponent<Sprite>()->setSpriteSource("angryeyes.png");

      // Simple entity
      GameObjectPtr gameObj2 = ConstructGameObject("Myres");
      gameObj2->AddComponent(ComponentPtr(new Sprite(*gameObj2)));
      gameObj2->AddComponent(ComponentPtr(new Transform(*gameObj2)));
      gameObj2->AddComponent(ComponentPtr(new DebugReport(*gameObj2)));
      gameObj2->getComponent<Transform>()->Translation = Real3(2.0f, 1.0f, 3.0f);
      gameObj2->getComponent<Sprite>()->setSpriteSource("angryeyes.png");


      //SerializeTest();
      
    }
    
    // Constructs a GameObject and loads some components onto it
    GameObjectPtr ConstructGameObject(std::string name) {
      GameObjectPtr gameObj(new GameObject(name, *space_, *gamesession_));

      AddGameObject(gameObj);
      Entity &gameObjRef = *gameObj;
      //ComponentPtr glTut = ComponentPtr(new Tutorial::GLCameraTutorial(gameObjRef));
      //ComponentPtr transform = ComponentPtr(new Transform(*gameObj));
      //ComponentPtr sprite = ComponentPtr(new Sprite(*gameObj));
      //gameObj->AddComponent(sprite);
      //gameObj->AddComponent(glTut);
      
      return gameObj;
    }

    void SerializeTest() {
      // Simple entity
      GameObjectPtr rango = ConstructGameObject("Rango");
      rango->AddComponent(ComponentPtr(new Sprite(*rango)));
      rango->AddComponent(ComponentPtr(new Transform(*rango)));
      rango->getComponent<Transform>()->Translation = Real3(2.0f, 1.0f, -3.0f);
      rango->getComponent<Sprite>()->setSpriteSource("angryeyes.png");

      // Serializer test
      std::string input = "{ \"Name\" : \"Pikapikano!\"}\n";
      Serializer::Deserialize(rango.get(), input);
      trace << "Raw Json Input\n" << input << "\n\n";
      std::string output;
      Serializer::Serialize(rango.get(), output);
      trace << "GameObject Serialized Output\n" << output << "\n\n";
    }

    GameObjectPtr doll;

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };


}