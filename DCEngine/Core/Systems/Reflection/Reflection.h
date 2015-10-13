#pragma once
#include "../System.h"


namespace DCEngine {
  namespace Systems {

    class Engine;
    class Reflection : public System {
      friend class Engine;
    public:


    private:
      Reflection();
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);


    };


  }


}