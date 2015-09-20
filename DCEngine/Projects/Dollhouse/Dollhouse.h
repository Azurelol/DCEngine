#pragma once
#include "..\..\Core\Resources\Level.h"

// Testing
#include "..\..\Tutorials\OpenGL\GLCameraTutorial.h"
#include "..\..\Core\Objects\Entity.h"
#include "..\..\Core\ComponentsInclude.h"

namespace DCEngine {

  class Space;
  //class Entity;
  class GameSession;

  class DollHouse : public Level {
  public:
    DollHouse(Space& space, GameSession& gamesession) : Level("Dollhouse"), 
                            space_(&space), gamesession_(&gamesession) {
      
      // THIS IS MY PSEUDO-SERIALIZATION JAM!

      //doll.reset(new GameObject("Doll", *space_, *gamesession_));
      //AddGameObject(doll);
      //Entity &dollRef = *doll;
      //ComponentPtr glTut = ComponentPtr(new Tutorial::GLCameraTutorial(dollRef));
      //ComponentPtr transform = ComponentPtr(new Transform(dollRef));
      //ComponentPtr sprite = ComponentPtr(new Sprite(dollRef));
      //doll->AddComponent(transform);
      //doll->AddComponent(sprite);
      //doll->AddComponent(glTut);

      GameObjectPtr rango = ConstructGameObject("Rango");
      rango->getComponent<Transform>()->Translation = Real3(1.0f, 1.0f, 1.0f);
      rango->getComponent<Sprite>()->setSpriteSource("angryeyes.png");

      //rango->getComponent<Sprite>()->getSpriteSource()->
      
      //GameObjectPtr khasox = ConstructGameObject("Khasocks");
      //GameObjectPtr bankplank = ConstructGameObject("Bankplank");
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
      gameObj->AddComponent(ComponentPtr(new Sprite(*gameObj)));
      gameObj->AddComponent(ComponentPtr(new Transform(*gameObj)));
      
      return gameObj;
    }

    GameObjectPtr doll;

    // (TEMP) Resources should not contain data of their owners
    Space* space_;
    GameSession* gamesession_;

  };


}