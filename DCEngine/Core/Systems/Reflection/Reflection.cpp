#include "Reflection.h"

// Include components so they get added to meta??
#include "../../ComponentsInclude.h"

namespace DCEngine {

  META_ADD_POD(int);
  META_ADD_POD(int*);
  META_ADD_POD(double);
  META_ADD_POD(float);
  META_ADD_POD(bool);
  META_ADD_POD(char);
  META_ADD_POD(char*);
  META_ADD_POD(std::string);

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