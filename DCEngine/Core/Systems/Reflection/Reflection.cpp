#include "Reflection.h"

namespace DCEngine {
  
  namespace Systems {

    Reflection::Reflection() : System(std::string("ReflectionSystem"), EnumeratedSystem::Reflection) {
    }

    void Reflection::Initialize()
    {
      RegisterTypes();
    }
    void Reflection::Update(float dt)
    {

    }
    void Reflection::Terminate()
    {
    }
    void Reflection::RegisterTypes()
    {

      //META_ADD_CLASS_MEMFN_CPP(double);
      //META_ADD_CLASS_MEMFN_CPP(Real);
      //META_ADD_CLASS_MEMFN_CPP(std::string);
    }
    void Reflection::Serialize(Json::Value & root)
    {
    }
    void Reflection::Deserialize(Json::Value & root)
    {
    }
  }


}