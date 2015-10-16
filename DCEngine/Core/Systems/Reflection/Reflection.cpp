#include "Reflection.h"

namespace DCEngine {

  namespace Systems {

    Reflection::Reflection() : System(std::string("ReflectionSystem"), EnumeratedSystem::Reflection) {
    }

    void Reflection::Initialize()
    {
    }
    void Reflection::Update(float dt)
    {
    }
    void Reflection::Terminate()
    {
    }
    void Reflection::Serialize(Json::Value & root)
    {
    }
    void Reflection::Deserialize(Json::Value & root)
    {
    }

    void Reflection::ReflectionTest()
    {
    }
  }


}